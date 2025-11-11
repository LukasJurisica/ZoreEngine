#include "zore/graphics/textures/texture_2D.hpp"
#include "zore/graphics/textures/texture_data.hpp"
#include "zore/debug.hpp"
#include <glad/glad.h>

namespace zore {

	//========================================================================
	//	Texture2D
	//========================================================================

    Texture2D::Texture2D(Texture::Format format) : Texture::Base(format) {}

	Texture2D::Texture2D(void* data, uint32_t width, uint32_t height, Texture::Format format) : Texture::Base(format) {
		Set(data, width, height, format);
	}

	Texture2D::Texture2D(const std::string& filename, Texture::Format format) : Texture::Base(format) {
		Set(filename, format);
	}

	void Texture2D::Update(void* data) {
		if (data && m_id != GL_INVALID_NAME)
			glTextureSubImage2D(m_id, 0, 0, 0, m_width, m_height, GetBaseFormat(), GL_UNSIGNED_BYTE, data);
	}

	void Texture2D::Set(void* data, uint32_t width, uint32_t height) {
		ENSURE(width * height != 0, "Cannot create Texture2D with 0 area (requested Width or Height are 0)");
		m_width = width;
		m_height = height;
		Init(GL_TEXTURE_2D);
		glTextureStorage2D(m_id, 1, GetInternalFormat(), m_width, m_height);
		Update(data);
	}

	void Texture2D::Set(void* data, uint32_t width, uint32_t height, Texture::Format format) {
		m_format = format;
		Set(data, width, height);
	}

	void Texture2D::Set(const std::string& filename, Texture::Format format) {
		Texture::Data image(filename, format);
		Set(image.data, image.width, image.height, format);
	}
}