#pragma once

#include "zore/utils/sized_integer.hpp"
#include <concepts>
#include <vector>

namespace zore {

	template <typename T, typename S = size_t>
		requires (sizeof(T) >= sizeof(S))
	class object_pool {
	public:
		object_pool(S count = 0) { m_data.reserve(count); }
		object_pool(const object_pool&) = delete;
		object_pool& operator=(const object_pool&) = delete;
		~object_pool() = default;

	public:
		template<typename... Args>
			requires std::constructible_from<T, Args...>
		S acquire(Args&&... args) {
			if (m_next_free_index == INVALID_INDEX) {
				m_data.emplace_back(std::forward<Args>(args)...);
				return static_cast<S>(m_data.size() - 1);
			}
			S index = m_next_free_index;
			m_next_free_index = reinterpret_cast<S&>(m_data[index]);
			m_data[index] = T(std::forward<Args>(args)...);
			return index;
		}

		void release(S index) {
			reinterpret_cast<S&>(m_data[index]) = m_next_free_index;
			m_next_free_index = index;
		}

		T& operator[](S index) {
			return m_data[index];
		}

		const T& operator[](S index) const {
			return m_data[index];
		}

		std::vector<T>::iterator begin() {
			return m_data.begin();
		}

		std::vector<T>::const_iterator begin() const {
			return m_data.begin();
		}

		std::vector<T>::iterator end() {
			return m_data.end();
		}

		std::vector<T>::const_iterator end() const {
			return m_data.end();
		}

		const T* data() const {
			return m_data.data();
		}

		size_t size() const {
			return m_data.size();
		}

	public:
		static constexpr inline S INVALID_INDEX = static_cast<S>(-1);

	private:
		std::vector<T> m_data;
		S m_next_free_index = INVALID_INDEX;
	};
}