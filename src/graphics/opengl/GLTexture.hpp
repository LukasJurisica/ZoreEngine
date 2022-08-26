#pragma once
#include "graphics/Texture.hpp"

namespace zore {

	//========================================================================
	//	OpenGL Base Texture Class
	//========================================================================

	enum class GLTextureFormat { DepthTexture };

	class GLTexture {
	public:
		GLTexture(uint target, TextureFormat textureFormat);
		GLTexture(uint target, uint sizedTextureFormat, uint baseTextureFormat);
		virtual ~GLTexture();
		uint GetID();

	protected:
		uint sizedFormat;
		uint baseFormat;
		uint unit;
		uint id;
	};

	//========================================================================
	//	OpenGL 2D Texture Class
	//========================================================================

	class GLTexture2D : public Texture2D, public GLTexture {
	public:
		GLTexture2D(const std::string& filename, TextureFormat textureFormat);
		GLTexture2D(uint width, uint height, TextureFormat textureFormat, void* data = nullptr);
		GLTexture2D(uint width, uint height, uint sizedTextureFormat, uint baseTextureFormat);
		~GLTexture2D() = default;
		
		void Bind() const override;
		void Bind(uint slot) override;
		void Unbind() const override;
		void SetSize(uint width, uint height) override;
		void SetTextureData(void* data) override;
	};

	//========================================================================
	//	OpenGL 2D Texture Array Class
	//========================================================================

	class GLTexture2DArray : public Texture2DArray, public GLTexture {
	public:
		GLTexture2DArray(const std::vector<std::string>& filenames, const std::string& root, TextureFormat textureFormat);
		GLTexture2DArray(uint width, uint height, uint layers, void* data, TextureFormat textureFormat);
		GLTexture2DArray(uint width, uint height, uint sizedTextureFormat, uint baseTextureFormat);
		~GLTexture2DArray() = default;

		void Bind() const override;
		void Bind(uint slot) override;
		void Unbind() const override;
		void SetSize(uint width, uint height, uint layers) override;
		void SetTextureData(uint first, uint count, void* data) override;
	};
}