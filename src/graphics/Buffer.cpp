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
		case API::OpenGL:
			return new GLVertexBuffer(data, size, stride);
		}
		throw ZORE_EXCEPTION("Invalid RenderAPI");
	}

	//========================================================================
	//	Platform Agnostic Index Buffer Class
	//========================================================================

	IndexBuffer* IndexBuffer::Create(void* data, unsigned int size) {
		switch (RenderEngine::GetAPI()) {
		case API::OpenGL:
			return new GLIndexBuffer(data, size);
		}
		throw ZORE_EXCEPTION("Invalid RenderAPI");
	}

	//========================================================================
	//	Platform Agnostic Shader Storage Buffer Class
	//========================================================================

	ShaderStorageBuffer* ShaderStorageBuffer::Create(void* data, unsigned int size, unsigned int index) {
		switch (RenderEngine::GetAPI()) {
		case API::OpenGL:
			return new GLShaderStorageBuffer(data, size, index);
		}
		throw ZORE_EXCEPTION("Invalid RenderAPI");
	}

	//========================================================================
	//	Platform Agnostic Uniform Buffer Class
	//========================================================================

	UniformBuffer* UniformBuffer::Create(void* data, unsigned int size, unsigned int index) {
		switch (RenderEngine::GetAPI()) {
		case API::OpenGL:
			return new GLUniformBuffer(data, size, index);
		}
		throw ZORE_EXCEPTION("Invalid RenderAPI");
	}

	//========================================================================
	//	Platform Agnostic Vertex Buffer Layout Class
	//========================================================================

	std::vector<VertexLayout*> layouts;
	const unsigned int VertexLayout::VertexDataTypeSize[] = { 1u, 1u, 1u, 4u, 4u };

	VertexElement::VertexElement(std::string name, VertexDataType type, unsigned int count, bool normalize) :
		type(type), count(count), name(name), normalize(normalize) {
		ENSURE(count < 5 && count > 0, "Invalid number of components for BufferElement");
	}

	VertexLayout::VertexLayout(const std::string& name) : name(name), stride(0) {
		layouts.push_back(this);
	}

	VertexLayout* VertexLayout::Create(const std::string& name, Shader* shader, const std::vector<VertexElement>& elements) {
		switch (RenderEngine::GetAPI()) {
		case API::OpenGL:
			return new GLVertexLayout(name, shader, elements);
		}
		throw ZORE_EXCEPTION("Invalid RenderAPI");
	}

	VertexLayout* VertexLayout::Get(const std::string& name) {
		for (VertexLayout* layout : layouts) {
			if (layout->name == name)
				return layout;
		}
		throw ZORE_EXCEPTION("No vertex buffer layout with the name: " + name);
	}

	VertexLayout::~VertexLayout() {
		auto iter = std::find(layouts.begin(), layouts.end(), this);
		if (iter != layouts.end())
			layouts.erase(iter);
	}

	unsigned int VertexLayout::GetStride() {
		return stride;
	}
}