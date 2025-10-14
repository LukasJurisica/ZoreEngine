#include "zore/graphics/frame_buffer.hpp"
#include "zore/debug.hpp"
#include <glad/glad.h>

#define MAX_FRAMEBUFFER_ATTACHMENTS 8

namespace zore {

	struct FrameBufferAttachmentIndexArray {
		FrameBufferAttachmentIndexArray() {
			for (uint32_t i = 0; i < MAX_FRAMEBUFFER_ATTACHMENTS; i++)
				indices[i] = GL_COLOR_ATTACHMENT0 + i;
		}
		~FrameBufferAttachmentIndexArray() = default;
		uint32_t indices[MAX_FRAMEBUFFER_ATTACHMENTS];
	};

	//========================================================================
	//	Frame Buffer
	//========================================================================

	FrameBuffer::FrameBuffer() : m_attachment_count(0) {
		glCreateFramebuffers(1, &m_id);
	}

	FrameBuffer::FrameBuffer(uint32_t width, uint32_t height, uint32_t a_count, Texture::Format a_format, DepthFormat d_format) {
		glCreateFramebuffers(1, &m_id);
		Set(width, height, a_count, a_format, d_format);
	}

	FrameBuffer::~FrameBuffer() {
		glDeleteFramebuffers(1, &m_id);
	}

	uint32_t FrameBuffer::GetID() const {
		return m_id;
	}

	Texture2DArray& FrameBuffer::GetColourTexture() {
		return m_colour_buffer;
	}

	DepthBuffer& FrameBuffer::GetDepthBuffer() {
		return m_depth_buffer;
	}

	void FrameBuffer::Set(uint32_t width, uint32_t height) {
		m_colour_buffer.Set(nullptr, width, height, m_attachment_count);
		m_depth_buffer.Set(width, height);
		UpdateAttachments();
	}

	void FrameBuffer::Set(uint32_t width, uint32_t height, uint32_t a_count, Texture::Format a_format, DepthFormat d_format) {
		ENSURE((m_attachment_count = a_count) <= MAX_FRAMEBUFFER_ATTACHMENTS, "Cannot create framebuffer: too many colour attachments requested.");
		m_colour_buffer.Set(nullptr, width, height, m_attachment_count, a_format);
		m_depth_buffer.Set(width, height, d_format);
		UpdateAttachments();
		static FrameBufferAttachmentIndexArray active_attachments;
		glNamedFramebufferDrawBuffers(m_id, m_attachment_count, active_attachments.indices);

		// Ensure FrameBuffer was created successfully
		uint32_t result = glCheckNamedFramebufferStatus(m_id, GL_FRAMEBUFFER);
		ENSURE(result == GL_FRAMEBUFFER_COMPLETE, "Error creating FrameBuffer. Error code: " + std::to_string(result));
	}

	void FrameBuffer::Bind() const {
		glBindFramebuffer(GL_FRAMEBUFFER, m_id);
	}

	void FrameBuffer::Unbind() {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void FrameBuffer::UpdateAttachments() {
		for (uint32_t i = 0; i < m_attachment_count; i++)
			glNamedFramebufferTextureLayer(m_id, GL_COLOR_ATTACHMENT0 + i, m_colour_buffer.GetID(), 0, i);

		if (m_depth_buffer.IsRenderBuffer())
			glNamedFramebufferRenderbuffer(m_id, m_depth_buffer.GetBaseFormat(), GL_RENDERBUFFER, m_depth_buffer.GetID());
		else if (m_depth_buffer.IsRenderTexture())
			glNamedFramebufferTexture(m_id, m_depth_buffer.GetBaseFormat(), m_depth_buffer.GetID(), 0);
	}
}