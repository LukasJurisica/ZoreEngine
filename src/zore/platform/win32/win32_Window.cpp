#include "zore/devices/Window.hpp"

#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <glfw/glfw3native.h>

namespace zore {

	void Window::GetNativeHandle(void* result) {
		*reinterpret_cast<HWND*>(result) = glfwGetWin32Window(Window::GetWindowHandle());
	}
}