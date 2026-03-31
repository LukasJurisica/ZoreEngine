#pragma once

#include "zore/graphics/buffers/buffer_base.hpp"

namespace zore {

	//========================================================================
	//	Uniform Buffer
	//========================================================================

	class UniformBuffer : public Buffer::Base {
	public:
		UniformBuffer();
		UniformBuffer(const void_span& span);
		UniformBuffer(const void* data, size_t size);
		UniformBuffer(UniformBuffer&&) = default;
		UniformBuffer& operator=(UniformBuffer&&) = default;
		~UniformBuffer() = default;

		void Bind() const;
		void Bind(uint32_t bind_point);

	private:
		uint32_t m_bind_point;
	};
}