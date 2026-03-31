#include "zore/graphics/resource.hpp"

namespace zore {

	Resource::Resource() : m_ref_count(1), m_id(uint32_max) {}

	uint32_t Resource::GetID() const {
		return m_id;
	}

	void Resource::Acquire() {
		m_ref_count++;
	}

	void Resource::Release() {
		m_ref_count--;
		if (m_ref_count == 0)
			Delete();
	}

	bool Resource::IsValid() const {
		return m_id != uint32_max;
	}
}