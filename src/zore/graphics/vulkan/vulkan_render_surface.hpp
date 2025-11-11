#pragma once
#include <vulkan/vulkan.h>

namespace zore {

	class RenderSurface {
	public:
		RenderSurface() = default;
		~RenderSurface() = default;
		void Init(VkInstance instance);
		void Cleanup(VkInstance instance);

	public:
		VkSurfaceKHR m_surface = nullptr;
	};
}