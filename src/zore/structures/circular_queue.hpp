#pragma once
#include <mutex>

template <typename T, size_t S>
class circular_queue {
public:
	explicit circular_queue() : m_head(0), m_tail(0), m_full(false) {};
	~circular_queue() = default;

	void push(const T& item) {
		std::lock_guard<std::mutex> lock(m_mutex);
		m_data[m_head] = item;
		increment_index(m_head);
		if (m_full)
			increment_index(m_tail);
		else
			m_full = m_head == m_tail;
	}

	T pop() {
		std::lock_guard<std::mutex> lock(m_mutex);
		T item = std::move(m_data[m_tail]);
		increment_index(m_tail);
		m_full = false;
		return item;
	}

	void reset() {
		std::lock_guard<std::mutex> lock(m_mutex);
		m_head = m_tail;
		m_full = false;
	}

	bool empty() const {
		return (m_head == m_tail) && !m_full;
	}

	bool full() const {
		return m_full;
	}

	size_t size() const {
		return m_head - m_tail + ((m_head < m_tail || m_full) ? S : 0);
	}

	size_t capacity() const {
		return S;
	}

private:
	void increment_index(size_t& index) {
		++index;
		if (index == capacity())
			index = 0;
	}

private:
	std::mutex m_mutex;
	T m_data[S];
	size_t m_head, m_tail;
	bool m_full;
};