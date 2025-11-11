#pragma once

#include "zore/utils/sized_integer.hpp"
#include <type_traits>

namespace zore {

#if defined(ZORE_RENDER_API_OPENGL)
	typedef uint32_t handle_raw_t;
#elif defined(ZORE_RENDER_API_VULKAN)
	typedef uint64_t handle_raw_t;
#else 
#error "Render API not defined"
#endif

	//========================================================================
	//	Graphics Handle
	//========================================================================

	struct handle_t {
	public:
		handle_t() : m_value(0) {}
		operator handle_raw_t() const { return m_value; }
		~handle_t() = default;

		template<typename HandleType>
		handle_t(HandleType handle) {
			static_assert(sizeof(HandleType) == sizeof(m_value), "handle_t can only be constructed from types that match handle_raw_t in size");
			if constexpr (std::is_pointer_v<HandleType> || std::is_null_pointer_v<HandleType>)
				m_value = static_cast<handle_raw_t>(reinterpret_cast<uintptr_t>(handle));
			else
				m_value = static_cast<handle_raw_t>(handle);
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
		handle_raw_t m_value;
	};
}