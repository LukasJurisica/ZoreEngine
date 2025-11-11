#pragma once

#include "zore/graphics/graphics_core.hpp"
#include "zore/utils/span.hpp"

namespace zore {

	//========================================================================
	//	Uniform Buffer
	//========================================================================

	class UniformBuffer {
	public:
		UniformBuffer();
		UniformBuffer(const VoidSpan& span);
		UniformBuffer(const void* data, uint32_t size);
		UniformBuffer(const UniformBuffer&) = delete;
		UniformBuffer(UniformBuffer&&) = delete;
		UniformBuffer& operator=(const UniformBuffer&) = delete;
		UniformBuffer& operator=(UniformBuffer&&) = delete;
		~UniformBuffer();

		uint32_t GetID() const;
		void Set(const VoidSpan& span);
		void Set(const void* data, uint32_t size);
		void Update(const void* data, uint32_t size, uint32_t offset = 0u);
		void Bind() const;
		void Bind(uint32_t index);

	private:
		uint32_t m_id;
		uint32_t m_index;
	};
}