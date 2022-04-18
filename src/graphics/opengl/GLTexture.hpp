#pragma once
#include "graphics/Texture.hpp"

namespace zore {

	//========================================================================
	//	OpenGL Base Texture Class
	//========================================================================

	class GLTexture {
	public:
		GLTexture(unsigned int target, TextureFormat textureFormat);
		virtual ~GLTexture();
		void SetTextureParameters();
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
		GLTexture2D(const std::string& filename, TextureFormat textureFormat);
		GLTexture2D(unsigned int width, unsigned int height, TextureFormat textureFormat, void* data = nullptr);
		~GLTexture2D() = default;
		
		void Bind() const override;
		void Unbind() const override;
		void SetSize(uint width, uint height) override;
		void SetTextureSlot(unsigned int slot) override;
		void SetTextureData(void* data) override;
	};

	//========================================================================
	//	OpenGL 2D Texture Array Class
	//========================================================================

	class GLTexture2DArray : public Texture2DArray, public GLTexture {
	public:
		GLTexture2DArray(const std::vector<std::string>& filenames, TextureFormat textureFormat);
		GLTexture2DArray(uint width, uint height, uint layers, void* data, TextureFormat textureFormat);
		~GLTexture2DArray() = default;

		void Bind() const override;
		void Unbind() const override;
		void SetSize(uint width, uint height, uint layers) override;
		void SetTextureSlot(unsigned int slot) override;
		void SetTextureData(uint first, uint count, void* data) override;
	};
}