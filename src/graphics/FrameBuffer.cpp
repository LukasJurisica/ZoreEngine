#include "graphics/FrameBuffer.hpp"
#include "graphics/opengl/GLFrameBuffer.hpp"
#include "graphics/RenderEngine.hpp"
#include "utils/DataTypes.hpp"
#include "debug/Debug.hpp"

namespace zore {

	//========================================================================
	//	Generic Frame Buffer Class
	//========================================================================

	FrameBuffer::~FrameBuffer() {
		for (Texture2D* tex : attachments)
			delete tex;
	}

	FrameBuffer* FrameBuffer::Create(unsigned int width, unsigned int height, unsigned int colorAttachmentCount, DepthFormat format) {
		ENSURE(colorAttachmentCount <= MAX_FRAMEBUFFER_COLOUR_ATTACHMENTS, "Failed to create framebuffer - Texture attachment count has exceeded the maximum.");

		switch (RenderEngine::GetAPI()) {
		case API::OPENGL:
			return new GLFrameBuffer(width, height, colorAttachmentCount, format);
		}
		throw ZORE_EXCEPTION("Invalid RenderAPI");
		return nullptr;
	}

	const Texture2D* FrameBuffer::GetTexture(unsigned int index) const {
		ENSURE(index < attachments.size(), "Texture index larger than attachment count.");
		return attachments[index];
	}
}