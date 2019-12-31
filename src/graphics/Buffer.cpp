#include "graphics/Buffer.hpp"
#include "graphics/opengl/OpenGLBuffer.hpp"
#include "graphics/RenderEngine.hpp"

///////////////////
// Vertex Buffer //
///////////////////

VertexBuffer* VertexBuffer::create(float* vertices, uint32_t size) {
	switch (RenderEngine::getAPI()) {
	case RenderAPI::None:
		Logger::error("NULL RenderAPI is not currently supported"); return nullptr;
	case RenderAPI::OpenGL:
		return new OpenGLVertexBuffer(vertices, size);
	default:
		Logger::error("Invalid RenderAPI"); return nullptr;
	}
}

//////////////////
// Index Buffer //
//////////////////

IndexBuffer* IndexBuffer::create(uint32_t* indices, uint32_t count) {
	switch (RenderEngine::getAPI()) {
	case RenderAPI::None:
		Logger::error("NULL RenderAPI is not currently supported"); return nullptr;
	case RenderAPI::OpenGL:
		return new OpenGLIndexBuffer(indices, count);
	default:
		Logger::error("Invalid RenderAPI"); return nullptr;
	}
}

///////////////////
// Buffer Layout //
///////////////////

BufferLayout::BufferLayout() : m_stride(0) {}

BufferLayout::BufferLayout(const std::initializer_list<BufferElement>& elements) : m_elements(elements) {
	m_stride = 0;
	for (BufferElement& element : m_elements) {
		element.offset = m_stride;
		m_stride += element.size;
	}
}