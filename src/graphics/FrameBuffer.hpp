#pragma once
#include "graphics/Texture.hpp"
#include <vector>

#define MAX_FRAMEBUFFER_COLOUR_ATTACHMENTS 8

namespace zore {

	//========================================================================
	//	Generic Frame Buffer Attachment Specifications
	//========================================================================

	enum class DepthFormat { NONE, DEPTH32_TEXTURE, DEPTH24_STENCIL8_TEXTURE, DEPTH32_BUFFER, DEPTH24_STENCIL8_BUFFER };

	//========================================================================
	//	Generic Frame Buffer Class
	//========================================================================

	class FrameBuffer {
	public:
		static FrameBuffer* Create(unsigned int width, unsigned int height, unsigned int colorAttachmentCount, DepthFormat format = DepthFormat::NONE);
		virtual ~FrameBuffer();

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;
		virtual void SetSize(unsigned int width, unsigned int height) = 0;
		const Texture2D* GetTexture(unsigned int index) const;

	protected:
		std::vector<Texture2D*> attachments;
	};
}