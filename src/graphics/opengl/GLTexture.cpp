#include "graphics/opengl/GLTexture.hpp"
#include "debug/Debug.hpp"
#include <glad/glad.h>

namespace zore {

	//========================================================================
	//	OpenGL Base Texture Class
	//========================================================================

	const unsigned int GLTexture::TextureFormatToGLFormat[] = { GL_RED, GL_RG, GL_RGB, GL_RGBA };

	GLTexture::GLTexture(unsigned int target, unsigned int format) :
		internalFormat(format), dataFormat(format), type(GL_UNSIGNED_BYTE) {
		
		glGenTextures(1, &id);
		glBindTexture(target, id);
	}

	GLTexture::GLTexture(unsigned int target, unsigned int internalFormat, unsigned int dataFormat, unsigned int type) :
		internalFormat(internalFormat), dataFormat(dataFormat), type(type) {
		
		glGenTextures(1, &id);
		glBindTexture(target, id);
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

	}

	GLTexture2D::GLTexture2D(unsigned int width, unsigned int height, unsigned int format) :
		GLTexture(GL_TEXTURE_2D, format) {
		
		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, dataFormat, type, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	}

	GLTexture2D::GLTexture2D(unsigned int width, unsigned int height, TextureFormat format) :
		GLTexture(GL_TEXTURE_2D, TextureFormatToGLFormat[static_cast<int>(format)]) {
		
		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, dataFormat, type, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	}

	GLTexture2D::GLTexture2D(unsigned int width, unsigned int height, unsigned int internalFormat, unsigned int dataFormat, unsigned int type, void* data) :
		GLTexture(GL_TEXTURE_2D, internalFormat, dataFormat, type) {

		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, dataFormat, type, data);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	}

	void GLTexture2D::Bind() const {
		glBindTexture(GL_TEXTURE_2D, id);
	}

	void GLTexture2D::Unbind() const {
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void GLTexture2D::SetSize(unsigned int width, unsigned int height) {
		glBindTexture(GL_TEXTURE_2D, id);
		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, dataFormat, type, nullptr);
	}
}