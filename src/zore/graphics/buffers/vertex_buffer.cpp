#include "zore/graphics/buffers/vertex_buffer.hpp"
#include <glad/glad.h>

namespace zore {

	//========================================================================
	//	Vertex Buffer
	//========================================================================

	VertexBuffer::VertexBuffer(bool instanced) : m_stride(0), m_instance(instanced ? 1 : 0) {}

	VertexBuffer::VertexBuffer(const void_span& span, bool instanced) {
		Set(span);
	}

	VertexBuffer::VertexBuffer(const void* data, size_t size, size_t stride, bool instanced) : m_instance(instanced ? 1 : 0) {
		Set(data, size, stride);
	}

	void VertexBuffer::Set(const void_span& span) {
		Set(span.data(), span.size_bytes(), span.element_size());
	}

	void VertexBuffer::Set(const void* data, size_t size, size_t stride) {
		m_stride = stride;
		Base::Set(data, size);
	}

	void VertexBuffer::Bind() const {
		glBindVertexBuffer(m_instance, GetID(), 0, m_stride);
	}
}