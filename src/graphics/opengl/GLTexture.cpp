#include "graphics/opengl/GLTexture.hpp"
#include "stb/stb_image.h"
#include "debug/Debug.hpp"
#include <glad/glad.h>

namespace zore {

	//========================================================================
	//	OpenGL Base Texture Class
	//========================================================================

	static const unsigned int GLFormat[] = { GL_R8, GL_RG8, GL_RGB8, GL_RGBA8 };
	static const unsigned int InputDataFormat[] = { GL_RED, GL_RG, GL_RGB, GL_RGBA };

	GLTexture::GLTexture(unsigned int target, TextureFormat textureFormat)
		: format(static_cast<uint>(textureFormat)), unit(0) {
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

	GLTexture2D::GLTexture2D(const std::string& filename, TextureFormat textureFormat) : GLTexture(GL_TEXTURE_2D, textureFormat) {
		int w, h, c;
		ubyte* data = stbi_load(filename.c_str(), &w, &h, &c, format + 1);
		ENSURE(data, "Could not load texture: " + filename);
		Init(w, h);
		glTextureStorage2D(id, 1, GLFormat[format], width, height);
		glTextureSubImage2D(id, 0, 0, 0, width, height, InputDataFormat[format], GL_UNSIGNED_BYTE, data);
		stbi_image_free(data);
	}

	GLTexture2D::GLTexture2D(unsigned int width, unsigned int height, TextureFormat textureFormat, void* data) : GLTexture(GL_TEXTURE_2D, textureFormat) {
		Init(width, height);
		glTextureStorage2D(id, 1, GLFormat[format], width, height);
		if (data)
			SetTextureData(data);
	}

	void GLTexture2D::Bind() const {
		glBindTextureUnit(unit, id);
	}

	void GLTexture2D::Bind(unsigned int slot) {
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
		glTextureStorage2D(id, 1, GLFormat[format], width, height);
	}

	void GLTexture2D::SetTextureData(void* data) {
		DEBUG_ENSURE(data, "Nullptr passed as argument to GLTexture2D::SetTextureData.");
		glTextureSubImage2D(id, 0, 0, 0, width, height, InputDataFormat[format], GL_UNSIGNED_BYTE, data);
	}

	//========================================================================
	//	OpenGL 2D Texture Array Class
	//========================================================================
	
	GLTexture2DArray::GLTexture2DArray(const std::vector<std::string>& filenames, const std::string& root, TextureFormat textureFormat) : GLTexture(GL_TEXTURE_2D_ARRAY, textureFormat) {
		int w, h, c;
		for (int i = 0; i < filenames.size(); i++) {
			std::string path = root + filenames[i];
			ubyte* data = stbi_load(path.c_str(), &w, &h, &c, static_cast<int>(textureFormat) + 1);
			ENSURE(data, "Could not load texture: " + path);
			if (i == 0) {
				Init(w, h, static_cast<uint>(filenames.size()));
				glTextureStorage3D(id, 1, GLFormat[format], width, height, layers);
			}

			ENSURE(w == width && h == height, "The following texture cannot be added to the texture array as it has different dimensions: " + path);
			glTextureSubImage3D(id, 0, 0, 0, i, width, height, 1, InputDataFormat[format], GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);
		}
	}

	GLTexture2DArray::GLTexture2DArray(uint width, uint height, uint layers, void* data, TextureFormat textureFormat) : GLTexture(GL_TEXTURE_2D_ARRAY, textureFormat) {
		Init(width, height, layers);
		glTextureStorage3D(id, 1, GLFormat[format], width, height, layers);
		if (data)
			SetTextureData(0, layers, data);
	}

	void GLTexture2DArray::Bind() const {
		glBindTextureUnit(unit, id);
	}

	void GLTexture2DArray::Bind(unsigned int slot) {
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
		glTextureStorage3D(id, 1, GLFormat[format], width, height, layers);
	}

	void GLTexture2DArray::SetTextureData(uint first, uint count, void* data) {
		DEBUG_ENSURE(data, "Nullptr passed as argument to GLTexture2DArray::SetTextureData.");
		glTextureSubImage3D(id, 0, 0, 0, first, width, height, count, InputDataFormat[format], GL_UNSIGNED_BYTE, data);
	}
}