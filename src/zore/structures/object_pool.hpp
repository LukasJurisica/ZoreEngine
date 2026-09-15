#pragma once

#include "zore/utils/sized_integer.hpp"
#include "zore/debug.hpp"
#include <concepts>
#include <vector>

namespace zore {

	template <typename T, std::unsigned_integral S = size_t>
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
			if (m_free_indices.empty()) {
				ENSURE(m_data.size() < static_cast<size_t>(INVALID_INDEX), "object_pool index overflow");
				m_data.emplace_back(std::forward<Args>(args)...);
				return static_cast<S>(m_data.size() - 1);
			}
			else {
				S index = m_free_indices.back();
				m_free_indices.pop_back();
				m_data[index] = T(std::forward<Args>(args)...);
				return index;
			}
		}

		void release(S index) { m_free_indices.emplace_back(index); }
		S count() const { return static_cast<S>(m_data.size() - m_free_indices.size()); }

		T& operator[](S index) { return m_data[index]; }
		const T& operator[](S index) const { return m_data[index]; }

		std::vector<T>::iterator begin() noexcept { return m_data.begin(); }
		std::vector<T>::iterator end() noexcept { return m_data.end(); }

		std::vector<T>::const_iterator begin() const noexcept { return m_data.begin(); }
		std::vector<T>::const_iterator end() const noexcept { return m_data.end(); }

		std::vector<T>::const_iterator cbegin() const noexcept { return m_data.cbegin(); }
		std::vector<T>::const_iterator cend() const noexcept { return m_data.cend(); }

	public:
		static constexpr inline S INVALID_INDEX = static_cast<S>(-1);

	private:
		std::vector<T> m_data;
		std::vector<S> m_free_indices;
	};
}