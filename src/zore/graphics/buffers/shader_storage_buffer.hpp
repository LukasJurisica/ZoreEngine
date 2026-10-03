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
		~ShaderStorageBuffer() = default;

		using Base::Set;
		void Bind() const;
		void Bind(uint32_t bind_point);

	private:
		uint32_t m_bind_point;
	};

	//========================================================================
	//	Shader Storage Buffer View
	//========================================================================

	class ShaderStorageBufferView {
	public:
		ShaderStorageBufferView(const Buffer::Base& other);
		~ShaderStorageBufferView() = default;

		void Bind() const;
		void Bind(uint32_t bind_point);

	private:
		uint32_t m_buffer_id;
		uint32_t m_bind_point;
	};
}