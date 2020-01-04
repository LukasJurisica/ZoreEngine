#include "window/Window.hpp"

#include <glfw/glfw3.h>

#include "graphics/opengl/OpenGLContext.hpp"
#include "core/Application.hpp"
#include "utils/Logger.hpp"

Window::Window(std::map<std::string, int> options, void* application) {
	// Initialize member variables
	m_windowData.width = options["width"];
	m_windowData.height = options["height"];
	switch (options["fullscreen"]) {
	case 0:
		m_windowData.screenMode = ScreenMode::Fullscreen; break;
	case 1:
		m_windowData.screenMode = ScreenMode::Windowed; break;
	case 2:
		m_windowData.screenMode = ScreenMode::Borderless; break;
	}
	m_windowData.vSync = options["vsync"];
	m_windowData.title = "Voxel Game";
	m_windowData.application = application;

	init();
}

void Window::init() {
	// Initialize GLFW
	if (glfwInit() == GLFW_FALSE)
		Logger::error("Failed to init GLFW");
	
	// Create GLFW window and OpenGL context
	GLFWmonitor* monitor = m_windowData.screenMode == ScreenMode::Fullscreen ? glfwGetPrimaryMonitor() : NULL;
	m_window = glfwCreateWindow(m_windowData.width, m_windowData.height, m_windowData.title, monitor, NULL);
	if (m_window == NULL)
		Logger::error("Failed to create GLFW window");
	glfwSetWindowUserPointer(m_window, &m_windowData);
	m_context = new OpenGLContext(m_window);
	m_context->init();

	// Enable Various features
	if (glfwRawMouseMotionSupported())
		glfwSetInputMode(m_window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
	glfwSwapInterval(m_windowData.vSync);
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Set GLFW event callbacks
	glfwSetErrorCallback(WindowCallback::errorCallback);
	glfwSetKeyCallback(m_window, WindowCallback::keyCallback);
	glfwSetCursorPosCallback(m_window, WindowCallback::cursorPositionCallback);
	glfwSetMouseButtonCallback(m_window, WindowCallback::mouseButtonCallback);
	glfwSetScrollCallback(m_window, WindowCallback::scrollCallback);
	glfwSetWindowSizeCallback(m_window, WindowCallback::windowSizeCallback);

	setViewport(0, 0, m_windowData.width, m_windowData.height);
}

bool Window::shouldClose() {
	return glfwWindowShouldClose(m_window);
}

bool Window::keyPressed(int key) {
	return (glfwGetKey(m_window, key));
}

bool Window::mousePressed(int button) {
	return (glfwGetMouseButton(m_window, button));
}

void Window::update() {
	glfwPollEvents();
	glfwSwapBuffers(m_window);
}

void Window::setResolution(int width, int height) {
	glfwSetWindowSize(m_window, width, height);
	m_windowData.width = width;
	m_windowData.height = height;
	glViewport(0, 0, width, height);
}

void Window::updateResolution(int width, int height) {
	m_windowData.width = width;
	m_windowData.height = height;
	glViewport(0, 0, width, height);
}

void Window::setViewport(int x, int y, int width, int height) {
	glViewport(x, y, width, height);
}

void Window::setVSync(bool enabled) {
	if (m_windowData.vSync != enabled) {
		glfwSwapInterval(enabled);
		m_windowData.vSync = enabled;
	}
}

Window::~Window() {};

namespace WindowCallback {
	void errorCallback(int error, const char* description) {
		Logger::error("(" + std::to_string(error) + "): " + description);
	}

	void windowSizeCallback(GLFWwindow* window, int width, int height) {
		Window::WindowData* windowData = (Window::WindowData*)glfwGetWindowUserPointer(window);

		Application* app = (Application*)(windowData->application);
		app->handleEvent(new WindowResizeEvent(width, height));
	}

	void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
		if (key == KEY_ESCAPE) {
			glfwSetWindowShouldClose(window, true);
		}
	}

	void cursorPositionCallback(GLFWwindow* window, double xpos, double ypos) {
		Window::WindowData* windowData = (Window::WindowData*)glfwGetWindowUserPointer(window);

		Application* app = (Application*)(windowData->application);
		app->handleEvent(new MouseMoveEvent((float)xpos, (float)ypos));
	}

	void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {

	}

	void scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {

	}
}