#include "zore/graphics/internal/vertex_layout.hpp"
#include "zore/graphics/shader.hpp"
#include <vulkan/vulkan.h>

namespace zore {

	//========================================================================
	//	Vertex Buffer Layout
	//========================================================================

	VertexLayout::VertexLayout(const Shader& shader, const std::vector<VertexElement>& elements, uint32_t interval) {
		Set(shader, elements, interval);
	}

	void VertexLayout::Set(const Shader& shader, const std::vector<VertexElement>& elements, uint32_t interval) {
		InitAttributes(elements, interval, shader);
	}

	void VertexLayout::InitAttributes(const std::vector<VertexElement>& elements, uint32_t interval, const Shader& shader) {
		VkPipelineVertexInputStateCreateInfo vertex_layout_info{};
		vertex_layout_info.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
		vertex_layout_info.vertexBindingDescriptionCount = 0;
		vertex_layout_info.pVertexBindingDescriptions = nullptr;
		vertex_layout_info.vertexAttributeDescriptionCount = 0;
		vertex_layout_info.pVertexAttributeDescriptions = nullptr;
		
	}
}