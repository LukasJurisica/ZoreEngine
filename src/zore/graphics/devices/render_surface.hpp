#pragma once
#include <vulkan/vulkan.h>

namespace zore {

	class RenderSurface {
	public:
		static void Init(VkInstance instance);
        static const VkSurfaceKHR& Get();
        static void Cleanup(VkInstance instance);
	};
}