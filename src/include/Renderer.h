#pragma once
#include<GL/glew.h>
#include <GLFW/glfw3.h>

class Renderer{
	GLFWwindow* window;
	Renderer(GLFWwindow* window);
	void loop();
};
