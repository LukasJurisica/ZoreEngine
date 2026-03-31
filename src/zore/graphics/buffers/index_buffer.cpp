#include "zore/graphics/buffers/index_buffer.hpp"
#include <glad/glad.h>

namespace zore {

	//========================================================================
	//	Index Buffer
	//========================================================================

	IndexBuffer::IndexBuffer(const void_span& span) : Base(span) {}

	IndexBuffer::IndexBuffer(const void* data, size_t size) : Base(data, size) {}

	void IndexBuffer::Bind() const {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, GetID());
	}
}