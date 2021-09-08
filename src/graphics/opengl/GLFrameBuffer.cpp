#include "graphics/opengl/GLFrameBuffer.hpp"
#include "graphics/opengl/GLTexture.hpp"
#include "debug/Debug.hpp"
#include <glad/glad.h>

namespace zore {

	GLFrameBuffer::GLFrameBuffer(unsigned int width, unsigned int height, const std::vector<FrameBufferAttachmentSpecification>& attachments) : FrameBuffer(width, height) {
		glCreateFramebuffers(1, &frameBufferID);
		glBindFramebuffer(GL_FRAMEBUFFER, frameBufferID);

		unsigned int activeAttachments[MAX_FRAMEBUFFER_COLOUR_ATTACHMENTS];
		int attachmentCount = static_cast<int> (attachments.size());
		for (int i = 0; i < attachmentCount; i++) {
			GLTexture2D* tex = new GLTexture2D(width, height, attachments[i].channels);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, tex->GetTextureID(), 0);
			activeAttachments[i] = GL_COLOR_ATTACHMENT0 + i;
			this->attachments.push_back(tex);
		}
		glDrawBuffers(attachmentCount, activeAttachments);
	}

	GLFrameBuffer::~GLFrameBuffer() {
		glDeleteFramebuffers(1, &frameBufferID);
	}

	void GLFrameBuffer::Bind() const {
		glBindFramebuffer(GL_FRAMEBUFFER, frameBufferID);
	}

	void GLFrameBuffer::Unbind() const {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void GLFrameBuffer::Resize(unsigned int width, unsigned int height) {
		this->width = width;
		this->height = height;

		for (Texture2D* tex : attachments)
			tex->Resize(width, height);
	}
}