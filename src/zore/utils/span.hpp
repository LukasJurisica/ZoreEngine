#pragma once

#include <utility>

namespace zore {

	//========================================================================
	//	Void Span Class
	//========================================================================

	class void_span {
	public:
		template<class T, size_t N>
		void_span(T(&data)[N]) : void_span(data, N) {}
		template<class T, typename = decltype(std::declval<T>().data())>
		void_span(T& container) : void_span(container.data(), container.size()) {}
		template<class T>
		void_span(T* data, size_t size) : m_data(data), m_count(size), m_element_size(sizeof(T)) {}
		~void_span() = default;

		inline void* data() const { return m_data; };
		template<class T>
		inline T* data() const { return static_cast<T*>(m_data); }
		inline size_t size() const { return m_count; };
		inline size_t size_bytes() const { return m_count * m_element_size; };
		inline size_t element_size() const { return m_element_size; };

	private:
		void* m_data;
		size_t m_count;
		size_t m_element_size;
	};
}