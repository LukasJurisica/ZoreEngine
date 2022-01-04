#include "graphics/opengl/GLTexture.hpp"
#include "debug/Debug.hpp"
#include <glad/glad.h>

namespace zore {

	//========================================================================
	//	OpenGL Base Texture Class
	//========================================================================

	static const unsigned int TextureFormatToGLFormat[] = { GL_RED, GL_RG, GL_RGB, GL_RGBA8 };

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

	GLTexture2D::GLTexture2D(const std::string& name) : GLTexture(GL_TEXTURE_2D, 0) {
		//glTextureStorage2D(id, 0, internalFormat, width, height);
		// Upload data into buffer
		//glTextureSubImage2D(textureHandle, 0, 0, 0, imageW, imageH, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
	}

	GLTexture2D::GLTexture2D(unsigned int width, unsigned int height, TextureFormat textureFormat) :
		GLTexture(GL_TEXTURE_2D, TextureFormatToGLFormat[static_cast<int>(textureFormat)]) {
		glTextureStorage2D(id, 1, format, width, height);
		glTextureParameteri(id, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(id, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTextureParameteri(id, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTextureParameteri(id, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	}

	GLTexture2D::GLTexture2D(unsigned int width, unsigned int height, TextureFormat internalFormat, unsigned int dataFormat, unsigned int type, void* data) :
		GLTexture(GL_TEXTURE_2D, TextureFormatToGLFormat[static_cast<int>(internalFormat)]) {
		glTextureStorage2D(id, 1, format, width, height);
		glTextureSubImage2D(id, 0, 0, 0, width, height, dataFormat, type, data);
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
}