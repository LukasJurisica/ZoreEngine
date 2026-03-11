#ifdef __INTELLISENSE__
#include "zore/utils/uuid.hpp"
#endif
#include <atomic>
#include <utility>

namespace zore {

	//========================================================================
	//	Local Unique Identifier Class
	//========================================================================

	template<typename T>
	uuid<T>::uuid() {
		static std::atomic<T> id = 0;
		m_id = ++id;
	}

	template<typename T>
	uuid<T>::uuid(uuid&& other) noexcept : m_id(other.m_id) {
		other.m_id = INVALID_ID;
	}

	template<typename T>
	uuid<T>& uuid<T>::operator=(uuid&& other) noexcept {
		if (this != &other) {
			m_id = other.m_id;
			other.m_id = INVALID_ID;
		}
		return *this;
	}
}