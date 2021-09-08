#pragma once
#include "graphics/Texture.hpp"
#include <string>
#include <vector>

#define MAX_FRAMEBUFFER_COLOUR_ATTACHMENTS 8

namespace zore {

	//========================================================================
	//	Generic Frame Buffer Attachment Specifications
	//========================================================================

	enum class BufferBit {
		COLOUR        = 1 << 0,
		DEPTH         = 1 << 1,
		STENCIl       = 1 << 2,
		DEPTH_STENCIL = 1 << 3
	};

	struct FrameBufferAttachmentSpecification {
		unsigned int channels;
		BufferBit type;
		bool readOnly = false;
	};

	//========================================================================
	//	Generic Frame Buffer Class
	//========================================================================

	class FrameBuffer {
	public:
		static FrameBuffer* Create(unsigned int width, unsigned int height, const std::vector<FrameBufferAttachmentSpecification>& attachments);
		virtual ~FrameBuffer() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;
		virtual void Resize(unsigned int width, unsigned int height) = 0;
		//virtual Texture2D* GetDepthTexture();
		const Texture2D* GetTexture(unsigned int index);

	protected:
		FrameBuffer(unsigned int width, unsigned int height);

		std::vector<Texture2D*> attachments;
		unsigned int width;
		unsigned int height;
	};
}