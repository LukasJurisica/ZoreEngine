#pragma once
#include "graphics/Texture.hpp"

namespace zore {

	//========================================================================
	//	OpenGL Base Texture Class
	//========================================================================

	class GLTexture {
	public:
		GLTexture(uint target, Texture::Format format);
		GLTexture(uint target, uint sizeFormat, uint typeFormat);
		virtual ~GLTexture();
		uint GetID();

	protected:
		uint sizeFormat;
		uint typeFormat;
		uint unit;
		uint id;
	};

	//========================================================================
	//	OpenGL Texture Sampler Class
	//========================================================================

	class GLSampler : public Texture::Sampler {
	public:
		GLSampler(Texture::SampleMode mode);
		~GLSampler();

		void Bind() const override;
		void Bind(unsigned int slot) override;
		void Unbind() const override;

	private:
		unsigned int id;
		unsigned int unit;
	};

	//========================================================================
	//	OpenGL 2D Texture Class
	//========================================================================

	class GLTexture2D : public Texture2D, public GLTexture {
	public:
		GLTexture2D(uint width, uint height, Texture::Format format, void* data);
		GLTexture2D(uint width, uint height, uint sizeFormat, uint typeFormat);
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
		GLTexture2DArray(uint width, uint height, uint layers, Texture::Format format, void* data, uint count);
		GLTexture2DArray(uint width, uint height, uint sizeFormat, uint typeFormat);
		~GLTexture2DArray() = default;

		void Bind() const override;
		void Bind(uint slot) override;
		void Unbind() const override;
		void SetSize(uint width, uint height, uint layers) override;
		void SetTextureData(uint first, uint count, void* data) override;
	};
}