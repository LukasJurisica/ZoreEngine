#include "zore/graphics/render_engine.hpp"
#include "zore/graphics/devices/render_device.hpp"
#include "zore/graphics/devices/render_surface.hpp"
#include "zore/platform.hpp"
#include <GLFW/glfw3.h>
#include "zore/debug.hpp"
#include <vulkan/vulkan.h>

namespace zore {

	static VkInstance s_instance = VK_NULL_HANDLE;
	static VkDebugUtilsMessengerEXT s_messenger = VK_NULL_HANDLE;
	static std::vector<const char*> s_instance_extensions;
	static std::vector<const char*> s_validation_layers;

	//========================================================================
	//	Vulkan Render Engine Utility
	//========================================================================

	static VKAPI_ATTR VkBool32 VKAPI_CALL MessageCallback(
		VkDebugUtilsMessageSeverityFlagBitsEXT severity,
		VkDebugUtilsMessageTypeFlagsEXT type,
		const VkDebugUtilsMessengerCallbackDataEXT* message_data,
		void* user_data) {

		Logger::Log("Vulkan Validation Layer: ", message_data->pMessage);
		return VK_FALSE;
	}

	void InitMessenger() {
		VkDebugUtilsMessengerCreateInfoEXT messenger_info{};
		messenger_info.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
		messenger_info.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
		messenger_info.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
		messenger_info.pfnUserCallback = MessageCallback;
		messenger_info.pUserData = nullptr;

		auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(s_instance, "vkCreateDebugUtilsMessengerEXT");
		ENSURE(func != nullptr, "Failed to get vkCreateDebugUtilsMessengerEXT function address");
		if (func)
			ENSURE(func(s_instance, &messenger_info, nullptr, &s_messenger) == VK_SUCCESS, "Failed to create debug messenger");
	}

	void InitInstanceExtensions() {
		uint32_t glfw_extension_count = 0;
		const char** glfw_extensions;
		glfw_extensions = glfwGetRequiredInstanceExtensions(&glfw_extension_count);
		for (uint32_t i = 0; i < glfw_extension_count; i++)
			s_instance_extensions.emplace_back(glfw_extensions[i]);
		if (IS_PLATFORM_MACOS) {
			s_instance_extensions.emplace_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);
            s_instance_extensions.emplace_back(VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME);
        }
		if (IS_DEBUG)
			s_instance_extensions.emplace_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);

		uint32_t extension_count = 0;
		vkEnumerateInstanceExtensionProperties(nullptr, &extension_count, nullptr);
		std::vector<VkExtensionProperties> available_extensions(extension_count);
		vkEnumerateInstanceExtensionProperties(nullptr, &extension_count, available_extensions.data());

		auto SupportsExtension = [&](const char* extension) {
			for (const VkExtensionProperties& properties : available_extensions)
				if (strcmp(extension, properties.extensionName) == 0)
					return true;
			return false;
			};

		for (const char* extension : s_instance_extensions)
			ENSURE(SupportsExtension(extension), std::format("Missing Required Instance Extension: {}", extension));
	}

	void InitValidationLayers() {
		s_validation_layers = { "VK_LAYER_KHRONOS_validation" };
		uint32_t count = 0;
		vkEnumerateInstanceLayerProperties(&count, nullptr);
		std::vector<VkLayerProperties> available_layers(count);
		vkEnumerateInstanceLayerProperties(&count, available_layers.data());

		auto SupportsValidationLayer = [&](const char* layer) {
			for (const VkLayerProperties& layer_properties : available_layers)
				if (strcmp(layer, layer_properties.layerName) == 0)
					return true;
			return false;
			};

		for (const char* layer : s_validation_layers)
			ENSURE(SupportsValidationLayer(layer), std::format("Missing Required Validation Layer: {}", layer));
	}

	//========================================================================
	//	Vulkan Render Engine
	//========================================================================

	void RenderEngine::Init() {
		VkApplicationInfo application_info{};
		application_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		application_info.pApplicationName = "Zore Engine Application";
		application_info.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
		application_info.pEngineName = "Zore Engine";
		application_info.engineVersion = VK_MAKE_VERSION(1, 0, 0);
		application_info.apiVersion = VK_API_VERSION_1_0;

		InitInstanceExtensions();
		if (IS_DEBUG)
			InitValidationLayers();

		VkInstanceCreateInfo instance_info{};
		instance_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		instance_info.pApplicationInfo = &application_info;
		instance_info.enabledExtensionCount = static_cast<uint32_t>(s_instance_extensions.size());
		instance_info.ppEnabledExtensionNames = s_instance_extensions.data();
		instance_info.enabledLayerCount = static_cast<uint32_t>(s_validation_layers.size());
		instance_info.ppEnabledLayerNames = s_validation_layers.data();
		if (IS_PLATFORM_MACOS)
			instance_info.flags |= VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;

		ENSURE(vkCreateInstance(&instance_info, nullptr, &s_instance) == VK_SUCCESS, "Failed to create Vulkan Instance");
		Logger::Info("Vulkan Render Engine Initialization Complete.");

		RenderSurface::Init(s_instance);
		if (IS_DEBUG)
			InitMessenger();
		RenderDevice::Init(s_instance, RenderSurface::Get());
	}

	void RenderEngine::Cleanup() {
		RenderDevice::Cleanup();
		RenderSurface::Cleanup(s_instance);

		if (s_messenger != VK_NULL_HANDLE) {
			auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(s_instance, "vkDestroyDebugUtilsMessengerEXT");
			if (func)
				func(s_instance, s_messenger, nullptr);
			s_messenger = VK_NULL_HANDLE;
		}

		if (s_instance != VK_NULL_HANDLE) {
			vkDestroyInstance(s_instance, nullptr);
			s_instance = nullptr;
		}
	}

	void RenderEngine::SetViewport(uint32_t width, uint32_t height, uint32_t x, uint32_t y) {
		//RenderDevice::SetViewport(width, height, x, y);
	}
}