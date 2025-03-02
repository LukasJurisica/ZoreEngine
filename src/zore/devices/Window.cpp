#include "zore/Devices.hpp"
#include "zore/graphics/RenderEngine.hpp"
#include "zore/events/WindowEvents.hpp"
#include "zore/events/Manager.hpp"
#include "zore/utils/Time.hpp"
#include "zore/debug/Profiler.hpp"
#include "zore/Debug.hpp"

#include <GLFW/glfw3.h>

namespace zore {

	//========================================================================
	//	Window Class
	//========================================================================

	static glm::ivec2 s_size = { 0, 0 };
	static glm::ivec2 s_min_size = { GLFW_DONT_CARE, GLFW_DONT_CARE };
	static glm::ivec2 s_max_size = { GLFW_DONT_CARE, GLFW_DONT_CARE };
	static glm::ivec2 s_position = { 0, 0 };
	static GLFWwindow* s_window_handle = nullptr;
	static bool s_fullscreen = false;
	static bool s_cursor_hidden = false;
	static bool s_transparent = false;

	void Window::Init(bool transparent) {
		ENSURE(!s_window_handle, "The function Window::Init() should not be invoked by the client application.");
		// Initialize GLFW
		ENSURE(glfwInit(), "Failed to initialize GLFW.");
		glfwSetErrorCallback(ErrorCallback);
		Logger::Info("Window Initialization Complete.");

		glm::ivec2 native_resolution = Window::GetNativeResolution();
		s_size = { static_cast<int>(native_resolution.x * 0.75f), static_cast<int>(native_resolution.y * 0.75f) };
		s_transparent = transparent;

		Create();
		Logger::Info("Window Creation Complete.");
		RenderEngine::Init();
		Logger::Info("Render Engine Initialization Complete.");
		Update();
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
		Mouse::SetPosition(static_cast<float>(xpos), static_cast<float>(ypos));

		// Set GLFW event callbacks
		glfwSetWindowPosCallback(s_window_handle, MoveCallback);
		glfwSetFramebufferSizeCallback(s_window_handle, ResizeCallback);
		glfwSetWindowFocusCallback(s_window_handle, FocusCallback);
		glfwSetDropCallback(s_window_handle, PathDropCallback);
		glfwSetWindowMaximizeCallback(s_window_handle, MaximizedCallback);
		glfwSetWindowIconifyCallback(s_window_handle, MinimizedCallback);
		glfwSetKeyCallback(s_window_handle, Keyboard::KeyCallback);
		glfwSetCharCallback(s_window_handle, Keyboard::CharCallback);
		glfwSetScrollCallback(s_window_handle, Mouse::ScrollCallback);
		glfwSetCursorPosCallback(s_window_handle, Mouse::MoveCallback);
		glfwSetMouseButtonCallback(s_window_handle, Mouse::ButtonCallback);
		glfwSetCursorEnterCallback(s_window_handle, Mouse::EnterCallback);
	}

	void Window::Cleanup() {
		glfwDestroyWindow(s_window_handle);
		glfwTerminate();
		Logger::Info("Window Cleanup Complete.");
	}

	void Window::Update() {
		glfwSwapBuffers(s_window_handle);
		Time::NewFrame();
		Keyboard::ClearState(false);
		Mouse::ClearState(false);
		glfwPollEvents();
		FrameMark;
	}

	void Window::Close(bool value) {
		glfwSetWindowShouldClose(s_window_handle, value);
	}

	void Window::SetTitle(const char* title) {
		glfwSetWindowTitle(s_window_handle, title);
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
		if (!value)
			Mouse::SetPosition(static_cast<float>(s_size.x / 2), static_cast<float>(s_size.y / 2));
	}

	void Window::ToggleCursor() {
		HideCursor(!s_cursor_hidden);
	}

	void Window::SetSize(int width, int height) {
		s_size = { width, height };
		glfwSetWindowSize(s_window_handle, width, height);
		ResizeCallback(s_window_handle, width, height);
	}

	void Window::SetMinSize(int width, int height) {
		s_min_size = { width < 0 ? GLFW_DONT_CARE : width, height < 0 ? GLFW_DONT_CARE : height };
		glfwSetWindowSizeLimits(s_window_handle, s_min_size.x, s_min_size.y, s_max_size.x, s_max_size.y);
	}

	void Window::SetMaxSize(int width, int height) {
		s_max_size = { width < 0 ? GLFW_DONT_CARE : width, height < 0 ? GLFW_DONT_CARE : height };
		glfwSetWindowSizeLimits(s_window_handle, s_min_size.x, s_min_size.y, s_max_size.x, s_max_size.y);
	}

	void Window::ForceAspectRatio(int width, int height) {
		if (width < 0 or height < 0) {
			width = GLFW_DONT_CARE;
			height = GLFW_DONT_CARE;
		}
		glfwSetWindowAspectRatio(s_window_handle, width, height);
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

	bool Window::ShouldClose() {
		return glfwWindowShouldClose(s_window_handle);
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

	void Window::ErrorCallback(int error, const char* description) {
		Logger::Error("GLFW Error: (" + std::to_string(error) + "): " + description);
	}

	void Window::ResizeCallback(GLFWwindow* window_handle, int width, int height) {
		s_size = { width, height };
		RenderEngine::SetViewport(s_size.x, s_size.y);
		Event::Manager::Dispatch(WindowResizedEvent(width, height));
	}

	void Window::MoveCallback(GLFWwindow* window_handle, int xpos, int ypos) {
		s_position = { xpos, ypos };
		Event::Manager::Dispatch(WindowMovedEvent(xpos, ypos));
	}

	void Window::FocusCallback(GLFWwindow* window_handle, int focused) {
		Event::Manager::Dispatch(WindowFocusedEvent(focused));
	}

	void Window::PathDropCallback(GLFWwindow* window, int count, const char** paths) {
		Event::Manager::Dispatch(WindowFileDropEvent(count, paths));
	}

	void Window::MaximizedCallback(GLFWwindow* window_handle, int maximized) {
		Event::Manager::Dispatch(WindowMaximizedEvent(maximized));
	}

	void Window::MinimizedCallback(GLFWwindow* window_handle, int minimized) {
		Event::Manager::Dispatch(WindowMinimizedEvent(minimized));
	}
}