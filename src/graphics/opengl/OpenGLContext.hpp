#pragma once

#include "graphics/Context.hpp"

struct GLFWwindow;

class OpenGLContext : public Context {
public:
	OpenGLContext(GLFWwindow* window);

	void init();
	void swapBuffers();
private:
	GLFWwindow* window;
};