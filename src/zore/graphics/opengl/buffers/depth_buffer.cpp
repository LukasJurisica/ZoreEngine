#include "zore/graphics/buffers/depth_buffer.hpp"
#include "zore/graphics/textures/texture_base.hpp"
#include "zore/debug.hpp"
#include <glad/glad.h>

namespace zore {

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

	uint32_t DepthBuffer::GetInternalFormat() {
		static const uint32_t S_DEPTH_FORMAT_TO_INTERNAL_FORMAT[] = { GL_DEPTH_COMPONENT32, GL_DEPTH24_STENCIL8 };
		return S_DEPTH_FORMAT_TO_INTERNAL_FORMAT[static_cast<uint32_t>(m_format) & 1];
	}

	uint32_t DepthBuffer::GetBaseFormat() {
		static const uint32_t S_DEPTH_FORMAT_TO_BASE_FORMAT[] = { GL_DEPTH_ATTACHMENT, GL_DEPTH_STENCIL_ATTACHMENT };
		return S_DEPTH_FORMAT_TO_BASE_FORMAT[static_cast<uint32_t>(m_format) & 1];
	}

	uint32_t DepthBuffer::GetID() const {
		return m_id;
	}

	void DepthBuffer::Set(uint32_t width, uint32_t height) {
		if (IsRenderBuffer()) {
			if (m_id != GL_INVALID_NAME)
				glDeleteRenderbuffers(1, &m_id);
			glCreateRenderbuffers(1, &m_id);
			glNamedRenderbufferStorage(m_id, GetInternalFormat(), width, height);
		}
		else if (IsRenderTexture()) {
			if (m_id != GL_INVALID_NAME)
				glDeleteTextures(1, &m_id);
			glCreateTextures(GL_TEXTURE_2D, 1, &m_id);
			glTextureStorage2D(m_id, 1, GetBaseFormat(), width, height);
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
		Bind(Texture::Base::GetNamedTextureSlot(slot));
	}

	bool DepthBuffer::IsRenderBuffer() const {
		return m_format == DepthFormat::DEPTH32_BUFFER || m_format == DepthFormat::DEPTH24_STENCIL8_BUFFER;
	}

	bool DepthBuffer::IsRenderTexture() const {
		return m_format == DepthFormat::DEPTH32_TEXTURE || m_format == DepthFormat::DEPTH24_STENCIL8_TEXTURE;
	}
}