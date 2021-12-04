#include "graphics/opengl/GLFrameBuffer.hpp"
#include "graphics/opengl/GLTexture.hpp"
#include "debug/Debug.hpp"
#include <glad/glad.h>

namespace zore {

	//========================================================================
	//	OpenGL Render Buffer Class
	//========================================================================

	GLRenderBuffer::GLRenderBuffer(unsigned int width, unsigned int height, unsigned int format) : format(format) {
		glCreateRenderbuffers(1, &id);
		glNamedRenderbufferStorage(id, format, width, height);
	}

	GLRenderBuffer::~GLRenderBuffer() {
		glDeleteRenderbuffers(1, &id);
	}

	void GLRenderBuffer::SetSize(unsigned int width, unsigned int height) {
		glNamedRenderbufferStorage(id, format, width, height);
	}

	unsigned int GLRenderBuffer::GetID() const {
		return id;
	}

	//========================================================================
	//	OpenGL Frame Buffer Class
	//========================================================================

	GLFrameBuffer::GLFrameBuffer(unsigned int width, unsigned int height, unsigned int colorAttachmentCount, DepthFormat depthFormat) : rbo(nullptr) {
		glCreateFramebuffers(1, &id);

		// Create colour attachments
		GLTexture2D* tex;
		unsigned int activeAttachments[MAX_FRAMEBUFFER_COLOUR_ATTACHMENTS];
		for (unsigned int i = 0; i < colorAttachmentCount; i++) {
			glActiveTexture(GL_TEXTURE0 + i);
			tex = new GLTexture2D(width, height, GL_RGBA);
			glNamedFramebufferTexture(id, GL_COLOR_ATTACHMENT0 + i, tex->GetID(), 0);
			activeAttachments[i] = GL_COLOR_ATTACHMENT0 + i;
			attachments.push_back(tex);
		}
		glNamedFramebufferDrawBuffers(id, colorAttachmentCount, activeAttachments);

		// Create Depth/Stencil Buffer (If requested)
		if (depthFormat == DepthFormat::DEPTH32) {
			rbo = new GLRenderBuffer(width, height, GL_DEPTH_COMPONENT32);
			glNamedFramebufferRenderbuffer(id, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbo->GetID());
		}
		else if (depthFormat == DepthFormat::DEPTH24_STENCIL8) {
			rbo = new GLRenderBuffer(width, height, GL_DEPTH24_STENCIL8);
			glNamedFramebufferRenderbuffer(id, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo->GetID());
		}
		
		// Ensure FrameBuffer was created successfully
		unsigned int retval = glCheckNamedFramebufferStatus(id, GL_FRAMEBUFFER);
		ENSURE(retval == GL_FRAMEBUFFER_COMPLETE, "Error creating FrameBuffer. Error code: " + std::to_string(glCheckNamedFramebufferStatus(id, GL_FRAMEBUFFER)));
	}

	GLFrameBuffer::~GLFrameBuffer() {
		glDeleteFramebuffers(1, &id);
	}

	void GLFrameBuffer::Bind() const {
		glBindFramebuffer(GL_FRAMEBUFFER, id);
	}

	void GLFrameBuffer::Unbind() const {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void GLFrameBuffer::SetSize(unsigned int width, unsigned int height) {
		for (int i = 0; i < attachments.size(); i++)
			attachments[i]->SetSize(width, height);
		if (rbo)
			rbo->SetSize(width, height);
	}
}