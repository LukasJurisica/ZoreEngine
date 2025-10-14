#pragma once

#include "zore/utils/sized_integer.hpp"
#include "zore/utils/span.hpp"

namespace zore {

	//========================================================================
	//	Index Buffer
	//========================================================================

	class IndexBuffer {
	public:
		IndexBuffer();
		IndexBuffer(const VoidSpan& span);
		IndexBuffer(const void* data, uint32_t size);
		IndexBuffer(const IndexBuffer&) = delete;
		IndexBuffer(IndexBuffer&&) = delete;
		IndexBuffer& operator=(const IndexBuffer&) = delete;
		IndexBuffer& operator=(IndexBuffer&&) = delete;
		~IndexBuffer();

		uint32_t GetID() const;
		void Set(const VoidSpan& span);
		void Set(const void* data, uint32_t size);
		void Update(const void* data, uint32_t size, uint32_t offset = 0u);
		void Bind() const;

	private:
		uint32_t m_id;
	};
}