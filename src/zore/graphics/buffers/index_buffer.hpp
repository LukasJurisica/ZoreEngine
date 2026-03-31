#pragma once

#include "zore/graphics/buffers/buffer_base.hpp"

namespace zore {

	//========================================================================
	//	Index Buffer
	//========================================================================

	class IndexBuffer : public Buffer::Base {
	public:
		IndexBuffer() = default;
		IndexBuffer(const void_span& span);
		IndexBuffer(const void* data, size_t size);
		IndexBuffer(IndexBuffer&&) = default;
		IndexBuffer& operator=(IndexBuffer&&) = default;
		~IndexBuffer() = default;

		void Bind() const;
	};
}