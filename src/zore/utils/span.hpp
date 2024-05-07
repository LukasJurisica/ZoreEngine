#pragma once

namespace zore {

	class VoidSpan {
	public:
		template<class T, size_t N>
		VoidSpan(T(&data)[N]) : VoidSpan(data, N) {}

		template<class T, typename = decltype(std::declval<T>().data())>
		VoidSpan(T& cont) : VoidSpan(cont.data(), cont.size()) {}

		~VoidSpan() = default;

		void* Data() const { return m_data; };
		size_t Size() const { return m_count; };
		size_t SizeBytes() const { return m_count * m_element_size; };
		size_t ElementSize() const { return m_element_size; };

	private:
		template<class T>
		VoidSpan(T* data, size_t size) : m_data(data), m_count(size), m_element_size(sizeof(T)) {}

	private:
		void* m_data;
		size_t m_count;
		size_t m_element_size;
	};
}