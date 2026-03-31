#pragma once

#include "zore/graphics/buffers/buffer_base.hpp"

namespace zore {

	//========================================================================
	//	Shader Storage Buffer
	//========================================================================

	class ShaderStorageBuffer : public Buffer::Base {
	public:
		ShaderStorageBuffer();
		ShaderStorageBuffer(const void_span& span);
		ShaderStorageBuffer(const void* data, size_t size);
		ShaderStorageBuffer(ShaderStorageBuffer&&) = default;
		ShaderStorageBuffer& operator=(ShaderStorageBuffer&&) = default;
		ShaderStorageBuffer(const Buffer::Base& other);
		~ShaderStorageBuffer() = default;

		void Bind() const;
		void Bind(uint32_t bind_point);

	private:
		uint32_t m_bind_point;
	};
}