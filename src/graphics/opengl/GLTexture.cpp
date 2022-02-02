#include "graphics/opengl/GLTexture.hpp"
#include "stb/stb_image.h"
#include "debug/Debug.hpp"
#include <glad/glad.h>

namespace zore {

	//========================================================================
	//	OpenGL Base Texture Class
	//========================================================================

	static const unsigned int TextureFormatToGLFormat[] = { GL_R8, GL_RG8, GL_RGB8, GL_RGBA8 };

	GLTexture::GLTexture(unsigned int target, unsigned int textureFormat) : format(textureFormat), unit(0) {
		glCreateTextures(target, 1, &id);
	}

	GLTexture::~GLTexture() {
		glDeleteTextures(1, &id);
	}

	unsigned int GLTexture::GetID() {
		return id;
	}

	//========================================================================
	//	OpenGL 2D Texture Class
	//========================================================================

	GLTexture2D::GLTexture2D(const std::string& name, TextureFormat textureFormat) :
		Texture2D(0, 0), GLTexture(GL_TEXTURE_2D, TextureFormatToGLFormat[static_cast<int>(textureFormat)]) {
		//glTextureStorage2D(id, 0, internalFormat, width, height);
		// Upload data into buffer
		//glTextureSubImage2D(textureHandle, 0, 0, 0, imageW, imageH, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
	}

	GLTexture2D::GLTexture2D(unsigned int width, unsigned int height, TextureFormat textureFormat, void* data) :
		Texture2D(width, height), GLTexture(GL_TEXTURE_2D, TextureFormatToGLFormat[static_cast<int>(textureFormat)]) {
		glTextureStorage2D(id, 1, format, width, height);
		if (data)
			glTextureSubImage2D(id, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glTextureParameteri(id, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(id, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTextureParameteri(id, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTextureParameteri(id, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	}

	void GLTexture2D::Bind() const {
		glBindTextureUnit(unit, id);
	}

	void GLTexture2D::Unbind() const {
		glBindTextureUnit(unit, 0);
	}

	void GLTexture2D::SetTextureSlot(unsigned int slot) {
		unit = slot;
		glBindTextureUnit(unit, id);
	}

	//========================================================================
	//	OpenGL 2D Texture Array Class
	//========================================================================
	
	GLTexture2DArray::GLTexture2DArray(uint width, uint height, const std::vector<std::string>& filenames, TextureFormat textureFormat) :
	Texture2DArray(width, height, static_cast<uint>(filenames.size())), GLTexture(GL_TEXTURE_2D_ARRAY, TextureFormatToGLFormat[static_cast<int>(textureFormat)]) {
		glTextureStorage3D(id, 1, format, width, height, layers);

		int w, h, c;
		for (int i = 0; i < filenames.size(); i++) {
			ubyte* data = stbi_load(filenames[i].c_str(), &w, &h, &c, 4);
			ENSURE(data, "Could not load texture: " + filenames[i]);
			glTextureSubImage3D(id, 0, 0, 0, i, width, height, 1, GL_RGBA, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);
		}

		glTextureParameteri(id, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTextureParameteri(id, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTextureParameteri(id, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTextureParameteri(id, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	}

	GLTexture2DArray::GLTexture2DArray(uint width, uint height, uint layers, void* data, TextureFormat textureFormat) :
		Texture2DArray(width, height, layers), GLTexture(GL_TEXTURE_2D_ARRAY, TextureFormatToGLFormat[static_cast<int>(textureFormat)]) {
		glTextureStorage3D(id, 1, format, width, height, layers);
		if (data)
			glTextureSubImage3D(id, 0, 0, 0, 0, width, height, layers, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glTextureParameteri(id, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTextureParameteri(id, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTextureParameteri(id, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTextureParameteri(id, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	}

	void GLTexture2DArray::Bind() const {
		glBindTextureUnit(unit, id);
	}

	void GLTexture2DArray::Unbind() const {
		glBindTextureUnit(unit, 0);
	}

	void GLTexture2DArray::SetTextureSlot(unsigned int slot) {
		unit = slot;
		glBindTextureUnit(unit, id);
	}

	void GLTexture2DArray::SetTextureData(uint first, uint count, void* data) {
		glTextureSubImage3D(id, 0, 0, 0, first, width, height, count, GL_RGBA, GL_UNSIGNED_BYTE, data);
	}
}