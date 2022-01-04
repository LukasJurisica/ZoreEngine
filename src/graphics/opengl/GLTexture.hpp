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
		GLTexture2D(const std::string& name);
		GLTexture2D(unsigned int width, unsigned int height, TextureFormat textureFormat);
		GLTexture2D(unsigned int width, unsigned int height, TextureFormat internalFormat, unsigned int dataFormat, unsigned int type, void* data = nullptr);
		~GLTexture2D() = default;
		
		void Bind() const override;
		void Unbind() const override;
		void SetTextureSlot(unsigned int slot) override;
	};
}