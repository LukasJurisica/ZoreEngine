#include "zore/graphics/FrameBuffer.hpp"
#include "zore/Debug.hpp"
#include <glad/glad.h>

#define MAX_FRAMEBUFFER_ATTACHMENTS 8

namespace zore {

	static const uint32_t DepthFormatToGLBaseFormat[] = { GL_DEPTH_ATTACHMENT, GL_DEPTH_STENCIL_ATTACHMENT };
	static const uint32_t DepthFormatToGLSizedFormat[] = { GL_DEPTH_COMPONENT32, GL_DEPTH24_STENCIL8 };

	//========================================================================
	//	Depth Buffer Class
	//========================================================================

	DepthBuffer::DepthBuffer() : m_id(0), m_slot(0), m_format(DepthFormat::NONE) {}

	DepthBuffer::DepthBuffer(uint32_t width, uint32_t height, DepthFormat depthFormat) : m_id(0), m_slot(0), m_format(DepthFormat::NONE) {
		Set(width, height, depthFormat);
	}

	DepthBuffer::~DepthBuffer() {
		Free();
	}

	uint32_t DepthBuffer::GetID() const {
		return m_id;
	}

	void DepthBuffer::SetSize(uint32_t width, uint32_t height) {
		uint32_t formatIndex = static_cast<uint32_t>(m_format);
		if (m_format == DepthFormat::DEPTH32_BUFFER || m_format == DepthFormat::DEPTH24_STENCIL8_BUFFER)
			glNamedRenderbufferStorage(m_id, DepthFormatToGLSizedFormat[formatIndex], width, height);
		else if (m_format == DepthFormat::DEPTH32_TEXTURE || m_format == DepthFormat::DEPTH24_STENCIL8_TEXTURE) {
			glDeleteTextures(1, &m_id);
			glCreateTextures(GL_TEXTURE_2D, 1, &m_id);
			glTextureStorage2D(m_id, 1, DepthFormatToGLSizedFormat[formatIndex - 2], width, height);
		}
	}

	void DepthBuffer::Set(uint32_t width, uint32_t height, DepthFormat depthFormat) {
		Free();
		uint32_t formatIndex = static_cast<uint32_t>(depthFormat);
		if (depthFormat == DepthFormat::DEPTH32_BUFFER || depthFormat == DepthFormat::DEPTH24_STENCIL8_BUFFER) {
			glCreateRenderbuffers(1, &m_id);
			glNamedRenderbufferStorage(m_id, DepthFormatToGLSizedFormat[formatIndex], width, height);
		}
		else if (depthFormat == DepthFormat::DEPTH32_TEXTURE || depthFormat == DepthFormat::DEPTH24_STENCIL8_TEXTURE) {
			glCreateTextures(GL_TEXTURE_2D, 1, &m_id);
			glTextureStorage2D(m_id, 1, DepthFormatToGLSizedFormat[formatIndex - 2], width, height);
		}
		m_format = depthFormat;
	}

	void DepthBuffer::Bind() const {
		if (m_format == DepthFormat::NONE)
			Logger::Error("Cannot bind Depth Buffer as texture: This FrameBuffer does not contain a Depth Buffer.");
		else if (m_format == DepthFormat::DEPTH32_BUFFER || m_format == DepthFormat::DEPTH24_STENCIL8_BUFFER)
			Logger::Error("Cannot bind Depth Buffer as texture: This FrameBuffer contains a Render Buffer Depth Buffer");
		else
			glBindTextureUnit(m_slot, m_id);
	}

	void DepthBuffer::Bind(uint32_t slot) {
		m_slot = slot;
		Bind();
	}

	void DepthBuffer::Bind(const std::string& slot) {
		m_slot = Texture::GetNamedTextureSlot(slot);
		Bind();
	}

	void DepthBuffer::Free() {
		if (m_format == DepthFormat::DEPTH32_BUFFER || m_format == DepthFormat::DEPTH24_STENCIL8_BUFFER)
			glDeleteRenderbuffers(1, &m_id);
		else if (m_format == DepthFormat::DEPTH32_TEXTURE || m_format == DepthFormat::DEPTH24_STENCIL8_TEXTURE)
			glDeleteTextures(1, &m_id);
	}

	//========================================================================
	//	Frame Buffer Class
	//========================================================================

	FrameBuffer::FrameBuffer() : m_attachment_count(0) {
		glCreateFramebuffers(1, &m_id);
	}

	FrameBuffer::FrameBuffer(uint32_t width, uint32_t height, uint32_t attachmentCount, Texture::Format attachmentFormat, DepthFormat depthFormat) {
		glCreateFramebuffers(1, &m_id);
		Set(width, height, attachmentCount, attachmentFormat, depthFormat);
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

	DepthBuffer& FrameBuffer::GetDepthTexture() {
		return m_depth_buffer;
	}

	void FrameBuffer::SetSize(uint32_t width, uint32_t height) {
		m_colour_buffer.Set(nullptr, width, height, m_attachment_count);
		m_colour_buffer.Bind();
		uint32_t texture2DArrayID = m_colour_buffer.GetID();
		for (uint32_t i = 0; i < m_attachment_count; i++)
			glNamedFramebufferTextureLayer(m_id, GL_COLOR_ATTACHMENT0 + i, texture2DArrayID, 0, i);

		m_depth_buffer.SetSize(width, height);
		uint32_t depthFormatIndex = static_cast<uint32_t>(m_depth_buffer.m_format);
		if (m_depth_buffer.m_format == DepthFormat::DEPTH32_TEXTURE || m_depth_buffer.m_format == DepthFormat::DEPTH24_STENCIL8_TEXTURE)
			glNamedFramebufferTexture(m_id, DepthFormatToGLBaseFormat[depthFormatIndex - 2], m_depth_buffer.GetID(), 0);
	}

	void FrameBuffer::Set(uint32_t width, uint32_t height, uint32_t attachmentCount, Texture::Format attachmentFormat, DepthFormat depthFormat) {
		// Create colour attachments
		m_attachment_count = attachmentCount;
		uint32_t activeAttachments[MAX_FRAMEBUFFER_ATTACHMENTS];
		m_colour_buffer.Set(nullptr, width, height, m_attachment_count, Texture::Format::RGBA);
		uint32_t texture2DArrayID = m_colour_buffer.GetID();
		for (uint32_t i = 0; i < m_attachment_count; i++) {
			activeAttachments[i] = GL_COLOR_ATTACHMENT0 + i;
			glNamedFramebufferTextureLayer(m_id, activeAttachments[i], texture2DArrayID, 0, i);
		}
		glNamedFramebufferDrawBuffers(m_id, m_attachment_count, activeAttachments);

		// Create Depth/Stencil Buffer (If requested)
		uint32_t depthFormatIndex = static_cast<uint32_t>(depthFormat);
		m_depth_buffer.Set(width, height, depthFormat);
		if (depthFormat == DepthFormat::DEPTH32_BUFFER || depthFormat == DepthFormat::DEPTH24_STENCIL8_BUFFER)
			glNamedFramebufferRenderbuffer(m_id, DepthFormatToGLBaseFormat[depthFormatIndex], GL_RENDERBUFFER, m_depth_buffer.GetID());
		else if (depthFormat == DepthFormat::DEPTH32_TEXTURE || depthFormat == DepthFormat::DEPTH24_STENCIL8_TEXTURE)
			glNamedFramebufferTexture(m_id, DepthFormatToGLBaseFormat[depthFormatIndex - 2], m_depth_buffer.GetID(), 0);

		// Ensure FrameBuffer was created successfully
		uint32_t retval = glCheckNamedFramebufferStatus(m_id, GL_FRAMEBUFFER);
		ENSURE(retval == GL_FRAMEBUFFER_COMPLETE, "Error creating FrameBuffer. Error code: " + std::to_string(retval));
	}

	void FrameBuffer::Bind() const {
		glBindFramebuffer(GL_FRAMEBUFFER, m_id);
	}

	void FrameBuffer::Unbind() {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
}