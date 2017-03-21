#include "renderer.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GLFW/glfw3.h>
#include "logger.hpp"
#include "debug.hpp"

Renderer::Renderer()
{}


Renderer::~Renderer()
{}

void Renderer::init()
{

	int width, height;
	GLFWwindow* window = glfwGetCurrentContext();
	glfwGetFramebufferSize(window, &width, &height);

	if (!shader.createProgram("assets/shaders/vert.glsl", "assets/shaders/frag.glsl"))
	{
		Log::errorln("Could not create shaders");
		pauseExit();
	}
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
}
void Renderer::update()
{
	GLFWwindow* window = glfwGetCurrentContext();
	int state = glfwGetKey(window, GLFW_KEY_F5);
	if (state == GLFW_PRESS) {
		shader.reload();
	}
}

void Renderer::draw(std::shared_ptr<Mesh> mesh, std::shared_ptr<Material> material, glm::mat4 transform)
{
	RenderObject obj;
	obj.mesh = mesh;
	obj.material = material;
	obj.transform = transform;
	to_render.push_back(obj);
}


void Renderer::render()
{

	float ratio;
	int width, height;
	GLFWwindow* window = glfwGetCurrentContext();
	glfwGetFramebufferSize(window, &width, &height);
	ratio = width / (float)height;
	glViewport(0, 0, width, height);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	shader.use();

	glm::mat4 view = glm::lookAt(glm::vec3(5, 0, 0), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	glm::mat4 projection = glm::perspective(glm::radians(75.f), 16.f / 9, 1.f, 100.f);

	glm::mat4 VP = projection*view;

	glUniformMatrix4fv(glGetUniformLocation(shader.getProgram(), "view_projection"), 1, false, glm::value_ptr(VP));

	for (auto& obj : to_render)
	{
		glUniformMatrix4fv(glGetUniformLocation(shader.getProgram(), "model"), 1, false, glm::value_ptr(obj.transform));

		obj.mesh->render();
	}
	to_render.clear();
}
