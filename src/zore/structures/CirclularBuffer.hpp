#pragma once
#include <mutex>

template <size_t SIZE, class TYPE>
class circular_buffer {
public:
	explicit circular_buffer() : m_head(0), m_tail(0), m_full(false) {};
	~circular_buffer() = default;

	void push(const TYPE& item) {
		std::lock_guard<std::mutex> lock(m_mutex);
		m_data[m_head] = item;
		increment_index(m_head);
		if (m_full)
			increment_index(m_tail);
		else
			m_full = m_head == m_tail;
	}

	TYPE pop() {
		std::lock_guard<std::mutex> lock(m_mutex);
		TYPE item = m_data[m_tail];
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
		return m_head - m_tail + ((m_head < m_tail || m_full) ? SIZE : 0);
	}

	size_t capacity() const {
		return SIZE;
	}

private:
	void increment_index(size_t& index) {
		++index;
		if (index == SIZE)
			index = 0;
	}

private:
	std::mutex m_mutex;
	TYPE m_data[SIZE];
	size_t m_head, m_tail;
	bool m_full;
};