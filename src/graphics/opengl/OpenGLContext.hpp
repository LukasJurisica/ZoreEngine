#pragma once

#include "graphics/Context.hpp"

class OpenGLContext : public Context {
public:
	OpenGLContext(void* window);

	void init();
	void swapBuffers();
private:
	void* window;
};