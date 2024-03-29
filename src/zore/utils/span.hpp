#pragma once

namespace zore {

	class VoidSpan {
	public:
		template<class T, size_t N>
		VoidSpan(T(&data)[N]) : m_data(data), m_count(N), m_elementSize(sizeof(T)) {}
		template<class T>
		VoidSpan(T* data, size_t size) : m_data(data), m_count(size), m_elementSize(sizeof(T)) {}
#if defined(_ARRAY_)
		template<class T, size_t N>
		VoidSpan(std::array<T, N>& data) : m_data(data.data()), m_count(N), m_elementSize(sizeof(T)) {}
#endif
#if defined(_VECTOR_)
		template<class T>
		VoidSpan(std::vector<T>& data) : m_data(data.data()), m_count(data.size()), m_elementSize(sizeof(T)) {}
#endif
#if defined(_STRING_)
		VoidSpan(std::string& data) : m_data(data.data()), m_count(data.size()), m_elementSize(1) {}
#endif
		~VoidSpan() = default;

		void* Data() const { return m_data; };
		size_t Size() const { return m_count; };
		size_t SizeBytes() const { return m_count * m_elementSize; };
		size_t ElementSize() const { return m_elementSize; };

	private:
		void* m_data;
		size_t m_count;
		size_t m_elementSize;
	};
}