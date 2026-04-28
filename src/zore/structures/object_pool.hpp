#pragma once

#include "zore/utils/sized_integer.hpp"
#include <concepts>
#include <vector>
#include <queue>

namespace zore {

	template <typename T, typename S = size_t>
	class object_pool {
	public:
		object_pool(S count = 0) { m_data.resize(count); }
		object_pool(const object_pool&) = delete;
		object_pool& operator=(const object_pool&) = delete;
		~object_pool() = default;

	public:
		template<typename... Args>
			requires std::constructible_from<T, Args...>
		S acquire(Args&&... args) {
			if (m_free_indices.empty()) {
				m_data.emplace_back(std::forward<Args>(args)...);
				return static_cast<S>(m_data.size() - 1);
			}
			S index = get_free_index();
			m_data[index] = T(std::forward<Args>(args)...);
			return index;
		}

		void release(S index) {
			m_free_indices.push(index);
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

	private:
		S get_free_index() {
			S index = m_free_indices.front();
			m_free_indices.pop();
			return index;
		}

	public:
		static constexpr inline S INVALID_INDEX = static_cast<S>(-1);

	private:
		std::vector<T> m_data;
		std::queue<S> m_free_indices;
	};
}