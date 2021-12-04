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
		static FrameBuffer* Create(unsigned int width, unsigned int height, unsigned int colorAttachmentCount = 1, DepthFormat format = DepthFormat::NONE);
		virtual ~FrameBuffer();

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;
		virtual void SetSize(unsigned int width, unsigned int height) = 0;
		const Texture2D* GetTexture(unsigned int index) const;

	protected:
		std::vector<Texture2D*> attachments;
	};
}