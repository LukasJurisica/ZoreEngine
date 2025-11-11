#pragma once

#include "zore/graphics/graphics_core.hpp"
#include "zore/utils/span.hpp"

namespace zore {

	//========================================================================
	//	Vertex Buffer
	//========================================================================

	class VertexBuffer {
	public:
		VertexBuffer(bool instanced = false);
		VertexBuffer(const VoidSpan& span, bool instanced = false);
		VertexBuffer(const void* data, uint32_t size, uint32_t stride, bool instanced = false);
		VertexBuffer(const VertexBuffer&) = delete;
		VertexBuffer(VertexBuffer&&) = delete;
		VertexBuffer& operator=(const VertexBuffer&) = delete;
		VertexBuffer& operator=(VertexBuffer&&) = delete;
		~VertexBuffer();

		uint32_t GetID() const;
		void Set(const VoidSpan& span);
		void Set(const void* data, uint32_t size, uint32_t stride = 0u);
		void Update(const void* data, uint32_t size, uint32_t offset = 0u);
		void Bind() const;

	private:
		uint32_t m_id;
		uint32_t m_stride;
		uint32_t m_index;
	};
}