#include "zore/graphics/vulkan/render_surface.hpp"
#include "zore/devices/window.hpp"
#include "zore/debug.hpp"
#include <GLFW/glfw3.h>

namespace zore {

	void RenderSurface::Init(VkInstance instance) {
		ENSURE(glfwCreateWindowSurface(instance, Window::GetWindowHandle(), nullptr, &m_surface) == VK_SUCCESS, "Failed to create surface");
	}

	void RenderSurface::Cleanup(VkInstance instance) {
		if (m_surface)
			vkDestroySurfaceKHR(instance, m_surface, nullptr);
		m_surface = nullptr;
	}
}