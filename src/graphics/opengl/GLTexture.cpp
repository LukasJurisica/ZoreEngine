#include "graphics/opengl/GLTexture.hpp"
#include "debug/Debug.hpp"
#include <glad/glad.h>

namespace zore {

	//========================================================================
	//	OpenGL Base Texture Class
	//========================================================================

	GLTexture::GLTexture(uint target, Texture::Format format) : unit(0) {
		static const uint GLSizedFormat[] = { GL_R8, GL_RG8, GL_RGB8, GL_RGBA8 };
		static const uint GLBaseFormat[] = { GL_RED, GL_RG, GL_RGB, GL_RGBA };

		uint formatIndex = static_cast<uint>(format);
		sizeFormat = GLSizedFormat[formatIndex];
		typeFormat = GLBaseFormat[formatIndex];
		glCreateTextures(target, 1, &id);
	}

	GLTexture::GLTexture(uint target, uint sizeFormat, uint typeFormat) : sizeFormat(sizeFormat), typeFormat(typeFormat), unit(0) {
		glCreateTextures(target, 1, &id);
	}

	GLTexture::~GLTexture() {
		glDeleteTextures(1, &id);
	}

	unsigned int GLTexture::GetID() {
		return id;
	}

	//========================================================================
	//	OpenGL Texture Sampler Class
	//========================================================================

	GLSampler::GLSampler(Texture::SampleMode mode) : unit(0) {
		static const unsigned int SamplerModeToGLMode[] = { GL_LINEAR, GL_NEAREST };
		glCreateSamplers(1, &id);
		glSamplerParameteri(id, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		glSamplerParameteri(id, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glSamplerParameteri(id, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glSamplerParameteri(id, GL_TEXTURE_MIN_LOD, 0);
		glSamplerParameteri(id, GL_TEXTURE_MAX_LOD, 0);
		glSamplerParameteri(id, GL_TEXTURE_MAG_FILTER, SamplerModeToGLMode[static_cast<int>(mode)]); // Close up texture
		//glSamplerParameteri(id, GL_TEXTURE_MIN_FILTER, SamplerModeToGLMode[static_cast<int>(mode)]); // Far away texture
	}

	GLSampler::~GLSampler() {
		glDeleteSamplers(1, &id);
	}

	void GLSampler::Bind() const {
		glBindSampler(unit, id);
	}

	void GLSampler::Bind(unsigned int slot) {
		unit = slot;
		glBindSampler(unit, id);
	}

	void GLSampler::Unbind() const {
		glBindSampler(unit, 0);
	}

	//========================================================================
	//	OpenGL 2D Texture Class
	//========================================================================

	GLTexture2D::GLTexture2D(uint width, uint height, Texture::Format textureFormat, void* data) : GLTexture(GL_TEXTURE_2D, textureFormat) {
		Init(width, height);
		glTextureStorage2D(id, 1, sizeFormat, width, height);
		SetTextureData(data);
	}

	GLTexture2D::GLTexture2D(uint width, uint height, uint sizeFormat, uint typeFormat) : GLTexture(GL_TEXTURE_2D, sizeFormat, typeFormat) {
		Init(width, height);
		glTextureStorage2D(id, 1, sizeFormat, width, height);
	}

	void GLTexture2D::Bind() const {
		glBindTextureUnit(unit, id);
	}

	void GLTexture2D::Bind(uint slot) {
		unit = slot;
		glBindTextureUnit(unit, id);
	}

	void GLTexture2D::Unbind() const {
		glBindTextureUnit(unit, 0);
	}

	void GLTexture2D::SetSize(uint width, uint height) {
		Init(width, height);
		glDeleteTextures(1, &id);
		glCreateTextures(GL_TEXTURE_2D, 1, &id);
		glTextureStorage2D(id, 1, sizeFormat, width, height);
	}

	void GLTexture2D::SetTextureData(void* data) {
		if (data)
			glTextureSubImage2D(id, 0, 0, 0, width, height, typeFormat, GL_UNSIGNED_BYTE, data);
	}

	//========================================================================
	//	OpenGL 2D Texture Array Class
	//========================================================================

	GLTexture2DArray::GLTexture2DArray(uint width, uint height, uint layers, Texture::Format format, void* data, uint count) : GLTexture(GL_TEXTURE_2D_ARRAY, format) {
		Init(width, height, layers);
		glTextureStorage3D(id, 1, sizeFormat, width, height, layers);
		SetTextureData(0, count, data);
	}

	GLTexture2DArray::GLTexture2DArray(uint width, uint height, uint sizeFormat, uint typeFormat) : GLTexture(GL_TEXTURE_2D_ARRAY, sizeFormat, typeFormat) {
		Init(width, height, layers);
		glTextureStorage3D(id, 1, sizeFormat, width, height, layers);
	}

	void GLTexture2DArray::Bind() const {
		glBindTextureUnit(unit, id);
	}

	void GLTexture2DArray::Bind(uint slot) {
		unit = slot;
		glBindTextureUnit(unit, id);
	}

	void GLTexture2DArray::Unbind() const {
		glBindTextureUnit(unit, 0);
	}

	void GLTexture2DArray::SetSize(uint width, uint height, uint layers) {
		Init(width, height, layers);
		glDeleteTextures(1, &id);
		glCreateTextures(GL_TEXTURE_2D_ARRAY, 1, &id);
		glTextureStorage3D(id, 1, sizeFormat, width, height, layers);
	}

	void GLTexture2DArray::SetTextureData(uint first, uint count, void* data) {
		if (data && count > 0) {
			count = (first + count) <= layers ? count : layers - first;
			glTextureSubImage3D(id, 0, 0, 0, first, width, height, count, typeFormat, GL_UNSIGNED_BYTE, data);
		}
	}
}