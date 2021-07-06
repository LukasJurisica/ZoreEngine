#include "graphics/Buffer.hpp"
#include "graphics/opengl/GLBuffer.hpp"
#include "graphics/RenderEngine.hpp"
#include "debug/Debug.hpp"

namespace zore {

	//========================================================================
	//	Platform Agnostic Vertex Buffer Class
	//========================================================================

	VertexBuffer* VertexBuffer::Create(void* vertices, unsigned int size) {
		switch (RenderEngine::GetApi()) {
		case API::OpenGL:
			return new GLVertexBuffer(vertices, size);
		}
		throw ZORE_EXCEPTION("Invalid RenderAPI");
		return nullptr;
	}

	//========================================================================
	//	Platform Agnostic Index Buffer Class
	//========================================================================

	IndexBuffer* IndexBuffer::Create(void* indices, unsigned int size) {
		switch (RenderEngine::GetApi()) {
		case API::OpenGL:
			return new GLIndexBuffer(indices, size);
		}
		throw ZORE_EXCEPTION("Invalid RenderAPI");
		return nullptr;
	}

	//========================================================================
	//	Platform Agnostic Shader Storage Buffer Class
	//========================================================================

	ShaderStorageBuffer* ShaderStorageBuffer::Create(void* data, unsigned int size, unsigned int index) {
		switch (RenderEngine::GetApi()) {
		case API::OpenGL:
			return new GLShaderStorageBuffer(data, size, index);
		}
		throw ZORE_EXCEPTION("Invalid RenderAPI");
		return nullptr;
	}

	//========================================================================
	//	Platform Agnostic Uniform Buffer Class
	//========================================================================

	UniformBuffer* UniformBuffer::Create(void* data, unsigned int size, unsigned int index) {
		switch (RenderEngine::GetApi()) {
		case API::OpenGL:
			return new GLUniformBuffer(data, size, index);
		}
		throw ZORE_EXCEPTION("Invalid RenderAPI");
		return nullptr;
	}

	//========================================================================
	//	Platform Agnostic Vertex Buffer Layout Class
	//========================================================================

	std::vector<BufferLayout*> layouts;

	BufferElement::BufferElement(std::string name, ShaderDataType type, unsigned int count, bool normalized) :
		type(type), count(count), name(name), normalized(normalized), offset(0), size(ShaderDataTypeSize(type, count)), attribLocation(-1) {
		ENSURE(count < 5 && count > 0, "Invalid number of components for BufferElement");
	}

	BufferLayout::BufferLayout(const std::string& name, const std::vector<BufferElement>& elements) :
		name(name), elements(elements), stride(0) {
		layouts.push_back(this);
	}

	BufferLayout::~BufferLayout() {
		auto iter = std::find(layouts.begin(), layouts.end(), this);
		if (iter != layouts.end())
			layouts.erase(iter);
	}

	BufferLayout* BufferLayout::Create(const std::string& name, Shader* shader, const std::vector<BufferElement>& elements) {
		switch (RenderEngine::GetApi()) {
		case API::OpenGL:
			return new GLBufferLayout(name, shader, elements);
		}
		throw ZORE_EXCEPTION("Invalid RenderAPI");
		return nullptr;
	}

	BufferLayout* BufferLayout::Get(const std::string& name) {
		for (BufferLayout* layout : layouts) {
			if (layout->name == name)
				return layout;
		}
		throw ZORE_EXCEPTION("No vertex buffer layout with the name: " + name);
	}

	unsigned int BufferLayout::GetStride() const {
		return stride;
	}
}