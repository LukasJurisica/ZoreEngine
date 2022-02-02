#pragma once
#include "graphics/Texture.hpp"

namespace zore {

	//========================================================================
	//	OpenGL Base Texture Class
	//========================================================================

	class GLTexture {
	public:
		GLTexture(unsigned int target, unsigned int textureFormat);
		virtual ~GLTexture();
		unsigned int GetID();

	protected:
		unsigned int format;
		unsigned int unit;
		unsigned int id;
	};

	//========================================================================
	//	OpenGL 2D Texture Class
	//========================================================================

	class GLTexture2D : public Texture2D, public GLTexture {
	public:
		GLTexture2D(const std::string& name, TextureFormat textureFormat);
		GLTexture2D(unsigned int width, unsigned int height, TextureFormat textureFormat, void* data = nullptr);
		~GLTexture2D() = default;
		
		void Bind() const override;
		void Unbind() const override;
		void SetTextureSlot(unsigned int slot) override;
	};

	//========================================================================
	//	OpenGL 2D Texture Array Class
	//========================================================================

	class GLTexture2DArray : public Texture2DArray, public GLTexture {
	public:
		GLTexture2DArray(uint width, uint height, const std::vector<std::string>& filenames, TextureFormat textureFormat = TextureFormat::RGBA);
		GLTexture2DArray(uint width, uint height, uint layers, void* data = nullptr, TextureFormat textureFormat = TextureFormat::RGBA);
		~GLTexture2DArray() = default;

		void Bind() const override;
		void Unbind() const override;
		void SetTextureSlot(unsigned int slot) override;
		void SetTextureData(uint first, uint count, void* data) override;
	};
}