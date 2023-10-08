#pragma once

#include <vector>
#include <array>

namespace zore {

	class VoidSpan {
	public:
		template<class T, size_t N>
		VoidSpan(T(&data)[N]) : m_data(data), m_count(N), m_elementSize(sizeof(T)) {}
		template<class T>
		VoidSpan(T* data, size_t size) : m_data(data), m_count(size), m_elementSize(sizeof(T)) {}
		template<class T, size_t N>
		VoidSpan(std::array<T, N>& data) : m_data(data.data()), m_count(N), m_elementSize(sizeof(T)) {}
		template<class T>
		VoidSpan(std::vector<T>& data) : m_data(data.data()), m_count(data.size()), m_elementSize(sizeof(T)) {}
		~VoidSpan() = default;

		void* Data() const;
		size_t Size() const;
		size_t SizeBytes() const;
		size_t ElementSize() const;

	private:
		void* m_data;
		size_t m_elementSize;
		size_t m_count;
	};
}