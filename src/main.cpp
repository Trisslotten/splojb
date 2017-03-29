#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include "logger.hpp"
#include "engine.hpp"
#include "debug.hpp"

int main(int argc, char *argv[])
{
	Log::showOutput(true);
	Log::whatToShow(true, true, true);
	bool fullscreen = false;
	int width = 1280;
	int height = 720;
	std::string title = "gaem";
	Engine engine;


	if (!glfwInit())
	{
		Log::errorln("Could not init GLFW");
		std::cin.get();
		exit(EXIT_FAILURE);
	}
	
	GLFWmonitor* monitor = nullptr;
	if (fullscreen)
		monitor = glfwGetPrimaryMonitor();
	GLFWwindow* window = glfwCreateWindow(width, height, title.c_str(), monitor, nullptr);
	if (!window)
	{
		Log::errorln("Window or OpenGL context creation failed");
		pauseExit();
	}
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	GLenum err = glewInit();
	if (err != GLEW_OK)
	{
		Log::errorln("Could not init GLEW");
		pauseExit();
	}

	engine.init();

	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);

	double last_time = glfwGetTime();
	while (!glfwWindowShouldClose(window))
	{
		double dt = glfwGetTime() - last_time;
		last_time = glfwGetTime();
		engine.update(dt);
		engine.render();
		glfwSwapBuffers(window);
		glfwPollEvents();
		GLFWwindow* window = glfwGetCurrentContext();
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		{
			glfwSetWindowShouldClose(window, GLFW_TRUE);
		}
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	return EXIT_SUCCESS;
}