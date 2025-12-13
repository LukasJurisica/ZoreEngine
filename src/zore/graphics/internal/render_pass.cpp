#include "zore/graphics/internal/render_pass.hpp"
#include "zore/graphics/devices/render_device.hpp"
#include "zore/debug.hpp"
#include <unordered_map>
#include <vector>

namespace zore {

    static std::unordered_map<uint64_t, std::weak_ptr<RenderPass>> s_render_passes;

    RenderPass::RenderPass(uint32_t attachment_count) {
        std::vector<VkAttachmentDescription> attachments(attachment_count);
        for (uint32_t i = 0; i < attachment_count; i++) {
            attachments[i].format = RenderDevice::GetSurfaceFormat().format;
            attachments[i].samples = VK_SAMPLE_COUNT_1_BIT;
            attachments[i].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
            attachments[i].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
            attachments[i].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
            attachments[i].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
            attachments[i].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
            attachments[i].finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
        }

        std::vector<VkAttachmentReference> color_attachment_refs(attachment_count);
        for (uint32_t i = 0; i < attachment_count; i++) {
            color_attachment_refs[i].attachment = i;
            color_attachment_refs[i].layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
        }
        
		VkSubpassDescription subpass{};
		subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
		subpass.colorAttachmentCount = attachment_count;
		subpass.pColorAttachments = color_attachment_refs.data();

		VkRenderPassCreateInfo render_pass_info{};
		render_pass_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		render_pass_info.attachmentCount = attachment_count;
		render_pass_info.pAttachments = attachments.data();
		render_pass_info.subpassCount = 1;
		render_pass_info.pSubpasses = &subpass;

		VkRenderPass render_pass = VK_NULL_HANDLE;
		ENSURE(vkCreateRenderPass(RenderDevice::Get(), &render_pass_info, nullptr, &render_pass) == VK_SUCCESS, "Failed to create render pass.");
		m_handle = render_pass;
    }

    RenderPass::~RenderPass() {
        if (m_handle) {
            vkDestroyRenderPass(RenderDevice::Get(), m_handle.as<VkRenderPass>(), nullptr);
            m_handle = VK_NULL_HANDLE;
        }
    }

    std::shared_ptr<RenderPass> RenderPass::Get(uint32_t attachment_count) {
        uint64_t key = std::hash<uint32_t>()(attachment_count);
        auto iter = s_render_passes.find(key);
        if (iter != s_render_passes.end())
            if (auto render_pass = iter->second.lock())
                return render_pass;

        auto render_pass = std::make_shared<RenderPass>(attachment_count);
        s_render_passes[key] = render_pass;
        return render_pass;
    }
}