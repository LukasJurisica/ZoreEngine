#pragma once
#include "graphics/Texture.hpp"

namespace zore {

	//========================================================================
	//	OpenGL Base Texture Class
	//========================================================================

	// Channel Count to OpenGL Texture Type
	unsigned int CCtoGLTT(unsigned int channels);

	class GLTexture {
	public:
		GLTexture(unsigned int type, unsigned int channels);
		virtual ~GLTexture();
		unsigned int GetTextureID();

	protected:
		unsigned int type;
		unsigned int channels;
		unsigned int textureID;
	};

	//========================================================================
	//	OpenGL 2D Texture Class
	//========================================================================

	class GLTexture2D : public GLTexture, public Texture2D {
	public:
		GLTexture2D(const std::string& name);
		GLTexture2D(unsigned int width, unsigned int height, unsigned int channels);
		~GLTexture2D() = default;

		void Bind() const override;
		void Unbind() const override;
		void Resize(unsigned int width, unsigned int height) override;
	};
}