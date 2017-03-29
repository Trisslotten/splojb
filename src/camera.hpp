#pragma once

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


class Camera
{

	float fov = 70;


	double lastx = 0;
	double lasty = 0;
public:

	float yaw, pitch;

	glm::vec3 pos, look_vec;

	float fov_rad = glm::radians(70.f);

	Camera() : pos(0, 3, 0)
	{
		yaw = 0;
		pitch = 0;
	}

	void update(double dt)
	{
		GLFWwindow* window = glfwGetCurrentContext();
		int lshift = glfwGetKey(window, GLFW_KEY_LEFT_SHIFT);

		float speed = 3;
		if (lshift == GLFW_PRESS)
		{
			speed = 10;
		}

		int space = glfwGetKey(window, GLFW_KEY_SPACE);
		if (space == GLFW_PRESS)
		{
			pos.y += speed*dt;
		}
		int ctrl = glfwGetKey(window, GLFW_KEY_LEFT_CONTROL);
		if (ctrl == GLFW_PRESS)
		{
			pos.y -= speed*dt;
		}

		glm::vec3 walk_dir(0, 0, 0);
		int w = glfwGetKey(window, GLFW_KEY_W);
		if (w == GLFW_PRESS)
		{
			walk_dir.x += 1.0;
		}
		int s = glfwGetKey(window, GLFW_KEY_S);
		if (s == GLFW_PRESS)
		{
			walk_dir.x -= 1.0;
		}
		int a = glfwGetKey(window, GLFW_KEY_A);
		if (a == GLFW_PRESS)
		{
			walk_dir.z -= 1.0;
		}
		int d = glfwGetKey(window, GLFW_KEY_D);
		if (d == GLFW_PRESS)
		{
			walk_dir.z += 1.0;
		}
		if (glm::length(walk_dir) > 0.5)
		{

		}

		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		float deltay = ypos - lasty;
		float deltax = xpos - lastx;
		lasty = ypos;
		lastx = xpos;

		pitch -= deltay / 200;
		yaw -= deltax / 200;
		pitch = glm::clamp(pitch, -1.57f, 1.57f);

		glm::mat4 yaw_matrix = glm::rotate(glm::mat4(), yaw, glm::vec3(0, 1, 0));
		glm::mat4 rot_matrix = glm::rotate(yaw_matrix, pitch, glm::vec3(0, 0, 1));


		if (glm::length(walk_dir) > 0.5)
		{
			glm::vec4 forward_hom(normalize(walk_dir), 0);
			forward_hom = rot_matrix*forward_hom;
			glm::vec3 forward(forward_hom);
			pos += forward*speed * (float)dt;
		}






		look_vec = glm::vec3(1, 0, 0);
		look_vec = rot_matrix*glm::vec4(look_vec.x, look_vec.y, look_vec.z, 1.0);

	}

	glm::mat4 getTransform(GLFWwindow* window)
	{
		glm::mat4 view = glm::lookAt(
			pos,
			pos + look_vec,
			glm::vec3(0, 1, 0)
		);
		glm::mat4 projection = glm::perspective<float>(fov_rad, 16.0 / 9.0, 0.1, 2000);

		return projection*view;
	}
};

