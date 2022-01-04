#include "graphics/FrameBuffer.hpp"
#include "graphics/opengl/GLFrameBuffer.hpp"
#include "graphics/RenderEngine.hpp"
#include "debug/Debug.hpp"

namespace zore {

	//========================================================================
	//	Generic Frame Buffer Class
	//========================================================================

	FrameBuffer::~FrameBuffer() {
		for (Texture2D* tex : attachments)
			delete tex;
	}

	FrameBuffer* FrameBuffer::Create(uint width, uint height, uint colorAttachmentCount, DepthFormat format) {
		DEBUG_ENSURE(colorAttachmentCount <= MAX_FRAMEBUFFER_COLOUR_ATTACHMENTS, "Failed to create framebuffer - Texture attachment count is larger than the maximum.");

		switch (RenderEngine::GetAPI()) {
		case API::OPENGL:
			return new GLFrameBuffer(width, height, colorAttachmentCount, format);
		}
		throw ZORE_EXCEPTION("Invalid RenderAPI");
		return nullptr;
	}

	Texture2D* FrameBuffer::GetTexture(uint index) const {
		DEBUG_ENSURE(index < attachments.size(), "Texture index larger than attachment count.");
		return attachments[index];
	}

	uint FrameBuffer::GetNumTextureAttachments() {
		return attachments.size();
	}
}