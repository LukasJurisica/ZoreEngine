#pragma once

#include <glfw/glfw3.h>

#include <string>
#include <map>

#include "graphics/opengl/OpenGLContext.hpp"
#include "window/InputCodes.hpp"

class Window {
public:
	Window(std::map<std::string, int> options);
	~Window();

	float getAspectRatio();
	bool shouldClose();
	bool keyPressed(int key);
	bool mousePressed(int button);

	void update();
	void setResolution(int width, int height);
	void setViewport(int x, int y, int width, int height);
	void setVSync(bool enabled);

	enum class ScreenMode {
		Fullscreen,
		Windowed,
		Borderless
	};

private:
	void init();

	struct WindowData {
		int width;
		int height;
		ScreenMode screenMode;
		const char* title;
		bool vSync;
	};

	WindowData windowData;
	GLFWwindow* window;
	Context* context;
};

namespace WindowCallback {
	void errorCallback(int error, const char* description);
	void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	void cursorPositionCallback(GLFWwindow* window, double xpos, double ypos);
	void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
	void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
}