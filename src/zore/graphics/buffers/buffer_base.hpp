#pragma once

#include "zore/utils/sized_integer.hpp"
#include "zore/utils/span.hpp"

namespace zore::Buffer {

	//========================================================================
	//	Buffer Base
	//========================================================================

	class Base {
	protected:
		Base();
		Base(const void_span& span);
		Base(const void* data, size_t size);
		Base(const Base& other) = delete;
		Base& operator=(const Base& other) = delete;
		Base(Base&& other) noexcept;
		Base& operator=(Base&& other) noexcept;
		void Move(Base& other);
		~Base();

	public:
		uint32_t GetID() const;
		void Update(const void* data, size_t size, size_t offset = 0u);
		static void Delete(uint32_t& id);

	protected:
		void Set(const void_span& span);
		void Set(const void* data, size_t size);

	protected:
		uint32_t m_index;
	};
}