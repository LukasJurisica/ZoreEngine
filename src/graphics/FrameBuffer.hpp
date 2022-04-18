#pragma once
#include "graphics/Texture.hpp"
#include <vector>

#define MAX_FRAMEBUFFER_COLOUR_ATTACHMENTS 8

namespace zore {

	//========================================================================
	//	Generic Frame Buffer Attachment Specifications
	//========================================================================

	enum class DepthFormat { DEPTH32, DEPTH24_STENCIL8, NONE };

	//========================================================================
	//	Generic Frame Buffer Class
	//========================================================================

	class FrameBuffer {
	public:
		static FrameBuffer* Create(uint width, uint height, uint colorAttachmentCount = 1, TextureFormat textureFormat = TextureFormat::RGBA, DepthFormat depthFormat = DepthFormat::DEPTH32);
		virtual ~FrameBuffer();

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;
		virtual void SetSize(uint width, uint height) = 0;
		Texture2DArray* GetTextureArray() const;

	protected:
		Texture2DArray* textureArray = nullptr;
	};
}