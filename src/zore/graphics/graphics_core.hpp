#pragma once

#include "zore/utils/sized_integer.hpp"
#include <type_traits>

namespace zore {

	//========================================================================
	//	Graphics Handle
	//========================================================================

	struct handle_t {
	public:
		handle_t() : m_value(0) {}
        explicit operator bool() const { return m_value != 0; }
		~handle_t() = default;

		template<typename HandleType>
		handle_t(HandleType handle) {
			static_assert(sizeof(HandleType) == sizeof(m_value), "handle_t can only be constructed from types that match handle_raw_t in size");
			if constexpr (std::is_pointer_v<HandleType> || std::is_null_pointer_v<HandleType>)
				m_value = static_cast<uint64_t>(reinterpret_cast<uintptr_t>(handle));
			else
				m_value = static_cast<uint64_t>(handle);
		}

		template<typename HandleType>
		HandleType as() const {
			static_assert(sizeof(HandleType) == sizeof(m_value), "handle_t can only be converted to types that match handle_raw_t in size");
			if constexpr (std::is_pointer_v<HandleType>)
				return reinterpret_cast<HandleType>(static_cast<uintptr_t>(m_value));
			else
				return static_cast<HandleType>(m_value);
		}

	private:
        uint64_t m_value;
	};
}