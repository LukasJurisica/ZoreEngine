#pragma once
#include "graphics/FrameBuffer.hpp"

namespace zore {

	//========================================================================
	//	OpenGL Render Buffer Class
	//========================================================================

	class GLRenderBuffer {
	public:
		GLRenderBuffer(unsigned int width, unsigned int height, unsigned int format);
		~GLRenderBuffer();

		void SetSize(unsigned int width, unsigned int height);
		unsigned int GetID() const;

	private:
		unsigned int id;
		unsigned int format;
	};

	//========================================================================
	//	OpenGL Frame Buffer Class
	//========================================================================

	class GLFrameBuffer : public FrameBuffer {
	public:
		GLFrameBuffer(unsigned int width, unsigned int height, unsigned int colorAttachmentCount, DepthFormat depthFormat);
		~GLFrameBuffer();

		void Bind() const override;
		void Unbind() const override;
		void SetSize(unsigned int width, unsigned int height) override;

	private:
		unsigned int id;
		GLRenderBuffer* rbo;
	};
}