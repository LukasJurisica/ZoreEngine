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

		GLTexture2D* tex;
		// Create colour attachments
		unsigned int activeAttachments[MAX_FRAMEBUFFER_COLOUR_ATTACHMENTS];
		for (unsigned int i = 0; i < colorAttachmentCount; i++) {
			glActiveTexture(GL_TEXTURE0 + i);
			tex = new GLTexture2D(width, height, GL_RGBA);
			glNamedFramebufferTexture(id, GL_COLOR_ATTACHMENT0 + i, tex->GetID(), 0);
			attachments.push_back(tex);
		}

		// Create Depth/Stencil attachments (if applicable)
		switch (depthFormat) {
		case DepthFormat::DEPTH32_TEXTURE:
			glActiveTexture(GL_TEXTURE0 + colorAttachmentCount);
			tex = new GLTexture2D(width, height, GL_DEPTH_COMPONENT32, GL_DEPTH_COMPONENT, GL_UNSIGNED_INT);
			glNamedFramebufferTexture(id, GL_DEPTH_ATTACHMENT, tex->GetID(), 0);
			attachments.push_back(tex);
			break;
		case DepthFormat::DEPTH24_STENCIL8_TEXTURE:
			glActiveTexture(GL_TEXTURE0 + colorAttachmentCount);
			tex = new GLTexture2D(width, height, GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8);
			glNamedFramebufferTexture(id, GL_DEPTH_STENCIL_ATTACHMENT, tex->GetID(), 0);
			attachments.push_back(tex);
			break;
		case DepthFormat::DEPTH32_BUFFER:
			rbo = new GLRenderBuffer(width, height, GL_DEPTH_COMPONENT32);
			glNamedFramebufferRenderbuffer(id, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbo->GetID());
			break;
		case DepthFormat::DEPTH24_STENCIL8_BUFFER:
			rbo = new GLRenderBuffer(width, height, GL_DEPTH24_STENCIL8);
			glNamedFramebufferRenderbuffer(id, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo->GetID());
			break;
		}

		// Verify that the FrmaeBuffer has been created successfully
		ENSURE(glCheckNamedFramebufferStatus(id, GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Error creating FrameBuffer. Error code: " + std::to_string(glCheckNamedFramebufferStatus(id, GL_FRAMEBUFFER)));
		glNamedFramebufferDrawBuffers(id, colorAttachmentCount, activeAttachments);
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