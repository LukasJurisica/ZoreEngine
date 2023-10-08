#pragma once

#include <zore/utils/span.hpp>

namespace zore {

	void* VoidSpan::Data() const {
		return m_data;
	}

	size_t VoidSpan::Size() const {
		return m_count;
	}

	size_t VoidSpan::SizeBytes() const {
		return m_count * m_elementSize;
	}

	size_t VoidSpan::ElementSize() const {
		return m_elementSize;
	}
}