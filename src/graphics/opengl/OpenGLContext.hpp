#pragma once

#include "graphics/Context.hpp"

class OpenGLContext : public Context {
public:
	OpenGLContext(void* window);
	void init();

private:
	void* m_window;
};