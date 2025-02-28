#include "zore/devices/Clipboard.hpp"

#include <GLFW/glfw3.h>

namespace zore {

	std::string Clipboard::Get() {
		return glfwGetClipboardString(nullptr);
	}

	void Clipboard::Set(const std::string& s) {
		glfwSetClipboardString(nullptr, s.c_str());
	}
}