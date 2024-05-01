#pragma once

namespace zore {

	class VoidSpan {
	public:
		template<class T>
		VoidSpan(T& data) : m_data(&data), m_count(1), m_element_size(sizeof(T)) {}

		template<class T, size_t N>
		VoidSpan(T(&data)[N]) : m_data(data), m_count(N), m_element_size(sizeof(T)) {}

		template<class T>
		VoidSpan(T* data, size_t size) : m_data(data), m_count(size), m_element_size(sizeof(T)) {}

#if defined(_ARRAY_)
		template<class T, size_t N>
		VoidSpan(std::array<T, N>& data) : m_data(data.data()), m_count(N), m_element_size(sizeof(T)) {}
#endif

#if defined(_VECTOR_)
		template<class T>
		VoidSpan(std::vector<T>& data) : m_data(data.data()), m_count(data.size()), m_element_size(sizeof(T)) {}
#endif

#if defined(_STRING_)
		VoidSpan(std::string& data) : m_data(data.data()), m_count(data.size()), m_element_size(1) {}
#endif

		~VoidSpan() = default;

		void* Data() const { return m_data; };
		size_t Size() const { return m_count; };
		size_t SizeBytes() const { return m_count * m_element_size; };
		size_t ElementSize() const { return m_element_size; };

	private:
		void* m_data;
		size_t m_count;
		size_t m_element_size;
	};
}