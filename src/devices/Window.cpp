#include "devices/Window.hpp"
#include "devices/Keyboard.hpp"
#include "devices/Mouse.hpp"
#include "graphics/RenderEngine.hpp"
#include "debug/Debug.hpp"

#include <glfw/glfw3.h>

namespace zore {

	//========================================================================
	//	Window Class
	//========================================================================

	static glm::ivec2 size = { 0, 0 };
	static glm::ivec2 position = {0, 0};
	static GLFWwindow* windowHandle = nullptr;
	static bool fullscreen = false;
	static bool cursorHidden = false;

	void Window::Init(int width, int height) {
		// Initialize GLFW
		ENSURE(glfwInit(), "Failed to initialize GLFW");
		glfwSetErrorCallback(ErrorCallback);
		glfwWindowHint(GLFW_CENTER_CURSOR, true);

		switch (RenderEngine::GetAPI()) {
		case API::OPENGL:
			glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API); // Initialize OpenGL
			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
			break;
		case API::VULKAN:
			glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API); // Initialize Vulkan
			break;
		default:
			throw ZORE_EXCEPTION("No valid render api selected");
		}
		//glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, true); // Set Transparent Framebuffer
		//glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
		Logger::Info("Window Initialization Complete");


		// Create GLFW window
		size = { width, height };
		windowHandle = glfwCreateWindow(size.x, size.y, "Zore Engine Window", nullptr, nullptr);
		ENSURE(windowHandle, "Failed to create GLFW window");
		glfwMakeContextCurrent(windowHandle);

		// Enable Various features
		if (glfwRawMouseMotionSupported())
			glfwSetInputMode(windowHandle, GLFW_RAW_MOUSE_MOTION, true);
		glfwSetInputMode(windowHandle, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

		// Centre the window
		Centre();

		double xpos, ypos;
		glfwGetCursorPos(windowHandle, &xpos, &ypos);
		Mouse::ClearState(static_cast<float>(xpos), static_cast<float>(ypos));

		// Set GLFW event callbacks
		glfwSetWindowPosCallback(windowHandle, MoveCallback);
		glfwSetWindowSizeCallback(windowHandle, ResizeCallback);
		glfwSetWindowFocusCallback(windowHandle, FocusCallback);
		glfwSetKeyCallback(windowHandle, Keyboard::KeyCallback);
		glfwSetCharCallback(windowHandle, Keyboard::CharCallback);
		glfwSetScrollCallback(windowHandle, Mouse::ScrollCallback);
		glfwSetCursorPosCallback(windowHandle, Mouse::MoveCallback);
		glfwSetMouseButtonCallback(windowHandle, Mouse::ActionCallback);
		Logger::Info("Window Creation Complete");
	}

	void Window::Cleanup() {
		glfwDestroyWindow(windowHandle);
		glfwTerminate();
		Logger::Info("Window Cleanup Complete");
	}

	void Window::Update() {
		glfwPollEvents();
		glfwSwapBuffers(windowHandle);
	}

	bool Window::ShouldClose() {
		return glfwWindowShouldClose(windowHandle);
	}

	void Window::SetTitle(const char* title) {
		glfwSetWindowTitle(windowHandle, title);
	}

	void Window::SetFullscreen(bool value) {
		GLFWmonitor* monitor = value ? glfwGetPrimaryMonitor() : nullptr;
		glfwSetWindowMonitor(windowHandle, monitor, position.x, position.y, size.x, size.y, GLFW_DONT_CARE);
		Centre();
		fullscreen = value;
	}

	void Window::ToggleFullscreen() {
		SetFullscreen(!fullscreen);
	}

	void Window::SetPosition(int x, int y) {
		position = { x, y };
		glfwSetWindowPos(windowHandle, position.x, position.y);
	}

	void Window::Centre() {
		GLFWmonitor* monitor = glfwGetPrimaryMonitor();
		if (monitor) {
			const GLFWvidmode* videoMode = glfwGetVideoMode(monitor);
			position.x = (videoMode->width - size.x) / 2;
			position.y = (videoMode->height - size.y) / 2;
			glfwSetWindowPos(windowHandle, position.x, position.y);
		}
	}

	void Window::SetBorderless(bool value) {
		glfwSetWindowAttrib(windowHandle, GLFW_DECORATED, !value);
	}

	void Window::SetSize(int width, int height) {
		size = { width, height };
		glfwSetWindowSize(windowHandle, width, height);
		ResizeCallback(windowHandle, width, height);
	}

	void Window::HideCursor(bool value) {
		cursorHidden = value;
		glfwSetInputMode(windowHandle, GLFW_CURSOR, value ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);

		if (!value) {
			glfwSetCursorPos(windowHandle, size.x / 2, size.y / 2);
			Mouse::ClearState(static_cast<float>(size.x / 2), static_cast<float>(size.y / 2));
		}
	}

	void Window::ToggleCursor() {
		HideCursor(!cursorHidden);
	}

	float Window::GetAspectRatio() {
		return (static_cast<float>(size.x) / static_cast<float>(size.y));
	}

	const glm::ivec2& Window::GetSize() {
		return size;
	}

	//------------------------------------------------------------------------
	//	Window Callback Functions
	//------------------------------------------------------------------------

	std::vector<WindowListener*> listeners;

	void Window::ErrorCallback(int error, const char* description) {
		Logger::Error("GLFW Error: (" + std::to_string(error) + "): " + description);
	}

	void Window::ResizeCallback(GLFWwindow* windowHandle, int width, int height) {
		size = { width, height };
		for (WindowListener* listener : listeners)
			listener->OnWindowResize(width, height);
	}
	void Window::MoveCallback(GLFWwindow* windowHandle, int xpos, int ypos) {
		//if (!window->fullscreen)
		position = { xpos, ypos };
		for (WindowListener* listener : listeners)
			listener->OnWindowMove(xpos, ypos);
	}

	void Window::FocusCallback(GLFWwindow* windowHandle, int focused) {
		// focused = true means window gained focus
		for (WindowListener* listener : listeners)
			listener->OnWindowFocus(focused);
	}

	//========================================================================
	//	Window Listener Class
	//========================================================================

	WindowListener::WindowListener() {
		listeners.push_back(this);
	}

	WindowListener::~WindowListener() {
		auto iter = find(listeners.begin(), listeners.end(), this);
		listeners.erase(iter);
	}
}