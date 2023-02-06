#include "graphics/opengl/GLFrameBuffer.hpp"
#include "graphics/opengl/GLTexture.hpp"
#include "debug/Debug.hpp"
#include <glad/glad.h>

#include <iostream>

namespace zore {

	//========================================================================
	//	OpenGL Render Buffer Class
	//========================================================================

	GLRenderBuffer::GLRenderBuffer(uint width, uint height, uint format) : format(format) {
		glCreateRenderbuffers(1, &id);
		glNamedRenderbufferStorage(id, format, width, height);
	}

	GLRenderBuffer::~GLRenderBuffer() {
		glDeleteRenderbuffers(1, &id);
	}

	void GLRenderBuffer::SetSize(uint width, uint height) {
		glNamedRenderbufferStorage(id, format, width, height);
	}

	uint GLRenderBuffer::GetID() const {
		return id;
	}

	//========================================================================
	//	OpenGL Frame Buffer Class
	//========================================================================
	
	static const unsigned int DepthFormatToGLDepthFormat[] = { GL_DEPTH_COMPONENT32, GL_DEPTH24_STENCIL8 };
	static const unsigned int DepthFormatToGLAttachmentType[] = { GL_DEPTH_ATTACHMENT, GL_DEPTH_STENCIL_ATTACHMENT };

	GLFrameBuffer::GLFrameBuffer(uint width, uint height, uint colourAttachmentCount, Texture::Format textureFormat, DepthFormat depthFormat) {

		glCreateFramebuffers(1, &id);
		textureArrayLayerCount = colourAttachmentCount;

		// Create colour attachments
		uint activeAttachments[MAX_FRAMEBUFFER_COLOUR_ATTACHMENTS];
		GLTexture2DArray* tex = new GLTexture2DArray(width, height, colourAttachmentCount, textureFormat, nullptr, 0);
		uint texID = tex->GetID();
		textureArray = tex;
		for (uint i = 0; i < colourAttachmentCount; i++) {
			activeAttachments[i] = GL_COLOR_ATTACHMENT0 + i;
			glNamedFramebufferTextureLayer(id, activeAttachments[i], texID, 0, i);
		}
		glNamedFramebufferDrawBuffers(id, colourAttachmentCount, activeAttachments);

		// Create Depth/Stencil Buffer (If requested)
		depthBuffer = nullptr;
		depthFormatIndex = static_cast<uint>(depthFormat);
		if (depthFormatIndex < 2) { // DEPTH32_BUFFER or DEPTH24_STENCIL8_BUFFER
			depthBuffer = new GLRenderBuffer(width, height, DepthFormatToGLDepthFormat[depthFormatIndex]);
			glNamedFramebufferRenderbuffer(id, DepthFormatToGLAttachmentType[depthFormatIndex], GL_RENDERBUFFER, depthBuffer->GetID());
		}
		else if (depthFormatIndex < 4) { // DEPTH32_TEXTURE or DEPTH24_STENCIL8_TEXTURE
			GLTexture2D* tex = new GLTexture2D(width, height, DepthFormatToGLDepthFormat[depthFormatIndex - 2], GL_DEPTH_COMPONENT);
			depthTexture = tex;
			glNamedFramebufferTexture(id, DepthFormatToGLAttachmentType[depthFormatIndex - 2], tex->GetID(), 0);
		}

		// Ensure FrameBuffer was created successfully
		unsigned int retval = glCheckNamedFramebufferStatus(id, GL_FRAMEBUFFER);
		ENSURE(retval == GL_FRAMEBUFFER_COMPLETE, "Error creating FrameBuffer. Error code: " + std::to_string(retval));
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

	void GLFrameBuffer::SetSize(uint width, uint height) {
		textureArray->SetSize(width, height, textureArrayLayerCount);
		textureArray->Bind();
		uint textureID = reinterpret_cast<GLTexture2DArray*>(textureArray)->GetID();
		for (uint i = 0; i < textureArrayLayerCount; i++)
			glNamedFramebufferTextureLayer(id, GL_COLOR_ATTACHMENT0 + i, textureID, 0, i);
		if (depthBuffer)
			depthBuffer->SetSize(width, height);
		else if (depthTexture) {
			depthTexture->SetSize(width, height);
			GLTexture2D* tex = reinterpret_cast<GLTexture2D*>(depthTexture);
			glNamedFramebufferTexture(id, DepthFormatToGLAttachmentType[depthFormatIndex - 2], tex->GetID(), 0);
		}
	}
}