#include "zore/devices/Window.hpp"
#include "zore/devices/Keyboard.hpp"
#include "zore/devices/Mouse.hpp"
#include "zore/graphics/RenderEngine.hpp"
#include "zore/debug/Debug.hpp"

#include <glfw/glfw3.h>

namespace zore {

	//========================================================================
	//	Window Class
	//========================================================================

	static glm::ivec2 size = { 0, 0 };
	static glm::ivec2 position = { 0, 0 };
	static GLFWwindow* windowHandle = nullptr;
	static bool fullscreen = false;
	static bool cursorHidden = false;

	void Window::Init(int width, int height) {
		ENSURE(!windowHandle, "The function Window::Init() should not be invoked by the client application");
		// Initialize GLFW
		ENSURE(glfwInit(), "Failed to initialize GLFW");
		glfwSetErrorCallback(ErrorCallback);
		glfwWindowHint(GLFW_CENTER_CURSOR, true);

		// Set OpenGL Hints
		glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);

		//glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, true); // Set Transparent Framebuffer
		//glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
		Logger::Info("Window Initialization Complete");

		// Create GLFW window
		if (width * height == 0) {
			glm::ivec2 nativeRes = Window::GetNativeResolution();
			width = static_cast<int>(nativeRes.x * 0.75f);
			height = static_cast<int>(nativeRes.y * 0.75f);
		}
		size = { width, height };
		windowHandle = glfwCreateWindow(size.x, size.y, "Zore Engine Window", nullptr, nullptr);
		ENSURE(windowHandle, "Failed to create GLFW window");
		glfwMakeContextCurrent(windowHandle);
		Centre();

		// Enable Various features
		if (glfwRawMouseMotionSupported())
			glfwSetInputMode(windowHandle, GLFW_RAW_MOUSE_MOTION, true);
		glfwSetInputMode(windowHandle, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

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
		glfwSetMouseButtonCallback(windowHandle, Mouse::ButtonCallback);

		Logger::Info("Window Creation Complete");
		RenderEngine::Init();
		Logger::Info("Render Engine Initialization Complete");	
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

	void Window::SetMaximized(bool value) {
		value ? glfwMaximizeWindow(windowHandle) : glfwRestoreWindow(windowHandle);
	}

	void Window::SetBorderless(bool value) {
		glfwSetWindowAttrib(windowHandle, GLFW_DECORATED, !value);
	}

	void Window::SetFullscreen(bool value) {
		GLFWmonitor* monitor = value ? glfwGetPrimaryMonitor() : nullptr;
		glfwSetWindowMonitor(windowHandle, monitor, position.x, position.y, size.x, size.y, GLFW_DONT_CARE);
		if (!value)
			Centre();
		fullscreen = value;
	}

	void Window::ToggleFullscreen() {
		SetFullscreen(!fullscreen);
	}

	void Window::BringToFront() {
		glfwFocusWindow(windowHandle);
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

	void Window::SetSize(int width, int height) {
		size = { width, height };
		glfwSetWindowSize(windowHandle, width, height);
		ResizeCallback(windowHandle, width, height);
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

	bool Window::GetMaximized() {
		return glfwGetWindowAttrib(windowHandle, GLFW_MAXIMIZED);
	}

	float Window::GetAspectRatio() {
		return (static_cast<float>(size.x) / static_cast<float>(size.y));
	}

	const glm::ivec2& Window::GetSize() {
		return size;
	}

	const glm::ivec2& Window::GetPosition() {
		return position;
	}

	glm::ivec2 Window::GetNativeResolution(int monitorIndex) {
		int count;
		GLFWmonitor** monitors = glfwGetMonitors(&count);
		ENSURE(monitorIndex >= 0 && monitorIndex < count, "MonitorIndex specified out of range");

		if (!monitors[monitorIndex])
			return { 0, 0 };
		const GLFWvidmode* videoMode = glfwGetVideoMode(monitors[monitorIndex]);
		return { videoMode->width, videoMode->height };
	}

	GLFWwindow* Window::GetWindowHandle() {
		return windowHandle;
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
			listener->OnWindowResize(width, height, GetAspectRatio());
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