#include "graphics/Buffer.hpp"
#include "graphics/opengl/GLBuffer.hpp"
#include "graphics/RenderEngine.hpp"
#include "debug/Debug.hpp"

namespace zore {

	//========================================================================
	//	Platform Agnostic Vertex Buffer Class
	//========================================================================

	VertexBuffer* VertexBuffer::Create(void* data, unsigned int size, unsigned int stride) {
		switch (RenderEngine::GetAPI()) {
		case API::OPENGL:
			return new GLVertexBuffer(data, size, stride);
		}
		throw ZORE_EXCEPTION("Invalid RenderAPI");
	}

	//========================================================================
	//	Platform Agnostic Index Buffer Class
	//========================================================================

	IndexBuffer* IndexBuffer::Create(void* data, unsigned int size) {
		switch (RenderEngine::GetAPI()) {
		case API::OPENGL:
			return new GLIndexBuffer(data, size);
		}
		throw ZORE_EXCEPTION("Invalid RenderAPI");
	}

	//========================================================================
	//	Platform Agnostic Instance Buffer Class
	//========================================================================

	InstanceArrayBuffer* InstanceArrayBuffer::Create(void* data, unsigned int size, unsigned int stride) {
		switch (RenderEngine::GetAPI()) {
		case API::OPENGL:
			return new GLInstanceArrayBuffer(data, size, stride);
		}
		throw ZORE_EXCEPTION("Invalid RenderAPI");
	}

	//========================================================================
	//	Platform Agnostic Shader Storage Buffer Class
	//========================================================================

	ShaderStorageBuffer* ShaderStorageBuffer::Create(void* data, unsigned int size, unsigned int index) {
		switch (RenderEngine::GetAPI()) {
		case API::OPENGL:
			return new GLShaderStorageBuffer(data, size, index);
		}
		throw ZORE_EXCEPTION("Invalid RenderAPI");
	}

	//========================================================================
	//	Platform Agnostic Uniform Buffer Class
	//========================================================================

	UniformBuffer* UniformBuffer::Create(void* data, unsigned int size, unsigned int index) {
		switch (RenderEngine::GetAPI()) {
		case API::OPENGL:
			return new GLUniformBuffer(data, size, index);
		}
		throw ZORE_EXCEPTION("Invalid RenderAPI");
	}
}