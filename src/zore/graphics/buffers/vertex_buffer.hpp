#pragma once

#include "zore/graphics/buffers/buffer_base.hpp"

namespace zore {

	//========================================================================
	//	Vertex Buffer
	//========================================================================

	class VertexBuffer : public Buffer::Base {
	public:
		VertexBuffer(bool instanced = false);
		VertexBuffer(const void_span& span, bool instanced = false);
		VertexBuffer(const void* data, size_t size, size_t stride = 0u, bool instanced = false);
		VertexBuffer(VertexBuffer&&) = default;
		VertexBuffer& operator=(VertexBuffer&&) = default;
		~VertexBuffer() = default;

		void Set(const void_span& span);
		void Set(const void* data, size_t size, size_t stride);
		void Bind() const;

	private:
		uint32_t m_stride;
		uint32_t m_instance;
	};
}