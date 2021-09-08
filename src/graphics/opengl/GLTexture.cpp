#include "graphics/opengl/GLTexture.hpp"
#include "debug/Debug.hpp"
#include <glad/glad.h>

namespace zore {

	//========================================================================
	//	OpenGL Base Texture Class
	//========================================================================

	unsigned int CCtoGLTT(unsigned int channels) {
		switch (channels) {
		case 1:
			return GL_RED;
		case 2:
			return GL_RG;
		case 3:
			return GL_RGB;
		case 4:
			return GL_RGBA;
		default:
			throw ZORE_EXCEPTION("Invalid number of channels for texture creation");
			return 0;
		}
	}

	GLTexture::GLTexture(unsigned int type, unsigned int channels) : type(type), channels(channels) {
		//glCreateTextures(type, 1, &textureID);
		glGenTextures(1, &textureID);
		glBindTexture(type, textureID);
	}

	GLTexture::~GLTexture() {
		glDeleteTextures(1, &textureID);
	}

	unsigned int GLTexture::GetTextureID() {
		return textureID;
	}

	//========================================================================
	//	OpenGL 2D Texture Class
	//========================================================================

	GLTexture2D::GLTexture2D(const std::string& name) : GLTexture(GL_TEXTURE_2D, 0) {

	}

	GLTexture2D::GLTexture2D(unsigned int width, unsigned int height, unsigned int channels) : GLTexture(GL_TEXTURE_2D, channels) {
		glTexImage2D(GL_TEXTURE_2D, 0, CCtoGLTT(channels), width, height, 0, GL_RGB, GL_FLOAT, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	}

	void GLTexture2D::Bind() const {
		glBindTexture(GL_TEXTURE_2D, textureID);
	}

	void GLTexture2D::Unbind() const {
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void GLTexture2D::Resize(unsigned int width, unsigned int height) {
		//GL_PROXY_TEXTURE_2D;
		// Maybe use ^ if pixels (last param) is null
		glTexImage2D(GL_TEXTURE_2D, 0, CCtoGLTT(channels), width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
	}
}