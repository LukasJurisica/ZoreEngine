#include "zore/graphics/graphics_pipeline.hpp"
#include "zore/graphics/vulkan/vulkan_render_device.hpp"
#include "zore/debug.hpp"
#include <vulkan/vulkan.h>

namespace zore {

	handle_t CreatePipelineLayout() {
		VkPipelineLayoutCreateInfo pipeline_layout_info{};
		pipeline_layout_info.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipeline_layout_info.setLayoutCount = 0;
		pipeline_layout_info.pSetLayouts = nullptr;
		pipeline_layout_info.pushConstantRangeCount = 0;
		pipeline_layout_info.pPushConstantRanges = nullptr;

		VkPipelineLayout pipeline_layout = VK_NULL_HANDLE;
		ENSURE(vkCreatePipelineLayout(RenderDevice::Get(), &pipeline_layout_info, nullptr, &pipeline_layout) != VK_SUCCESS, "Failed to create pipeline layout!");
		return pipeline_layout;
	}

	handle_t CreateRenderPass() {
		VkAttachmentDescription color_attachment_description{};
		color_attachment_description.format = RenderDevice::GetSurfaceFormat().format;
		color_attachment_description.samples = VK_SAMPLE_COUNT_1_BIT;
		color_attachment_description.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		color_attachment_description.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
		color_attachment_description.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		color_attachment_description.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		color_attachment_description.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		color_attachment_description.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

		VkAttachmentReference color_attachment_ref{};
		color_attachment_ref.attachment = 0;
		color_attachment_ref.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

		VkSubpassDescription subpass{};
		subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
		subpass.colorAttachmentCount = 1;
		subpass.pColorAttachments = &color_attachment_ref;

		VkRenderPassCreateInfo render_pass_info{};
		render_pass_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		render_pass_info.attachmentCount = 1;
		render_pass_info.pAttachments = &color_attachment_description;
		render_pass_info.subpassCount = 1;
		render_pass_info.pSubpasses = &subpass;

		VkRenderPass render_pass = VK_NULL_HANDLE;
		ENSURE(vkCreateRenderPass(RenderDevice::Get(), &render_pass_info, nullptr, &render_pass) == VK_SUCCESS, "Failed to create render pass.");
		return render_pass;
	}

	GraphicsPipeline::GraphicsPipeline(const Shader& shader, const VertexLayout& layout) {
		VkPipelineInputAssemblyStateCreateInfo input_assembly_info{};
		input_assembly_info.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
		input_assembly_info.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
		input_assembly_info.primitiveRestartEnable = VK_FALSE;

		VkPipelineVertexInputStateCreateInfo vertex_layout_info{};
		vertex_layout_info.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
		vertex_layout_info.vertexBindingDescriptionCount = 0;
		vertex_layout_info.pVertexBindingDescriptions = nullptr;
		vertex_layout_info.vertexAttributeDescriptionCount = 0;
		vertex_layout_info.pVertexAttributeDescriptions = nullptr;

		static const VkShaderStageFlagBits s_vk_shader_stages[] = {
			VkShaderStageFlagBits::VK_SHADER_STAGE_VERTEX_BIT,
			VkShaderStageFlagBits::VK_SHADER_STAGE_GEOMETRY_BIT,
			VkShaderStageFlagBits::VK_SHADER_STAGE_FRAGMENT_BIT,
			VkShaderStageFlagBits::VK_SHADER_STAGE_COMPUTE_BIT,
			VkShaderStageFlagBits::VK_SHADER_STAGE_ALL
		};
		std::vector<VkPipelineShaderStageCreateInfo> shader_stages;
		for (const auto& [stage, module] : shader.GetStages()) {
			VkPipelineShaderStageCreateInfo shader_stage_info{};
			shader_stage_info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
			shader_stage_info.stage = s_vk_shader_stages[static_cast<uint32_t>(stage)];
			shader_stage_info.module = module.as<VkShaderModule>();
			shader_stage_info.pName = "main";
			shader_stages.emplace_back(shader_stage_info);
		}

		// STATIC VERSION
		VkViewport viewport{};
		viewport.x = 0.0f;
		viewport.y = 0.0f;
		viewport.width = static_cast<float>(RenderDevice::GetExtent().width);
		viewport.height = static_cast<float>(RenderDevice::GetExtent().height);
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;

		VkRect2D scissor{};
		scissor.offset = { 0, 0 };
		scissor.extent = RenderDevice::GetExtent();

		VkPipelineViewportStateCreateInfo viewport_info{};
		viewport_info.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
		viewport_info.viewportCount = 1;
		viewport_info.pViewports = &viewport;
		viewport_info.scissorCount = 1;
		viewport_info.pScissors = &scissor;
		
		/*
		// DYNAMIC VERSION
		static const VkDynamicState dynamic_states[] = { VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR };
		VkPipelineDynamicStateCreateInfo dynamic_state_info{};
		dynamic_state_info.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
		dynamic_state_info.dynamicStateCount = static_cast<uint32_t>(std::size(dynamic_states));
		dynamic_state_info.pDynamicStates = dynamic_states;

		VkPipelineViewportStateCreateInfo viewport_info{};
		viewport_info.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
		viewport_info.viewportCount = 1;
		viewport_info.scissorCount = 1;
		*/

		VkPipelineRasterizationStateCreateInfo rasterizer_info{};
		rasterizer_info.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
		rasterizer_info.depthClampEnable = VK_FALSE;
		rasterizer_info.rasterizerDiscardEnable = VK_FALSE;
		rasterizer_info.polygonMode = VK_POLYGON_MODE_FILL;
		rasterizer_info.lineWidth = 1.0f;
		rasterizer_info.cullMode = VK_CULL_MODE_BACK_BIT;
		rasterizer_info.frontFace = VK_FRONT_FACE_CLOCKWISE;
		rasterizer_info.depthBiasEnable = VK_FALSE;
		rasterizer_info.depthBiasConstantFactor = 0.0f;
		rasterizer_info.depthBiasClamp = 0.0f;
		rasterizer_info.depthBiasSlopeFactor = 0.0f;

		VkPipelineMultisampleStateCreateInfo multisampling{};
		multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
		multisampling.sampleShadingEnable = VK_FALSE;
		multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
		multisampling.minSampleShading = 1.0f;
		multisampling.pSampleMask = nullptr;
		multisampling.alphaToCoverageEnable = VK_FALSE;
		multisampling.alphaToOneEnable = VK_FALSE;

		VkPipelineColorBlendAttachmentState color_blend_attachment{};
		color_blend_attachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
		color_blend_attachment.blendEnable = VK_FALSE;
		// Normal Blending
		/*
		color_blend_attachment.blendEnable = VK_TRUE;
		color_blend_attachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
		color_blend_attachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
		color_blend_attachment.colorBlendOp = VK_BLEND_OP_ADD;
		color_blend_attachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
		color_blend_attachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
		color_blend_attachment.alphaBlendOp = VK_BLEND_OP_ADD;
		*/

		VkPipelineColorBlendStateCreateInfo color_blend_info{};
		color_blend_info.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
		color_blend_info.logicOpEnable = VK_FALSE;
		color_blend_info.logicOp = VK_LOGIC_OP_COPY;
		color_blend_info.attachmentCount = 1;
		color_blend_info.pAttachments = &color_blend_attachment;
		color_blend_info.blendConstants[0] = 0.0f;
		color_blend_info.blendConstants[1] = 0.0f;
		color_blend_info.blendConstants[2] = 0.0f;
		color_blend_info.blendConstants[3] = 0.0f;

		m_pipeline_layout = CreatePipelineLayout();
		m_render_pass = CreateRenderPass();

		VkGraphicsPipelineCreateInfo pipeline_info{};
		pipeline_info.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
		pipeline_info.stageCount = static_cast<uint32_t>(shader_stages.size());
		pipeline_info.pStages = shader_stages.data();

		pipeline_info.pVertexInputState = &vertex_layout_info;
		pipeline_info.pInputAssemblyState = &input_assembly_info;
		pipeline_info.pViewportState = &viewport_info;
		pipeline_info.pRasterizationState = &rasterizer_info;
		pipeline_info.pMultisampleState = &multisampling;
		pipeline_info.pDepthStencilState = nullptr;
		pipeline_info.pColorBlendState = &color_blend_info;
		//pipeline_info.pDynamicState = &dynamic_state_info;
		pipeline_info.layout = m_pipeline_layout.as<VkPipelineLayout>();
		pipeline_info.renderPass = m_render_pass.as<VkRenderPass>();
		pipeline_info.subpass = 0;
		pipeline_info.basePipelineHandle = VK_NULL_HANDLE;
		pipeline_info.basePipelineIndex = -1;

		VkPipeline pipeline = VK_NULL_HANDLE;
		ENSURE(vkCreateGraphicsPipelines(RenderDevice::Get(), VK_NULL_HANDLE, 1, &pipeline_info, nullptr, &pipeline) != VK_SUCCESS, "Failed to create graphics pipeline.");
		m_pipeline_id = pipeline;
	}

	GraphicsPipeline::~GraphicsPipeline() {
		if (m_pipeline_id) {
			vkDestroyPipeline(RenderDevice::Get(), m_pipeline_id.as<VkPipeline>(), nullptr);
			m_pipeline_id = VK_NULL_HANDLE;
		}
		if (m_pipeline_layout) {
			vkDestroyPipelineLayout(RenderDevice::Get(), m_pipeline_layout.as<VkPipelineLayout>(), nullptr);
			m_pipeline_layout = VK_NULL_HANDLE;
		}
		if (m_render_pass) {
			vkDestroyRenderPass(RenderDevice::Get(), m_render_pass.as<VkRenderPass>(), nullptr);
			m_render_pass = VK_NULL_HANDLE;
		}
	}
}