#include "zore/graphics/textures/texture_2D_array.hpp"
#include "zore/graphics/textures/texture_data.hpp"
#include "zore/debug.hpp"
#include <glad/glad.h>

namespace zore {

    Texture2DArray::Texture2DArray(Texture::Format format) : Texture::Base(format) {}

	Texture2DArray::Texture2DArray(void* data, uint32_t width, uint32_t height, uint32_t layers, Texture::Format format) : Texture::Base(format) {
		Set(data, width, height, layers, format);
	}

	Texture2DArray::Texture2DArray(const std::vector<std::string>& filenames, const std::string& root, Texture::Format format) : Texture::Base(format) {
		Set(filenames, root, format);
	}

	void Texture2DArray::Update(void* data, uint32_t offset, uint32_t count) {
		if (data && count > 0 && m_id != GL_INVALID_NAME) {
            ENSURE(offset + count <= m_layers, "Attempted to update a texture array with invalid offset and count");
			glTextureSubImage3D(m_id, 0, 0, 0, offset, m_width, m_height, count, GetBaseFormat(), GL_UNSIGNED_BYTE, data);
		}
	}

	void Texture2DArray::Set(void* data, uint32_t width, uint32_t height, uint32_t layers) {
		ENSURE(width * height * layers != 0, "Cannot create Texture2DArray with 0 area (requested Width, Height or Layers are 0)");
		m_width = width;
		m_height = height;
		m_layers = layers;
		Init(GL_TEXTURE_2D_ARRAY);
		glTextureStorage3D(m_id, 1, GetInternalFormat(), m_width, m_height, m_layers);
		Update(data, 0, m_layers);
	}

	void Texture2DArray::Set(void* data, uint32_t width, uint32_t height, uint32_t layers, Texture::Format format) {
		m_format = format;
		Set(data, width, height, layers);
	}

	void Texture2DArray::Set(const std::vector<std::string>& filenames, const std::string& root, Texture::Format format) {
		// Load the first image, and initialize the Texture2D Array
        Texture::Data image(root + filenames[0], format);
		Set(nullptr, image.width, image.height, static_cast<uint32_t>(filenames.size()), format);
		Update(image.data, 0, 1);

		// Load the remaining images
		for (int i = 1; i < filenames.size(); i++) {
			image.Load(root + filenames[i], format);
			ENSURE(image.width == m_width && image.height == m_height, "The following texture cannot be added to the texture array as it has different dimensions: " + root + filenames[i]);
			Update(image.data, i, 1);
		}
	}
}