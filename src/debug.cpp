#include "debug.hpp"


#include <GLFW/glfw3.h>
#include <iostream>

void pauseExit()
{
	glfwTerminate();
	std::cin.get();
	exit(EXIT_FAILURE);
}