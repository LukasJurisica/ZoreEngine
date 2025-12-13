#include "zore/graphics/devices/render_surface.hpp"
#include "zore/devices/window.hpp"
#include "zore/debug.hpp"
#include <GLFW/glfw3.h>

namespace zore {

    static VkSurfaceKHR s_surface = VK_NULL_HANDLE;

	void RenderSurface::Init(VkInstance instance) {
		ENSURE(glfwCreateWindowSurface(instance, Window::GetWindowHandle(), nullptr, &s_surface) == VK_SUCCESS, "Failed to create surface");
    }

	const VkSurfaceKHR& RenderSurface::Get() {
		ENSURE(s_surface != VK_NULL_HANDLE, "Surface not initialized");
		return s_surface;
	}

	void RenderSurface::Cleanup(VkInstance instance) {
		if (s_surface)
			vkDestroySurfaceKHR(instance, s_surface, nullptr);
		s_surface = VK_NULL_HANDLE;
	}
}