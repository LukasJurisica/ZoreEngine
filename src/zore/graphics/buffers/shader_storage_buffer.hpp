#pragma once

#include "zore/utils/sized_integer.hpp"
#include "zore/utils/span.hpp"

namespace zore {

	//========================================================================
	//	Shader Storage Buffer
	//========================================================================

	class ShaderStorageBuffer {
	public:
		ShaderStorageBuffer();
		ShaderStorageBuffer(const VoidSpan& span);
		ShaderStorageBuffer(const void* data, uint32_t size);
		ShaderStorageBuffer(const ShaderStorageBuffer&) = delete;
		ShaderStorageBuffer(ShaderStorageBuffer&&) = delete;
		ShaderStorageBuffer& operator=(const ShaderStorageBuffer&) = delete;
		ShaderStorageBuffer& operator=(ShaderStorageBuffer&&) = delete;
		~ShaderStorageBuffer();

		uint32_t GetID() const;
		void Set(const VoidSpan& span);
		void Set(const void* data, uint32_t size);
		void Update(const void* data, uint32_t size, uint32_t offset);
		void Bind() const;
		void Bind(uint32_t index);

	private:
		uint32_t m_id;
		uint32_t m_index;
	};
}