#pragma once
#include "graphics/Texture.hpp"
#include <vector>

#define MAX_FRAMEBUFFER_COLOUR_ATTACHMENTS 8

namespace zore {

	//========================================================================
	//	Generic Frame Buffer Attachment Specifications
	//========================================================================

	enum class DepthFormat { DEPTH32_BUFFER, DEPTH24_STENCIL8_BUFFER, DEPTH32_TEXTURE, DEPTH24_STENCIL8_TEXTURE, NONE };

	//========================================================================
	//	Generic Frame Buffer Class
	//========================================================================

	class FrameBuffer {
	public:
		static FrameBuffer* Create(uint width, uint height, uint colorAttachmentCount = 1, Texture::Format textureFormat = Texture::Format::RGBA, DepthFormat depthFormat = DepthFormat::DEPTH32_BUFFER);
		virtual ~FrameBuffer();

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;
		virtual void SetSize(uint width, uint height) = 0;
		Texture2DArray* GetTextureArray() const;
		Texture2D* GetDepthTexture() const;

	protected:
		Texture2DArray* textureArray = nullptr;
		Texture2D* depthTexture = nullptr;
		uint textureArrayLayerCount = 1;
	};
}