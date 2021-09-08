#include "graphics/FrameBuffer.hpp"
#include "graphics/opengl/GLFrameBuffer.hpp"
#include "graphics/RenderEngine.hpp"
#include "utils/DataTypes.hpp"
#include "debug/Debug.hpp"

namespace zore {

	//========================================================================
	//	Generic Frame Buffer Class
	//========================================================================

	FrameBuffer::FrameBuffer(unsigned int width, unsigned int height) : width(width), height(height) {}

	FrameBuffer* FrameBuffer::Create(unsigned int width, unsigned int height, const std::vector<FrameBufferAttachmentSpecification>& attachments) {

		Byte colourCount = 0;
		for (const FrameBufferAttachmentSpecification& attachment : attachments)
			colourCount += (attachment.type == BufferBit::COLOUR);
		ENSURE(colourCount <= MAX_FRAMEBUFFER_COLOUR_ATTACHMENTS, "Failed to create framebuffer - Colour Buffer Attachment count has exceeded the maximum.");

		switch (RenderEngine::GetAPI()) {
		case API::OpenGL:
			return new GLFrameBuffer(width, height, attachments);
		}
		throw ZORE_EXCEPTION("Invalid RenderAPI");
		return nullptr;
	}

	const Texture2D* FrameBuffer::GetTexture(unsigned int index) {
		return attachments[index];
	}
}