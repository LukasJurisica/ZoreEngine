#include "zore/devices/Window.hpp"
#include "zore/devices/Keyboard.hpp"
#include "zore/devices/Mouse.hpp"
#include "zore/graphics/RenderEngine.hpp"
#include "zore/debug/Debug.hpp"
#include "zore/debug/Profiler.hpp"

#include <glfw/glfw3.h>

namespace zore {

	//========================================================================
	//	Window Class
	//========================================================================

	static glm::ivec2 s_size = { 0, 0 };
	static glm::ivec2 s_position = { 0, 0 };
	static GLFWwindow* s_window_handle = nullptr;
	static bool s_fullscreen = false;
	static bool s_cursor_hidden = false;

	static bool s_transparent = false;

	void Window::Init(int width, int height) {
		ENSURE(!s_window_handle, "The function Window::Init() should not be invoked by the client application.");
		// Initialize GLFW
		ENSURE(glfwInit(), "Failed to initialize GLFW.");
		glfwSetErrorCallback(ErrorCallback);
		Logger::Info("Window Initialization Complete.");

		if (width * height == 0) {
			glm::ivec2 native_resolution = Window::GetNativeResolution();
			width = static_cast<int>(native_resolution.x * 0.75f);
			height = static_cast<int>(native_resolution.y * 0.75f);
		}
		s_size = { width, height };

		Create();
		Logger::Info("Window Creation Complete.");
		RenderEngine::Init();
		Logger::Info("Render Engine Initialization Complete.");	
	}

	void Window::Create() {
		glfwWindowHint(GLFW_CENTER_CURSOR, true);

		// Set OpenGL Hints
		glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);

		glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, s_transparent); // Set Transparent Framebuffer

		// Create GLFW window
		s_window_handle = glfwCreateWindow(s_size.x, s_size.y, "Zore Engine Window", nullptr, nullptr);
		ENSURE(s_window_handle, "Failed to create GLFW window.");
		glfwMakeContextCurrent(s_window_handle);
		Centre();

		// Enable Various features
		if (glfwRawMouseMotionSupported())
			glfwSetInputMode(s_window_handle, GLFW_RAW_MOUSE_MOTION, true);
		glfwSetInputMode(s_window_handle, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

		double xpos, ypos;
		glfwGetCursorPos(s_window_handle, &xpos, &ypos);
		Mouse::ClearState(static_cast<float>(xpos), static_cast<float>(ypos));

		// Set GLFW event callbacks
		glfwSetWindowPosCallback(s_window_handle, MoveCallback);
		glfwSetWindowSizeCallback(s_window_handle, ResizeCallback);
		glfwSetWindowFocusCallback(s_window_handle, FocusCallback);
		glfwSetKeyCallback(s_window_handle, Keyboard::KeyCallback);
		glfwSetCharCallback(s_window_handle, Keyboard::CharCallback);
		glfwSetScrollCallback(s_window_handle, Mouse::ScrollCallback);
		glfwSetCursorPosCallback(s_window_handle, Mouse::MoveCallback);
		glfwSetMouseButtonCallback(s_window_handle, Mouse::ButtonCallback);
	}

	void Window::Cleanup() {
		glfwDestroyWindow(s_window_handle);
		glfwTerminate();
		Logger::Info("Window Cleanup Complete.");
	}

	void Window::Update() {
		glfwPollEvents();
		glfwSwapBuffers(s_window_handle);
		FrameMark;
	}

	bool Window::ShouldClose() {
		return glfwWindowShouldClose(s_window_handle);
	}

	void Window::SetTitle(const char* title) {
		glfwSetWindowTitle(s_window_handle, title);
	}

	void Window::PreSetTransparent(bool value) {
		s_transparent = value;
	}

	void Window::SetResizable(bool value) {
		glfwSetWindowAttrib(s_window_handle, GLFW_RESIZABLE, value);
	}

	void Window::SetMaximized(bool value) {
		value ? glfwMaximizeWindow(s_window_handle) : glfwRestoreWindow(s_window_handle);
	}

	void Window::SetBorderless(bool value) {
		glfwSetWindowAttrib(s_window_handle, GLFW_DECORATED, !value);
	}

	void Window::SetFullscreen(bool value) {
		GLFWmonitor* monitor = value ? glfwGetPrimaryMonitor() : nullptr;
		glfwSetWindowMonitor(s_window_handle, monitor, s_position.x, s_position.y, s_size.x, s_size.y, GLFW_DONT_CARE);
		if (!value)
			Centre();
		s_fullscreen = value;
	}

	void Window::ToggleFullscreen() {
		SetFullscreen(!s_fullscreen);
	}

	void Window::BringToFront() {
		glfwFocusWindow(s_window_handle);
	}

	void Window::HideCursor(bool value) {
		s_cursor_hidden = value;
		glfwSetInputMode(s_window_handle, GLFW_CURSOR, value ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);

		if (!value) {
			glfwSetCursorPos(s_window_handle, s_size.x / 2, s_size.y / 2);
			Mouse::ClearState(static_cast<float>(s_size.x / 2), static_cast<float>(s_size.y / 2));
		}
	}

	void Window::ToggleCursor() {
		HideCursor(!s_cursor_hidden);
	}

	void Window::SetSize(int width, int height) {
		s_size = { width, height };
		glfwSetWindowSize(s_window_handle, width, height);
		ResizeCallback(s_window_handle, width, height);
	}

	void Window::SetPosition(int x, int y) {
		s_position = { x, y };
		glfwSetWindowPos(s_window_handle, s_position.x, s_position.y);
	}

	void Window::Centre() {
		GLFWmonitor* monitor = glfwGetPrimaryMonitor();
		if (monitor) {
			const GLFWvidmode* video_mode = glfwGetVideoMode(monitor);
			s_position.x = (video_mode->width - s_size.x) / 2;
			s_position.y = (video_mode->height - s_size.y) / 2;
			glfwSetWindowPos(s_window_handle, s_position.x, s_position.y);
		}
	}

	bool Window::GetMaximized() {
		return glfwGetWindowAttrib(s_window_handle, GLFW_MAXIMIZED);
	}

	float Window::GetAspectRatio() {
		return (static_cast<float>(s_size.x) / static_cast<float>(s_size.y));
	}

	const glm::ivec2& Window::GetSize() {
		return s_size;
	}

	const glm::ivec2& Window::GetPosition() {
		return s_position;
	}

	glm::ivec2 Window::GetNativeResolution(int monitor_index) {
		int count;
		GLFWmonitor** monitors = glfwGetMonitors(&count);
		ENSURE(monitor_index >= 0 && monitor_index < count, "Monitor Index specified out of range");

		if (!monitors[monitor_index])
			return { 0, 0 };
		const GLFWvidmode* video_mode = glfwGetVideoMode(monitors[monitor_index]);
		return { video_mode->width, video_mode->height };
	}

	GLFWwindow* Window::GetWindowHandle() {
		return s_window_handle;
	}

	//------------------------------------------------------------------------
	//	Window Callback Functions
	//------------------------------------------------------------------------

	std::vector<Window::Listener*> listeners;

	void Window::ErrorCallback(int error, const char* description) {
		Logger::Error("GLFW Error: (" + std::to_string(error) + "): " + description);
	}

	void Window::ResizeCallback(GLFWwindow* window_handle, int width, int height) {
		s_size = { width, height };
		for (Listener* listener : listeners)
			listener->OnWindowResize(width, height, GetAspectRatio());
	}
	void Window::MoveCallback(GLFWwindow* window_handle, int xpos, int ypos) {
		//if (!window->fullscreen)
		s_position = { xpos, ypos };
		for (Listener* listener : listeners)
			listener->OnWindowMove(xpos, ypos);
	}

	void Window::FocusCallback(GLFWwindow* window_handle, int focused) {
		// focused = true means window gained focus
		for (Listener* listener : listeners)
			listener->OnWindowFocus(focused);
	}

	//========================================================================
	//	Window Listener Class
	//========================================================================

	Window::Listener::Listener() {
		listeners.push_back(this);
	}

	Window::Listener::~Listener() {
		auto iter = find(listeners.begin(), listeners.end(), this);
		listeners.erase(iter);
	}
}