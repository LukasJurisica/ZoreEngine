#include "graphics/FrameBuffer.hpp"
#include "graphics/opengl/GLFrameBuffer.hpp"
#include "graphics/RenderEngine.hpp"
#include "debug/Debug.hpp"

namespace zore {

	//========================================================================
	//	Generic Frame Buffer Class
	//========================================================================

	FrameBuffer::~FrameBuffer() {
		delete textureArray;
	}

	FrameBuffer* FrameBuffer::Create(uint width, uint height, uint colorAttachmentCount, TextureFormat textureFormat, DepthFormat depthFormat) {
		DEBUG_ENSURE(colorAttachmentCount <= MAX_FRAMEBUFFER_COLOUR_ATTACHMENTS, "Failed to create framebuffer - Requested color attachment count is larger than the maximum.");

		switch (RenderEngine::GetAPI()) {
		case API::OPENGL:
			return new GLFrameBuffer(width, height, colorAttachmentCount, textureFormat, depthFormat);
		}
		throw ZORE_EXCEPTION("Invalid RenderAPI");
		return nullptr;
	}

	Texture2DArray* FrameBuffer::GetTextureArray() const {
		return textureArray;
	}
}