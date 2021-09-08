#pragma once
#include "graphics/FrameBuffer.hpp"

namespace zore {

	class GLFrameBuffer : public FrameBuffer {
	public:
		GLFrameBuffer(unsigned int width, unsigned int height, const std::vector<FrameBufferAttachmentSpecification>& attachments);
		~GLFrameBuffer();

		void Bind() const override;
		void Unbind() const override;
		void Resize(unsigned int width, unsigned int height) override;

	private:
		unsigned int frameBufferID;
	};
}