#include "zore/graphics/buffer.hpp"
#include "zore/debug.hpp"
#include <vulkan/vulkan.h>

namespace zore {

	static constexpr uint32_t INVALID_BUFFER_ID = ~0;

	//========================================================================
	//	OpenGL Vertex Buffer
	//========================================================================

	VertexBuffer::VertexBuffer(bool instanced) : m_id(INVALID_BUFFER_ID), m_stride(0), m_index(instanced ? 1 : 0) {}

	VertexBuffer::VertexBuffer(const VoidSpan& span, bool instanced) : m_id(0), m_stride(static_cast<uint32_t>(span.ElementSize())), m_index(instanced ? 1 : 0) {

	}

	VertexBuffer::VertexBuffer(const void* data, uint32_t size, uint32_t stride, bool instanced) : m_id(0), m_stride(stride), m_index(instanced ? 1 : 0) {

	}

	VertexBuffer::~VertexBuffer() {
		
	}

	uint32_t VertexBuffer::GetID() const {
		return m_id;
	}

	void VertexBuffer::Set(const VoidSpan& span) {

	}

	void VertexBuffer::Set(const void* data, uint32_t size, uint32_t stride) {

	}

	void VertexBuffer::Update(const void* data, uint32_t size, uint32_t offset) {

	}

	void VertexBuffer::Bind() const {

	}
}