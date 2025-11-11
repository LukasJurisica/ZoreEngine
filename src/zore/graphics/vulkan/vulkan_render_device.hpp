#pragma once

#include "zore/graphics/vulkan/vulkan_render_surface.hpp"
#include <vulkan/vulkan.h>
#include <vector>

namespace zore {

	enum class QueueFamily { GRAPHICS, PRESENT, COMPUTE, TRANSFER };

	//========================================================================
	//	Vulkan Render Device
	//========================================================================

	class RenderDevice {
	public:
		static void Init(VkInstance instance, RenderSurface& surface);
		static void Cleanup();

	private:
		static void InitPhysicalDevice(VkInstance instance, RenderSurface& surface);
		static bool InitQueueFamilies(VkPhysicalDevice device, RenderSurface& surface);
		static void InitLogicalDevice();
		static bool InitSwapChain(VkPhysicalDevice device, RenderSurface& surface);
		static void CreateSwapChain(VkPhysicalDevice device, RenderSurface& surface);
		static void CreateFramebuffer();
		static bool CheckDeviceExtensions(VkPhysicalDevice device);
		static void CreateQueues(const std::vector<QueueFamily>& families);

	public:
		static const VkDevice& Get();
		static const VkExtent2D& GetExtent();
		static const VkSurfaceFormatKHR& GetSurfaceFormat();
		static const VkPresentModeKHR& GetPresentMode();
	};
}