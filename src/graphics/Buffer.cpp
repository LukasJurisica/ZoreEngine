#include "graphics/Buffer.hpp"
#include "graphics/opengl/OpenGLBuffer.hpp"
#include "graphics/RenderAPI.hpp"

///////////////////
// Buffer Layout //
///////////////////

BufferLayout::BufferLayout() : m_stride(0) {}

BufferLayout::BufferLayout(const std::vector<BufferElement>& elements) : m_elements(elements) {
	m_stride = 0;
	for (BufferElement& element : m_elements) {
		element.offset = m_stride;
		m_stride += element.size;
	}
}

///////////////////
// Vertex Buffer //
///////////////////

VertexBuffer* VertexBuffer::create(float* vertices, uint32_t size) {
	switch (RenderAPI::getAPI()) {
	case RenderAPI::API::None:
		Logger::error("NULL RenderAPI is not currently supported"); return nullptr;
	case RenderAPI::API::OpenGL:
		return new OpenGLVertexBuffer(vertices, size);
	default:
		Logger::error("Invalid RenderAPI"); return nullptr;
	}
}

//////////////////
// Index Buffer //
//////////////////

IndexBuffer* IndexBuffer::create(uint32_t* indices, uint32_t count) {
	switch (RenderAPI::getAPI()) {
	case RenderAPI::API::None:
		Logger::error("NULL RenderAPI is not currently supported"); return nullptr;
	case RenderAPI::API::OpenGL:
		return new OpenGLIndexBuffer(indices, count);
	default:
		Logger::error("Invalid RenderAPI"); return nullptr;
	}
}

//////////////////
// Vertex Array //
//////////////////

VertexArray* VertexArray::create() {
	switch (RenderAPI::getAPI()) {
	case RenderAPI::API::None:
		Logger::error("NULL RenderAPI is not currently supported"); return nullptr;
	case RenderAPI::API::OpenGL:
		return new OpenGLVertexArray();
	default:
		Logger::error("Invalid RenderAPI"); return nullptr;
	}
}