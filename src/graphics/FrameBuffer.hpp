#pragma once
#include "graphics/Texture.hpp"
#include <vector>

#define MAX_FRAMEBUFFER_COLOUR_ATTACHMENTS 8

namespace zore {

	//========================================================================
	//	Generic Frame Buffer Attachment Specifications
	//========================================================================

	enum class DepthFormat { NONE, DEPTH32, DEPTH24_STENCIL8 };

	//========================================================================
	//	Generic Frame Buffer Class
	//========================================================================

	class FrameBuffer {
	public:
		static FrameBuffer* Create(uint width, uint height, uint colorAttachmentCount = 1, DepthFormat format = DepthFormat::NONE);
		virtual ~FrameBuffer();

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;
		virtual void SetSize(uint width, uint height) = 0;
		Texture2D* GetTexture(uint index) const;
		uint GetNumTextureAttachments();

	protected:
		std::vector<Texture2D*> attachments;
	};
}