#pragma once
#include <memory>
#include <utility>

namespace zore {

	template<typename T, void (*D)(T*) = nullptr>
	class unique_pointer {
	public:
		unique_pointer() : m_ptr(nullptr) {}
		explicit unique_pointer(T* ptr) noexcept : m_ptr(ptr) {}
		unique_pointer(const unique_pointer&) = delete;
		unique_pointer& operator=(const unique_pointer&) = delete;
		unique_pointer(unique_pointer&& other) noexcept : m_ptr(other.m_ptr) { other.m_ptr = nullptr; }
		unique_pointer& operator=(unique_pointer&& other) noexcept {
			if (this != &other) {
				reset(other.m_ptr);
				other.m_ptr = nullptr;
			}
			return *this;
		}
		~unique_pointer() { reset(); }

		explicit operator bool() const noexcept { return m_ptr != nullptr; }
		T& operator*() const { return *m_ptr; }
		T* operator->() const noexcept { return m_ptr; }
		T* get() const noexcept { return m_ptr; }

		void reset(T* ptr = nullptr) {
			if (m_ptr == ptr)
				return;

			T* prev = std::exchange(m_ptr, ptr);
			if (prev != nullptr) {
				if constexpr (D != nullptr)
					D(prev);
				else
					delete prev;
			}
		}

		T** c_ptr() noexcept { return &m_ptr; }

	private:
		T* m_ptr;
	};
}