#pragma once

#include "zore/utils/sized_integer.hpp"
#include <concepts>
#include <vector>
#include <queue>

namespace zore {

	template <typename T>
	class object_pool {
	public:
		object_pool(uint32_t count = 0) {
			if (count > 0)
				m_data.resize(count);
		}
		object_pool(const object_pool&) = delete;
		object_pool& operator=(const object_pool&) = delete;
		~object_pool() = default;

	public:
		template<typename... Args>
			requires std::constructible_from<T, Args...>
		uint32_t acquire(Args&&... args) {
			if (m_free_indices.empty()) {
				m_data.emplace_back(std::forward<Args>(args)...);
				return static_cast<uint32_t>(m_data.size() - 1);
			}
			uint32_t index = get_free_index();
			m_data[index] = T(std::forward<Args>(args)...);
			return index;
		}

		void release(uint32_t index) {
			m_free_indices.push(index);
		}

		T& operator[](uint32_t index) {
			return m_data[index];
		}

		const T& operator[](uint32_t index) const {
			return m_data[index];
		}

	private:
		uint32_t get_free_index() {
			uint32_t index = m_free_indices.front();
			m_free_indices.pop();
			return index;
		}

	public:
		static constexpr inline uint32_t INVALID_INDEX = uint32_max;

	private:
		std::vector<T> m_data;
		std::queue<uint32_t> m_free_indices;
	};
}