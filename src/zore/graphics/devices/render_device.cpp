#include "zore/graphics/devices/render_device.hpp"
#include "zore/graphics/graphics_pipeline.hpp"
#include "zore/devices/window.hpp"
#include "zore/math/math.hpp"
#include "zore/platform.hpp"
#include "zore/debug.hpp"
#include <unordered_map>
#include <vulkan/vulkan_beta.h>
#include <GLFW/glfw3.h>

namespace zore {

	static VkPhysicalDevice s_physical_device = nullptr;
	static VkDevice s_device = nullptr;

	static std::vector<const char*> s_device_extensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };
	static std::vector<QueueFamily> required_queue_families = { QueueFamily::GRAPHICS, QueueFamily::PRESENT };
	static std::unordered_map<QueueFamily, uint32_t> s_queue_families;
	static std::vector<uint32_t> s_unique_queue_families;
	static std::unordered_map<QueueFamily, VkQueue> s_queues;

	static VkSwapchainKHR s_swap_chain = nullptr;
	static std::vector<VkImage> s_swap_chain_images;
	static std::vector<VkImageView> s_swap_chain_image_views;
    static std::vector<VkFramebuffer> s_swap_chain_framebuffers;
	static VkExtent2D s_extent{ 0, 0 };
	static VkSurfaceFormatKHR s_surface_format{ VK_FORMAT_UNDEFINED, VK_COLOR_SPACE_MAX_ENUM_KHR };
	static VkPresentModeKHR s_present_mode = VK_PRESENT_MODE_MAX_ENUM_KHR;

	//========================================================================
	//	Vulkan Render Device
	//========================================================================

	void RenderDevice::Init(VkInstance instance, VkSurfaceKHR surface) {
		InitPhysicalDevice(instance, surface);
		InitQueueFamilies(s_physical_device, surface);
		InitLogicalDevice();
		CreateQueues(required_queue_families);
		InitSwapChain(s_physical_device, surface);
		CreateSwapChain(s_physical_device, surface);
		CreateFramebuffer();
	}

	void RenderDevice::Cleanup() {
		for (VkImageView& view : s_swap_chain_image_views)
			vkDestroyImageView(s_device, view, nullptr);
		s_swap_chain_image_views.clear();
		if (s_swap_chain)
			vkDestroySwapchainKHR(s_device, s_swap_chain, nullptr);
		s_swap_chain = nullptr;
		if (s_device)
			vkDestroyDevice(s_device, nullptr);
		s_device = nullptr;
		s_physical_device = nullptr;
	}

	void RenderDevice::InitPhysicalDevice(VkInstance instance, VkSurfaceKHR surface) {
		uint32_t device_count = 0;
		vkEnumeratePhysicalDevices(instance, &device_count, nullptr);
		ENSURE(device_count > 0, "No GPUs with Vulkan support found");
		std::vector<VkPhysicalDevice> devices(device_count);
		vkEnumeratePhysicalDevices(instance, &device_count, devices.data());

        if (IS_PLATFORM_MACOS)
            s_device_extensions.emplace_back(VK_KHR_PORTABILITY_SUBSET_EXTENSION_NAME);

		uint32_t best_score = 0;
		for (const VkPhysicalDevice& device : devices) {
			VkPhysicalDeviceProperties properties;
			vkGetPhysicalDeviceProperties(device, &properties);
			VkPhysicalDeviceFeatures features;
			vkGetPhysicalDeviceFeatures(device, &features);

			uint32_t current_score = 0;
			if (properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
				current_score += 1000;
			current_score += properties.limits.maxImageDimension2D;

			// Check if all extensions are supported by the device
			if (!CheckDeviceExtensions(device))
				continue;
			// Ensure that the device supports the required queue families
			if (!InitQueueFamilies(device, surface))
				continue;
			// Ensure that the device can provide an adequate swap chain
			if (!InitSwapChain(device, surface))
				continue;

			if (current_score > best_score) {
				best_score = current_score;
				s_physical_device = device;
			}
		}
		ENSURE(s_physical_device != nullptr, "Failed to find a suitable GPU");
		Logger::Info("Physical Render Device Selection Complete");
	}

	void RenderDevice::InitLogicalDevice() {
		float priority = 1.f;
		std::vector<VkDeviceQueueCreateInfo> queue_infos;

		queue_infos.resize(s_unique_queue_families.size());
		for (int i = 0; i < s_unique_queue_families.size(); i++) {
			queue_infos[i].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			queue_infos[i].queueFamilyIndex = s_unique_queue_families[i];
			queue_infos[i].queueCount = 1;
			queue_infos[i].pQueuePriorities = &priority;
			queue_infos[i].pNext = nullptr;
			queue_infos[i].flags = 0;
		}

		VkPhysicalDeviceFeatures device_features{};
		VkDeviceCreateInfo device_info{};
		device_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		device_info.queueCreateInfoCount = static_cast<uint32_t>(queue_infos.size());
		device_info.pQueueCreateInfos = queue_infos.data();
		device_info.pEnabledFeatures = &device_features;
		device_info.enabledExtensionCount = static_cast<uint32_t>(s_device_extensions.size());
		device_info.ppEnabledExtensionNames = s_device_extensions.data();

		// TODO ADD VALIDATION LAYERS HERE
		//device_info.enabledLayerCount = s_validation_layers.size();
		//device_info.ppEnabledLayerNames = s_validation_layers.data();
		ENSURE(vkCreateDevice(s_physical_device, &device_info, nullptr, &s_device) == VK_SUCCESS, "Failed to create logical device");
		Logger::Info("Logical Render Device Creation Complete");
	}

	bool RenderDevice::InitQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR surface) {
		s_queue_families.clear();
		uint32_t queue_family_count = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(device, &queue_family_count, nullptr);
		std::vector<VkQueueFamilyProperties> queue_families(queue_family_count);
		vkGetPhysicalDeviceQueueFamilyProperties(device, &queue_family_count, queue_families.data());

		for (uint32_t i = 0; i < queue_family_count; i++) {
			if (queue_families[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
				s_queue_families[QueueFamily::GRAPHICS] = i;
			VkBool32 present_support = false;
			vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &present_support);
			if (present_support)
				s_queue_families[QueueFamily::PRESENT] = i;
			if (queue_families[i].queueFlags & VK_QUEUE_COMPUTE_BIT)
				s_queue_families[QueueFamily::COMPUTE] = i;
			if (queue_families[i].queueFlags & VK_QUEUE_TRANSFER_BIT)
				s_queue_families[QueueFamily::TRANSFER] = i;
		}

		s_unique_queue_families.clear();
		for (const auto& pair : s_queue_families)
			if (std::find(s_unique_queue_families.begin(), s_unique_queue_families.end(), pair.second) == s_unique_queue_families.end())
				s_unique_queue_families.push_back(pair.second);

		for (QueueFamily family : required_queue_families)
			if (s_queue_families.count(family) == 0)
				return false;
		return true;
	}

	bool RenderDevice::InitSwapChain(VkPhysicalDevice device, VkSurfaceKHR surface) {
		// Select Optimal Surface Format
		uint32_t format_count;
		vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &format_count, nullptr);
		if (format_count == 0)
			return false;
		std::vector<VkSurfaceFormatKHR> surface_formats;
		surface_formats.resize(format_count);
		vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &format_count, surface_formats.data());
		auto SelectOptimalFormat = [](const std::vector<VkSurfaceFormatKHR>& formats) {
			for (const auto& format : formats)
				if (format.format == VK_FORMAT_B8G8R8A8_SRGB && format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
					return format;
			return formats[0];
			};
		s_surface_format = SelectOptimalFormat(surface_formats);

		// Select Optimal Present Mode
		uint32_t present_mode_count;
		vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &present_mode_count, nullptr);
		if (present_mode_count == 0)
			return false;
		std::vector<VkPresentModeKHR> present_modes;
		present_modes.resize(present_mode_count);
		vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &present_mode_count, present_modes.data());
		if (std::find(present_modes.begin(), present_modes.end(), VK_PRESENT_MODE_MAILBOX_KHR) != present_modes.end())
			s_present_mode = VK_PRESENT_MODE_MAILBOX_KHR;
		else
			s_present_mode = VK_PRESENT_MODE_FIFO_KHR; // Maybe use this on mobile to save battery

		return true;
	}

	void RenderDevice::CreateSwapChain(VkPhysicalDevice device, VkSurfaceKHR surface) {
		VkSurfaceCapabilitiesKHR surface_capabilities{};
		vkGetPhysicalDeviceSurfaceCapabilitiesKHR(s_physical_device, surface, &surface_capabilities);
		s_extent = surface_capabilities.currentExtent;
		if (s_extent.width == std::numeric_limits<uint32_t>::max()) {
			int width, height;
			glfwGetFramebufferSize(Window::GetWindowHandle(), &width, &height);
			s_extent.width = zm::Clamp(static_cast<uint32_t>(width), surface_capabilities.minImageExtent.width, surface_capabilities.maxImageExtent.width);
			s_extent.height = zm::Clamp(static_cast<uint32_t>(height), surface_capabilities.minImageExtent.height, surface_capabilities.maxImageExtent.height);
		}
		uint32_t image_count = surface_capabilities.minImageCount + 1;
		if (surface_capabilities.maxImageCount > 0 && image_count > surface_capabilities.maxImageCount)
			image_count = surface_capabilities.maxImageCount;

		VkSwapchainCreateInfoKHR swapchain_info{};
		swapchain_info.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		swapchain_info.surface = surface;
		swapchain_info.minImageCount = image_count;
		swapchain_info.imageFormat = s_surface_format.format;
		swapchain_info.imageColorSpace = s_surface_format.colorSpace;
		swapchain_info.imageExtent = s_extent;
		swapchain_info.imageArrayLayers = 1;
		swapchain_info.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
		swapchain_info.preTransform = surface_capabilities.currentTransform;
		swapchain_info.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
		swapchain_info.presentMode = s_present_mode;
		swapchain_info.clipped = VK_TRUE;
		swapchain_info.oldSwapchain = nullptr;

		uint32_t queue_family_indices[] = { s_queue_families[QueueFamily::GRAPHICS], s_queue_families[QueueFamily::PRESENT] };
		if (s_queue_families[QueueFamily::GRAPHICS] != s_queue_families[QueueFamily::PRESENT]) {
			swapchain_info.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
			swapchain_info.queueFamilyIndexCount = 2;
			swapchain_info.pQueueFamilyIndices = queue_family_indices;
		}
		else {
			swapchain_info.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
			swapchain_info.queueFamilyIndexCount = 0; // Optional
			swapchain_info.pQueueFamilyIndices = nullptr; // Optional
		}

		ENSURE(vkCreateSwapchainKHR(s_device, &swapchain_info, nullptr, &s_swap_chain) == VK_SUCCESS, "Failed to create swap chain");
		Logger::Info("Swap Chain Creation Complete");
	}

	void RenderDevice::CreateFramebuffer() {
		uint32_t image_count = 0;
		vkGetSwapchainImagesKHR(s_device, s_swap_chain, &image_count, nullptr);
		s_swap_chain_images.resize(image_count);
		vkGetSwapchainImagesKHR(s_device, s_swap_chain, &image_count, s_swap_chain_images.data());
		s_swap_chain_image_views.resize(image_count);
        s_swap_chain_framebuffers.resize(image_count);

		for (int i = 0; i < image_count; i++) {
			VkImageViewCreateInfo view_info{};
			view_info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
			view_info.image = s_swap_chain_images[i];
			view_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
			view_info.format = s_surface_format.format;
			view_info.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
			view_info.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
			view_info.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
			view_info.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
			view_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			view_info.subresourceRange.baseMipLevel = 0;
			view_info.subresourceRange.levelCount = 1;
			view_info.subresourceRange.baseArrayLayer = 0;
			view_info.subresourceRange.layerCount = 1;
			ENSURE(vkCreateImageView(s_device, &view_info, nullptr, &s_swap_chain_image_views[i]) == VK_SUCCESS, "Failed to create image view");
		
            VkRenderPass render_pass = RenderPass::Get(1)->Handle().as<VkRenderPass>();
            VkImageView attachments[] = { s_swap_chain_image_views[i] };
            VkFramebufferCreateInfo framebuffer_info{};
            framebuffer_info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
            framebuffer_info.renderPass = render_pass;
            framebuffer_info.attachmentCount = 1;
            framebuffer_info.pAttachments = attachments;
            framebuffer_info.width = s_extent.width;
            framebuffer_info.height = s_extent.height;
            framebuffer_info.layers = 1;
            ENSURE(vkCreateFramebuffer(s_device, &framebuffer_info, nullptr, &s_swap_chain_framebuffers[i]) == VK_SUCCESS, "Failed to create framebuffer");
        }
		Logger::Info("Frame Buffer Creation Complete");
	}

	bool RenderDevice::CheckDeviceExtensions(VkPhysicalDevice device) {
		uint32_t extension_count = 0;
		vkEnumerateDeviceExtensionProperties(device, nullptr, &extension_count, nullptr);
		std::vector<VkExtensionProperties> available_extensions(extension_count);
		vkEnumerateDeviceExtensionProperties(device, nullptr, &extension_count, available_extensions.data());

		auto SupportsExtension = [&](const char* extension) {
			for (const VkExtensionProperties& properties : available_extensions)
				if (strcmp(extension, properties.extensionName) == 0)
					return true;
			return false;
			};

		for (const char* extension : s_device_extensions)
			if (!SupportsExtension(extension))
				return false;
		return true;
	}

	void RenderDevice::CreateQueues(const std::vector<QueueFamily>& families) {
		for (QueueFamily family : families) {
			s_queues[family] = VkQueue();
			vkGetDeviceQueue(s_device, s_queue_families[family], 0, &s_queues[family]);
		}
	}

	const VkDevice& RenderDevice::Get() {
		return s_device;
	}

	const VkExtent2D& RenderDevice::GetExtent() {
		return s_extent;
	}

	const VkSurfaceFormatKHR& RenderDevice::GetSurfaceFormat() {
		return s_surface_format;
	}

	const VkPresentModeKHR& RenderDevice::GetPresentMode() {
		return s_present_mode;
	}
}