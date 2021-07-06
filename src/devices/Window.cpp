#include "devices/Window.hpp"
#include "graphics/RenderEngine.hpp"
#include "debug/Logger.hpp"
#include "debug/debug.hpp"
#include <glfw/glfw3.h>

namespace zore {

	//========================================================================
	//	Window Class
	//========================================================================

	std::vector<Window*> windows;

	Window::Window(const std::string& name, int width, int height) : name(name), size(width, height), mouse(name), keyboard(name) {
		// Ensure the provided window name is valid
		ENSURE(name != "", "Invalid window name");
		for (auto iter : windows)
			ENSURE(iter->name != name, "A Window with the name: \"" + name + "\" already exists");
		windows.push_back(this);

		// Register waiting window listeners
		WindowListener::ClaimWaiters(this, name);

		GLFWmonitor* monitor = glfwGetPrimaryMonitor();
		const GLFWvidmode* videoMode = glfwGetVideoMode(monitor);

		// Create GLFW window
		windowHandle = glfwCreateWindow(size.x, size.y, "Zore Engine Window", nullptr, nullptr);
		ENSURE(windowHandle, "Failed to create GLFW window");
		glfwSetWindowUserPointer(windowHandle, this);
		glfwMakeContextCurrent(windowHandle);

		// Initiate RenderAPI (If it has not already been done so)
		RenderEngine::Init();

		// Enable Various features
		if (glfwRawMouseMotionSupported())
			glfwSetInputMode(windowHandle, GLFW_RAW_MOUSE_MOTION, true);
		glfwSetInputMode(windowHandle, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

		// Centre the window
		position.x = (videoMode->width - size.x) / 2;
		position.y = (videoMode->height - size.y) / 2;
		glfwSetWindowPos(windowHandle, position.x, position.y);

		double xpos, ypos;
		glfwGetCursorPos(windowHandle, &xpos, &ypos);
		mouse.ClearState(static_cast<float>(xpos), static_cast<float>(ypos));

		// Set GLFW event callbacks
		glfwSetWindowPosCallback(windowHandle, MoveCallback);
		glfwSetWindowSizeCallback(windowHandle, ResizeCallback);
		glfwSetWindowFocusCallback(windowHandle, FocusCallback);
		glfwSetKeyCallback(windowHandle, Keyboard::KeyCallback);
		glfwSetCharCallback(windowHandle, Keyboard::CharCallback);
		glfwSetScrollCallback(windowHandle, Mouse::ScrollCallback);
		glfwSetCursorPosCallback(windowHandle, Mouse::MoveCallback);
		glfwSetMouseButtonCallback(windowHandle, Mouse::ActionCallback);
	}

	Window::~Window() {
		glfwDestroyWindow(windowHandle);
		WindowListener::ReleaseListeners(listeners);

		auto iter = std::find(windows.begin(), windows.end(), this);
		if (iter != windows.end())
			windows.erase(iter);
	}

	void Window::Init() {
		ENSURE(glfwInit(), "Failed to initialize GLFW");
		glfwSetErrorCallback(ErrorCallback);
		glfwWindowHint(GLFW_CENTER_CURSOR, true);

		switch (RenderEngine::GetApi()) {
		case API::OpenGL:
			glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API); // Initialize OpenGL
			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
			break;
		case API::Vulkan:
			glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API); // Initialize Vulkan
			break;
		default:
			throw ZORE_EXCEPTION("No valid render api selected");
		}
		//glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, true); // Set Transparent Framebuffer
		//glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	}

	Window* Window::Get(const std::string& name) {
		if (name == "" && windows.size() > 0)
			return windows[0];
		for (Window* window : windows) {
			if (window->name == name)
				return window;
		}
		return nullptr;
	}

	void Window::Cleanup() {
		glfwTerminate();
	}

	void Window::RegisterListener(WindowListener* listener) {
		listeners.push_back(listener);
	}

	void Window::UnregisterListener(WindowListener* listener) {
		auto iter = std::find(listeners.begin(), listeners.end(), listener);
		if (iter != listeners.end())
			listeners.erase(iter);
	}

	void Window::Bind() {
		glfwMakeContextCurrent(windowHandle);
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
	}

	void Window::SetBorderless(bool value) {
		glfwSetWindowAttrib(windowHandle, GLFW_DECORATED, !value);
	}

	void Window::SetSize(int width, int height) {
		size = { width, height };
		glfwSetWindowSize(windowHandle, width, height);
	}

	void Window::HideCursor(bool value) {
		glfwSetInputMode(windowHandle, GLFW_CURSOR, value ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
	}

	float Window::GetAspectRatio() {
		return (static_cast<float>(size.x) / static_cast<float>(size.y));
	}

	glm::ivec2& Window::GetSize() {
		return size;
	}

	Mouse* Window::GetMouse() {
		return &mouse;
	}

	Keyboard* Window::GetKeyboard() {
		return &keyboard;
	}

	//------------------------------------------------------------------------
	//	Window Callback Functions
	//------------------------------------------------------------------------

	void Window::ErrorCallback(int error, const char* description) {
		Logger::Error("GLFW Error: (" + std::to_string(error) + "): " + description);
	}

	void Window::ResizeCallback(GLFWwindow* windowHandle, int width, int height) {
		Window* window = reinterpret_cast<Window*>(glfwGetWindowUserPointer(windowHandle));
		window->size = { width, height };
		for (WindowListener* listener : window->listeners)
			listener->OnWindowResize(width, height);
	}
	void Window::MoveCallback(GLFWwindow* windowHandle, int xpos, int ypos) {
		Window* window = reinterpret_cast<Window*>(glfwGetWindowUserPointer(windowHandle));
		//if (!window->fullscreen)
		//	window->position = { xpos, ypos };
		for (WindowListener* listener : window->listeners)
			listener->OnWindowMove(xpos, ypos);
	}

	void Window::FocusCallback(GLFWwindow* windowHandle, int focused) {
		Window* window = reinterpret_cast<Window*>(glfwGetWindowUserPointer(windowHandle));
		// focused = true means window gained focus
		for (WindowListener* listener : window->listeners)
			listener->OnWindowFocus(focused);
	}

	//========================================================================
	//	Window Listener Class
	//========================================================================

	std::vector<WindowListener*> waiters;

	WindowListener::WindowListener(const std::string& name) : name(name), window(Window::Get(name)) {
		if (window)
			window->RegisterListener(this);
		else
			waiters.push_back(this);
	}

	WindowListener::~WindowListener() {
		if (window)
			window->UnregisterListener(this);
		else
			RemoveWaiter(this);
	}

	void WindowListener::RemoveWaiter(WindowListener* listener) {
		for (int i = 0; i < waiters.size();) {
			if (waiters[i] == listener)
				waiters.erase(waiters.begin() + i);
			else
				i++;
		}
	}

	void WindowListener::ClaimWaiters(Window* window, const std::string& name) {
		for (int i = 0; i < waiters.size();) {
			if (waiters[i]->name == name || waiters[i]->name == "") {
				window->RegisterListener(waiters[i]);
				waiters[i]->window = window;
				waiters.erase(waiters.begin() + i);
			}
			else
				i++;
		}
	}

	void WindowListener::ReleaseListeners(std::vector<WindowListener*>& listeners) {
		for (WindowListener* listener : listeners) {
			listener->window = nullptr;
			waiters.push_back(listener);
		}
	}
}