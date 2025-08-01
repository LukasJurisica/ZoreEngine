#pragma once

namespace zore {

	//========================================================================
	//	Void Span Class
	//========================================================================

	class VoidSpan {
	public:
		template<class T, size_t N>
		VoidSpan(T(&data)[N]) : VoidSpan(data, N) {}
		template<class T, typename = decltype(std::declval<T>().data())>
		VoidSpan(T& container) : VoidSpan(container.data(), container.size()) {}
		template<class T>
		VoidSpan(T* data, size_t size) : m_data(data), m_count(size), m_element_size(sizeof(T)) {}
		~VoidSpan() = default;

		inline void* Data() const { return m_data; };
		template<class T>
		inline T* Data() const { return static_cast<T*>(m_data); }
		inline size_t Size() const { return m_count; };
		inline size_t SizeBytes() const { return m_count * m_element_size; };
		inline size_t ElementSize() const { return m_element_size; };

	private:
		void* m_data;
		size_t m_count;
		size_t m_element_size;
	};
}