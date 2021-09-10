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
		GLTexture(unsigned int target, unsigned int format);
		GLTexture(unsigned int target, unsigned int internalFormat, unsigned int dataFormat, unsigned int type);
		virtual ~GLTexture();
		unsigned int GetID();

	protected:
		unsigned int internalFormat;
		unsigned int dataFormat;
		unsigned int type;
		unsigned int id;
	};

	//========================================================================
	//	OpenGL 2D Texture Class
	//========================================================================

	class GLTexture2D : public Texture2D, public GLTexture {
	public:
		GLTexture2D(const std::string& name);
		GLTexture2D(unsigned int width, unsigned int height, unsigned int format);
		GLTexture2D(unsigned int width, unsigned int height, unsigned int internalFormat, unsigned int dataFormat, unsigned int type, void* data = nullptr);
		~GLTexture2D() = default;

		void Bind() const override;
		void Unbind() const override;
		void SetSize(unsigned int width, unsigned int height) override;
	};
}