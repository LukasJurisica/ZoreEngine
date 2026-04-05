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
		Base(const Base& other);
		Base(Base&& other) noexcept;
		Base& operator=(const Base& other);
		Base& operator=(Base&& other) noexcept;
		void Move(Base& other);
		void Copy(const Base& other);
		~Base();

	public:
		static void Init();
		static void Cleanup();

		uint32_t GetID() const;
		void Set(const void_span& span);
		void Set(const void* data, size_t size);
		void Update(const void* data, size_t size, size_t offset = 0u);

	protected:
		uint32_t m_index;
	};
}