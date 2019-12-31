#include "window/Window.hpp"

#include <glfw/glfw3.h>

#include "graphics/opengl/OpenGLContext.hpp"
#include "utils/Logger.hpp"

Window::Window() : context(nullptr), window(nullptr), windowData({}) {}

void Window::init(std::map<std::string, int> options) {
	// Initialize GLFW
	if (glfwInit() == GLFW_FALSE)
		Logger::error("Failed to init GLFW");

	// Initialize member variables
	windowData.width = options["width"];
	windowData.height = options["height"];
	switch (options["fullscreen"]) {
	case 0:
		windowData.screenMode = ScreenMode::Fullscreen; break;
	case 1:
		windowData.screenMode = ScreenMode::Windowed; break;
	case 2:
		windowData.screenMode = ScreenMode::Borderless; break;
	}
	windowData.vSync = options["vsync"];
	windowData.title = "Voxel Game";
	
	// Create GLFW window and OpenGL context
	GLFWmonitor* monitor = windowData.screenMode == ScreenMode::Fullscreen ? glfwGetPrimaryMonitor() : NULL;
	window = glfwCreateWindow(windowData.width, windowData.height, windowData.title, monitor, NULL);
	if (window == NULL)
		Logger::error("Failed to create GLFW window");
	glfwSetWindowUserPointer(window, &windowData);
	context = new OpenGLContext(window);
	context->init();

	// Enable Various features
	if (glfwRawMouseMotionSupported())
		glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
	setVSync(windowData.vSync);
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Set GLFW event callbacks
	glfwSetErrorCallback(WindowCallback::errorCallback);
	glfwSetKeyCallback(window, WindowCallback::keyCallback);
	glfwSetCursorPosCallback(window, WindowCallback::cursorPositionCallback);
	glfwSetMouseButtonCallback(window, WindowCallback::mouseButtonCallback);
	glfwSetScrollCallback(window, WindowCallback::scrollCallback);

	setViewport(0, 0, windowData.width, windowData.height);
}

float Window::getAspectRatio() {
	return (float)windowData.width / (float)windowData.height;
}

bool Window::shouldClose() {
	return glfwWindowShouldClose(window);
}

bool Window::keyPressed(int key) {
	return (glfwGetKey(window, key) == GLFW_PRESS);
}

bool Window::mousePressed(int button) {
	return (glfwGetMouseButton(window, button));
}

void Window::update() {
	glfwPollEvents();
	context->swapBuffers();
}

void Window::setResolution(int width, int height) {
	if (windowData.width != width && windowData.height != height) {
		windowData.width = width;
		windowData.height = height;
		glfwSetWindowSize(window, width, height);
		setViewport(0, 0, width, height);
	}
}

void Window::setViewport(int x, int y, int width, int height) {
	glViewport(x, y, width, height);
}

void Window::setVSync(bool enabled) {
	if (windowData.vSync != enabled) {
		glfwSwapInterval(enabled ? 1 : 0);
		windowData.vSync = enabled;
	}
}

Window::~Window() {};

namespace WindowCallback {
	void errorCallback(int error, const char* description) {

	}

	void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
		if (key == KEY_ESCAPE) {
			glfwSetWindowShouldClose(window, true);
		}
	}

	void cursorPositionCallback(GLFWwindow* window, double xpos, double ypos) {

	}

	void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {

	}

	void scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {

	}
}