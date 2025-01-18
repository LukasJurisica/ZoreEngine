#include "zore/graphics/FrameBuffer.hpp"
#include "zore/Debug.hpp"
#include <glad/glad.h>

#define MAX_FRAMEBUFFER_ATTACHMENTS 8

namespace zore {

	static const uint32_t S_DEPTH_FORMAT_TO_BASE_FORMAT[] = { GL_DEPTH_ATTACHMENT, GL_DEPTH_STENCIL_ATTACHMENT };
	static const uint32_t S_DEPTH_FORMAT_TO_INTERNAL_FORMAT[] = { GL_DEPTH_COMPONENT32, GL_DEPTH24_STENCIL8 };

	//========================================================================
	//	Depth Buffer Class
	//========================================================================

	DepthBuffer::DepthBuffer() : m_id(GL_INVALID_NAME), m_slot(0), m_format(DepthFormat::NONE) {}

	DepthBuffer::DepthBuffer(uint32_t width, uint32_t height, DepthFormat format) : m_slot(0) {
		Set(width, height, format);
	}

	DepthBuffer::~DepthBuffer() {
		if (m_id != GL_INVALID_NAME) {
			if (IsRenderBuffer())
				glDeleteRenderbuffers(1, &m_id);
			else if (IsRenderTexture())
				glDeleteTextures(1, &m_id);
		}
	}

	uint32_t DepthBuffer::GetID() const {
		return m_id;
	}

	void DepthBuffer::Set(uint32_t width, uint32_t height) {
		if (IsRenderBuffer()) {
			if (m_id != GL_INVALID_NAME)
				glDeleteRenderbuffers(1, &m_id);
			glCreateRenderbuffers(1, &m_id);
			glNamedRenderbufferStorage(m_id, S_DEPTH_FORMAT_TO_INTERNAL_FORMAT[static_cast<uint32_t>(m_format)], width, height);
		}
		else if (IsRenderTexture()) {
			if (m_id != GL_INVALID_NAME)
				glDeleteTextures(1, &m_id);
			glCreateTextures(GL_TEXTURE_2D, 1, &m_id);
			glTextureStorage2D(m_id, 1, S_DEPTH_FORMAT_TO_INTERNAL_FORMAT[static_cast<uint32_t>(m_format) - 2], width, height);
		}
	}

	void DepthBuffer::Set(uint32_t width, uint32_t height, DepthFormat format) {
		m_format = format;
		Set(width, height);
	}

	void DepthBuffer::Bind() const {
		if (IsRenderTexture())
			glBindTextureUnit(m_slot, m_id);
		else if (IsRenderBuffer())
			Logger::Error("Cannot bind Depth Buffer as texture: This FrameBuffer contains a Render Buffer Depth Buffer");
		else
			Logger::Error("Cannot bind Depth Buffer as texture: This FrameBuffer does not contain a Depth Buffer.");
	}

	void DepthBuffer::Bind(uint32_t slot) {
		m_slot = slot;
		Bind();
	}

	void DepthBuffer::Bind(const std::string& slot) {
		Bind(Texture::GetNamedTextureSlot(slot));
	}

	bool DepthBuffer::IsRenderBuffer() const {
		return m_format == DepthFormat::DEPTH32_BUFFER || m_format == DepthFormat::DEPTH24_STENCIL8_BUFFER;
	}

	bool DepthBuffer::IsRenderTexture() const {
		return m_format == DepthFormat::DEPTH32_TEXTURE || m_format == DepthFormat::DEPTH24_STENCIL8_TEXTURE;
	}

	//========================================================================
	//	Frame Buffer Class
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

	DepthBuffer& FrameBuffer::GetDepthTexture() {
		return m_depth_buffer;
	}

	void FrameBuffer::Set(uint32_t width, uint32_t height) {
		m_colour_buffer.Set(nullptr, width, height, m_attachment_count);
		for (uint32_t i = 0; i < m_attachment_count; i++)
			glNamedFramebufferTextureLayer(m_id, GL_COLOR_ATTACHMENT0 + i, m_colour_buffer.GetID(), 0, i);

		m_depth_buffer.Set(width, height);
		if (m_depth_buffer.IsRenderBuffer())
			glNamedFramebufferRenderbuffer(m_id, S_DEPTH_FORMAT_TO_BASE_FORMAT[static_cast<uint32_t>(m_depth_buffer.m_format)], GL_RENDERBUFFER, m_depth_buffer.GetID());
		else if (m_depth_buffer.IsRenderTexture())
			glNamedFramebufferTexture(m_id, S_DEPTH_FORMAT_TO_BASE_FORMAT[static_cast<uint32_t>(m_depth_buffer.m_format) - 2], m_depth_buffer.GetID(), 0);
	}

	void FrameBuffer::Set(uint32_t width, uint32_t height, uint32_t a_count, Texture::Format a_format, DepthFormat d_format) {
		// Create colour attachments
		ENSURE((m_attachment_count = a_count) <= MAX_FRAMEBUFFER_ATTACHMENTS, "Cannot create framebuffer: too many colour attachments requested.");
		uint32_t active_attachments[MAX_FRAMEBUFFER_ATTACHMENTS];
		m_colour_buffer.Set(nullptr, width, height, m_attachment_count, a_format);
		for (uint32_t i = 0; i < m_attachment_count; i++) {
			active_attachments[i] = GL_COLOR_ATTACHMENT0 + i;
			glNamedFramebufferTextureLayer(m_id, active_attachments[i], m_colour_buffer.GetID(), 0, i);
		}
		glNamedFramebufferDrawBuffers(m_id, m_attachment_count, active_attachments);

		// Create Depth/Stencil Buffer (If requested)
		uint32_t depthFormatIndex = static_cast<uint32_t>(d_format);
		m_depth_buffer.Set(width, height, d_format);
		if (m_depth_buffer.IsRenderBuffer())
			glNamedFramebufferRenderbuffer(m_id, S_DEPTH_FORMAT_TO_BASE_FORMAT[static_cast<uint32_t>(m_depth_buffer.m_format)], GL_RENDERBUFFER, m_depth_buffer.GetID());
		else if (m_depth_buffer.IsRenderTexture())
			glNamedFramebufferTexture(m_id, S_DEPTH_FORMAT_TO_BASE_FORMAT[static_cast<uint32_t>(m_depth_buffer.m_format) - 2], m_depth_buffer.GetID(), 0);

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
}