#include "renderer.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GLFW/glfw3.h>
#include "logger.hpp"
#include "debug.hpp"
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtx/matrix_interpolation.hpp>


Renderer::Renderer()
{}


Renderer::~Renderer()
{}

void Renderer::init()
{

	int width, height;
	GLFWwindow* window = glfwGetCurrentContext();
	glfwGetFramebufferSize(window, &width, &height);

	if (!shader_geometry.createProgram("assets/shaders/geometry_vert.glsl", "assets/shaders/geometry_frag.glsl"))
	{
		Log::errorln("Could not create geometry-pass shaders");
		pauseExit();
	}
	if (!shader_shading.createProgram("assets/shaders/shading_vert.glsl", "assets/shaders/shading_frag.glsl"))
	{
		Log::errorln("Could not create shading-pass shaders");
		pauseExit();
	}
	if (!shader_postprocess.createProgram("assets/shaders/postprocess_vert.glsl", "assets/shaders/postprocess_frag.glsl"))
	{
		Log::errorln("Could not create postprocess shaders");
		pauseExit();
	}


	shader_postprocess.use();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);


	glGenFramebuffers(1, &g_buffer);
	glBindFramebuffer(GL_FRAMEBUFFER, g_buffer);

	glGenTextures(1, &texture_position);
	glBindTexture(GL_TEXTURE_2D, texture_position);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture_position, 0);

	glGenTextures(1, &texture_color);
	glBindTexture(GL_TEXTURE_2D, texture_color);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_FLOAT, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, texture_color, 0);

	glGenTextures(1, &texture_normal);
	glBindTexture(GL_TEXTURE_2D, texture_normal);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_FLOAT, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, texture_normal, 0);

	glGenTextures(1, &texture_velocity);
	glBindTexture(GL_TEXTURE_2D, texture_velocity);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RG16F, width, height, 0, GL_RG, GL_FLOAT, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, texture_velocity, 0);



	GLuint att[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
	glDrawBuffers(4, att);


	glGenRenderbuffers(1, &depth);
	glBindRenderbuffer(GL_RENDERBUFFER, depth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depth);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		Log::errorln("Framebuffer not complete!");
	}
	else
	{
		Log::debugln("Framebuffer complete");
	}
	glGenFramebuffers(1, &postprocess);
	glBindFramebuffer(GL_FRAMEBUFFER, postprocess);

	glGenTextures(1, &texture_postprocess);
	glBindTexture(GL_TEXTURE_2D, texture_postprocess);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_FLOAT, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture_postprocess, 0);
	glDrawBuffers(1, att);


	glBindFramebuffer(GL_FRAMEBUFFER, 0);


	// quad that covers the window
	GLfloat vertices[] =
	{
		//(x,y,z) (u,v)
		-1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
		-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
		1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
		1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
	};
	glGenVertexArrays(1, &quad_vao);
	glGenBuffers(1, &quad_vbo);
	glBindVertexArray(quad_vao);
	glBindBuffer(GL_ARRAY_BUFFER, quad_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glBindVertexArray(0);

}
void Renderer::update(double dt)
{
	GLFWwindow* window = glfwGetCurrentContext();
	int state = glfwGetKey(window, GLFW_KEY_F5);
	if (state == GLFW_PRESS) {
		shader_geometry.reload();
		shader_shading.reload();
		shader_postprocess.reload();
	}
	camera.update(dt);
}

void Renderer::draw(Entity& entity)
{
	to_render.push_back(entity);
}

void printVec(glm::vec3 vec)
{
	std::cout << vec.x << " " << vec.y << " " << vec.z << "\n";
}

void printVec(glm::vec4 vec)
{
	std::cout << vec.x << " " << vec.y << " " << vec.z << " " << vec.w << "\n";
}


void Renderer::render(double dt)
{
	int width, height;
	GLFWwindow* window = glfwGetCurrentContext();
	glfwGetFramebufferSize(window, &width, &height);	
	

	glBindFramebuffer(GL_FRAMEBUFFER, g_buffer);
	glViewport(0, 0, width, height);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	shader_geometry.use();
	glUniform1i(glGetUniformLocation(shader_geometry.getProgram(), "tex_color"), 6);
	//glUniform1i(glGetUniformLocation(shader_geometry.getProgram(), "tex_color"), 7);
	//glUniform1i(glGetUniformLocation(shader_geometry.getProgram(), "tex_color"), 8);
	glm::mat4 view_projection = camera.getTransform(window);

	
	glUniformMatrix4fv(glGetUniformLocation(shader_geometry.getProgram(), "view_projection"), 1, false, glm::value_ptr(view_projection));
	glUniformMatrix4fv(glGetUniformLocation(shader_geometry.getProgram(), "prev_view_projection"), 1, false, glm::value_ptr(prev_view_projection));
	glUniform1f(glGetUniformLocation(shader_geometry.getProgram(), "dt"), dt);

	prev_view_projection = view_projection;

	for (auto& entity : to_render)
	{

		//glUniform1f(glGetUniformLocation(shader_geometry.getProgram(), "dt"), trans.dt);
		glUniformMatrix4fv(glGetUniformLocation(shader_geometry.getProgram(), "model"), 1, false, glm::value_ptr(entity.model));
		glUniformMatrix4fv(glGetUniformLocation(shader_geometry.getProgram(), "prev_model"), 1, false, glm::value_ptr(entity.prev_model));
		entity.mesh->render();
	}
	to_render.clear();

	
	glBindFramebuffer(GL_FRAMEBUFFER, postprocess);
	shader_shading.use();
	glBindVertexArray(quad_vao);

	glUniform1i(glGetUniformLocation(shader_shading.getProgram(), "tex_position"), 0);
	glUniform1i(glGetUniformLocation(shader_shading.getProgram(), "tex_color"), 1);
	glUniform1i(glGetUniformLocation(shader_shading.getProgram(), "tex_normal"), 2);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture_position);
	glActiveTexture(GL_TEXTURE0+1);
	glBindTexture(GL_TEXTURE_2D, texture_color);
	glActiveTexture(GL_TEXTURE0+2);
	glBindTexture(GL_TEXTURE_2D, texture_normal);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);


	glBindFramebuffer(GL_READ_FRAMEBUFFER, g_buffer);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_DEPTH_BUFFER_BIT, GL_NEAREST);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	shader_postprocess.use();

	glUniform1i(glGetUniformLocation(shader_postprocess.getProgram(), "tex_color"), 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture_postprocess);

	glUniform1i(glGetUniformLocation(shader_postprocess.getProgram(), "tex_velocity"), 1);
	glActiveTexture(GL_TEXTURE0 + 1);
	glBindTexture(GL_TEXTURE_2D, texture_velocity);

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	
}
