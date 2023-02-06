#pragma once
#include "graphics/FrameBuffer.hpp"

namespace zore {

	//========================================================================
	//	OpenGL Render Buffer Class
	//========================================================================

	class GLRenderBuffer {
	public:
		GLRenderBuffer(uint width, uint height, uint format);
		~GLRenderBuffer();

		void SetSize(uint width, uint height);
		uint GetID() const;

	private:
		uint id;
		uint format;
	};

	//========================================================================
	//	OpenGL Frame Buffer Class
	//========================================================================

	class GLFrameBuffer : public FrameBuffer {
	public:
		GLFrameBuffer(uint width, uint height, uint colourAttachmentCount, Texture::Format textureFormat, DepthFormat depthFormat);
		~GLFrameBuffer();

		void Bind() const override;
		void Unbind() const override;
		void SetSize(uint width, uint height) override;

	private:
		uint id;
		GLRenderBuffer* depthBuffer;
		uint depthFormatIndex;
	};
}