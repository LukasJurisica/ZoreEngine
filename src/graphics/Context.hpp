#pragma once

class Context {
public:
	virtual void init() = 0;
	virtual void swapBuffers() = 0;
};