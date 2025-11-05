#include "zore/graphics/render_engine.hpp"
#include "zore/graphics/vulkan/render_device.hpp"
#include "zore/graphics/vulkan/render_surface.hpp"
#include "zore/platform.hpp"
#include "zore/debug.hpp"
#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>

namespace zore {

	static VkInstance s_instance = nullptr;
	static VkDebugUtilsMessengerEXT s_messenger = nullptr;
	static std::vector<const char*> s_instance_extensions;
	static std::vector<const char*> s_validation_layers;

	RenderSurface surface;

	//========================================================================
	//	Render Engine Utility
	//========================================================================

	static VKAPI_ATTR VkBool32 VKAPI_CALL MessageCallback(
		VkDebugUtilsMessageSeverityFlagBitsEXT severity,
		VkDebugUtilsMessageTypeFlagsEXT type,
		const VkDebugUtilsMessengerCallbackDataEXT* message_data,
		void* user_data) {

		Logger::Log("Vulkan Validation Layer: ", message_data->pMessage);
		return VK_FALSE;
	}

	void InitMessenger(const VkAllocationCallbacks* allocator = nullptr) {
		VkDebugUtilsMessengerCreateInfoEXT messenger_info{};
		messenger_info.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
		messenger_info.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
		messenger_info.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
		messenger_info.pfnUserCallback = MessageCallback;
		messenger_info.pUserData = nullptr;

		auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(s_instance, "vkCreateDebugUtilsMessengerEXT");
		ENSURE(func != nullptr, "Failed to get vkCreateDebugUtilsMessengerEXT function address");
		if (func)
			ENSURE(func(s_instance, &messenger_info, allocator, &s_messenger) == VK_SUCCESS, "Failed to create debug messenger");
	}

	void InitInstanceExtensions() {
		uint32_t glfw_extension_count = 0;
		const char** glfw_extensions;
		glfw_extensions = glfwGetRequiredInstanceExtensions(&glfw_extension_count);
		for (uint32_t i = 0; i < glfw_extension_count; i++)
			s_instance_extensions.emplace_back(glfw_extensions[i]);
		if (IS_PLATFORM_MACOS)
			s_instance_extensions.emplace_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);
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
			ENSURE(SupportsExtension(extension), "Missing Required Instance Extension:", extension);
	}

	void InitValidationLayers() {
		if (IS_DEBUG)
			s_validation_layers = { "VK_LAYER_KHRONOS_validation" };
		else
			return;

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
			ENSURE(SupportsValidationLayer(layer), "Missing Required Validation Layer:", layer);
		return;
	}

	//========================================================================
	//	Render Engine Interface
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

		surface.Init(s_instance);
		if (IS_DEBUG)
			InitMessenger();
		RenderDevice::Init(s_instance, surface);
	}

	void RenderEngine::Cleanup() {
		if (s_messenger) {
			auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(s_instance, "vkDestroyDebugUtilsMessengerEXT");
			if (func)
				func(s_instance, s_messenger, nullptr);
		}
		s_messenger = nullptr;
		RenderDevice::Cleanup();
		surface.Cleanup(s_instance);
		if (s_instance)
			vkDestroyInstance(s_instance, nullptr);
		s_instance = nullptr;
	}

	//void RenderEngine::ResetViewport() {
	//	const glm::ivec2& res = Window::GetSize();
	//	glViewport(0, 0, res.x, res.y);
	//}

	//void RenderEngine::SetIndexType(IndexType type) {
	//	static const uint32_t s_index_type_to_gl_index_type[] = { GL_UNSIGNED_SHORT, GL_UNSIGNED_INT };
	//	static const uint64_t s_index_type_to_index_size[] = { sizeof(uint16_t), sizeof(uint32_t) };
	//	s_index_type = s_index_type_to_gl_index_type[static_cast<uint32_t>(type)];
	//	s_index_size = s_index_type_to_index_size[static_cast<uint32_t>(type)];
	//}

	void RenderEngine::SetViewport(uint32_t width, uint32_t height, uint32_t x, uint32_t y) {
		//	glViewport(x, y, width, height);
	}

	//void RenderEngine::SetBlending(bool value) {
	//	static bool s_blending_enabled = false;
	//	SetGLFeature(GL_BLEND, s_blending_enabled, value);
	//}

	//void RenderEngine::SetDepthWrite(bool value) {
	//	glDepthMask(value);
	//}

	//void RenderEngine::SetFaceCulling(bool value) {
	//	static bool s_face_culling_enabled = false;
	//	SetGLFeature(GL_CULL_FACE, s_face_culling_enabled, value);
	//}

	//void RenderEngine::SetFaceCulling(CullingMode mode) {
	//	static const uint32_t s_culling_mode_to_gl_culling_mode[] = { GL_NONE, GL_FRONT, GL_BACK, GL_FRONT_AND_BACK };
	//	static CullingMode s_face_culling_mode = CullingMode::BACK;

	//	SetFaceCulling(mode != CullingMode::NONE);
	//	if (mode != CullingMode::NONE && mode != s_face_culling_mode)
	//		glCullFace(s_culling_mode_to_gl_culling_mode[static_cast<uint32_t>(mode)]);
	//	s_face_culling_mode = mode;
	//}

	//void RenderEngine::SetDepthTest(bool value) {
	//	static bool s_depth_test_enabled = false;
	//	SetGLFeature(GL_DEPTH_TEST, s_depth_test_enabled, value);
	//}

	//void RenderEngine::SetDepthTest(DepthTest mode) {
	//	static const uint32_t s_depth_mode_to_gl_depth_mode[] = { GL_NEVER, GL_LESS, GL_LEQUAL, GL_EQUAL, GL_GEQUAL, GL_GREATER, GL_NOTEQUAL, GL_ALWAYS };
	//	static DepthTest s_depth_test = DepthTest::LESS;

	//	SetDepthTest(s_depth_test != DepthTest::ALWAYS);
	//	if (mode != s_depth_test) {
	//		glDepthFunc(s_depth_mode_to_gl_depth_mode[static_cast<uint32_t>(mode)]);
	//		s_depth_test = mode;
	//	}
	//}

	//void RenderEngine::SetStencilTest(bool value) {
	//	static bool s_stencil_test_enabled = false;
	//	SetGLFeature(GL_STENCIL_TEST, s_stencil_test_enabled, value);
	//}

	//void RenderEngine::SetWireframe(bool value) {
	//	glPolygonMode(GL_FRONT_AND_BACK, value ? GL_LINE : GL_FILL);
	//}

	//void RenderEngine::SetVSync(bool value) {
	//	glfwSwapInterval(value);
	//}

	//void RenderEngine::SetClearDepthValue(float depth) {
	//	glClearDepth(depth);
	//}

	//void RenderEngine::SetClearStencilValue(int32_t stencil) {
	//	glClearStencil(stencil);
	//}

	//void RenderEngine::SetClearColour(float r, float g, float b, float a) {
	//	glClearColor(r, g, b, a);
	//}

	//void RenderEngine::SetClearMode(const std::vector<BufferType>& buffers) {
	//	static const uint32_t s_buffer_type_to_gl_buffer_type[] = { GL_COLOR_BUFFER_BIT, GL_DEPTH_BUFFER_BIT, GL_STENCIL_BUFFER_BIT };
	//	s_clear_mode = 0u;
	//	for (const BufferType& b : buffers)
	//		s_clear_mode |= s_buffer_type_to_gl_buffer_type[static_cast<int>(b)];
	//}

	//void RenderEngine::SetTopology(MeshTopology t) {
	//	static const uint32_t s_mesh_topology_to_gl_mesh_topology[] = { GL_POINTS, GL_LINES, GL_LINE_STRIP, GL_LINE_LOOP, GL_TRIANGLES, GL_TRIANGLE_STRIP, GL_TRIANGLE_FAN };
	//	s_topology = s_mesh_topology_to_gl_mesh_topology[static_cast<int>(t)];
	//}

	//void RenderEngine::EnableColourChannels(bool r, bool g, bool b, bool a) {
	//	glColorMask(r, g, b, a);
	//}

	//void RenderEngine::Clear() {
	//	glClear(s_clear_mode);
	//}

	//void RenderEngine::DrawLinear(uint32_t vertex_count, uint32_t offset) {
	//	if (vertex_count > 0)
	//		glDrawArrays(s_topology, offset, vertex_count);
	//}

	//void RenderEngine::DrawIndexed(uint32_t index_count, uint32_t offset) {
	//	if (index_count > 0)
	//		glDrawElements(s_topology, index_count, s_index_type, reinterpret_cast<void*>(offset * s_index_size));
	//}

	//void RenderEngine::DrawLinearInstanced(uint32_t vertex_count, uint32_t model_count, uint32_t offset) {
	//	if (vertex_count > 0 && model_count > 0)
	//		glDrawArraysInstanced(s_topology, offset, vertex_count, model_count);
	//}

	//void RenderEngine::DrawIndexedInstanced(uint32_t index_count, uint32_t model_count, uint32_t offset) {
	//	if (index_count > 0 && model_count > 0)
	//		glDrawElementsInstanced(s_topology, index_count, s_index_type, reinterpret_cast<void*>(offset * s_index_size), model_count);
	//}

 //   /*
	////========================================================================
	////	Multidraw Command Buffer Class
	////========================================================================

	//MultidrawCommand::MultidrawCommand(uint32_t vertex_count, uint32_t vertex_offset, uint32_t instance_count, uint32_t instance_offset)
	//	: m_vertex_count(vertex_count), m_vertex_offset(vertex_offset), m_instance_count(instance_count), m_instance_offset(instance_offset) {}

	//MultidrawCommandBuffer::MultidrawCommandBuffer(MultidrawCommand* data, uint32_t count, bool calculate_instance_offsets) {
	//	glCreateBuffers(1, &m_id);
	//	Set(data, count, calculate_instance_offsets);
	//}

	//MultidrawCommandBuffer::~MultidrawCommandBuffer() {
	//	glDeleteBuffers(1, &m_id);
	//}

	//uint32_t MultidrawCommandBuffer::GetID() const {
	//	return m_id;
	//}

	//void MultidrawCommandBuffer::Set(MultidrawCommand* data, uint32_t count, bool calculate_instance_offsets) {
	//	if (calculate_instance_offsets)
	//		CalculateInstanceOffsets(data, count, 0);
	//	glNamedBufferData(m_id, sizeof(MultidrawCommand) * count, data, GL_STATIC_DRAW);
	//}

	//void MultidrawCommandBuffer::Update(MultidrawCommand* data, uint32_t count, uint32_t offset) {
	//	void* ptr = glMapNamedBufferRange(m_id, sizeof(MultidrawCommand) * offset, sizeof(MultidrawCommand) * count, GL_MAP_WRITE_BIT);
	//	memcpy(ptr, data, sizeof(MultidrawCommand) * count);
	//	glUnmapNamedBuffer(m_id);
	//}

	//void MultidrawCommandBuffer::Bind() const {
	//	glBindBuffer(GL_DRAW_INDIRECT_BUFFER, m_id);
	//}

	//void MultidrawCommandBuffer::CalculateInstanceOffsets(MultidrawCommand* data, uint32_t count, uint32_t base_offset) {
	//	uint32_t offset = base_offset;
	//	for (uint32_t i = 0; i < count; i++) {
	//		data[i].m_instance_offset = offset;
	//		offset += data[i].m_instance_count;
	//	}
	//}
 //   */
}