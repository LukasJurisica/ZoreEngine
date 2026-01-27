#pragma once

#include <utility>

namespace zore {

	//========================================================================
	//	Static For Loop
	//=========================================================================

	template<size_t N, typename F>
	constexpr void static_for(F&& function) {
		[&] <size_t... S>(std::index_sequence<S...>) {
			(function(std::integral_constant<size_t, S>{}), ...);
		}(std::make_index_sequence<N>{});
	}

	template<size_t Start, size_t Stop, typename F>
	constexpr void static_for(F&& function) {
		static_assert(Start <= Stop);
		[&] <size_t... S>(std::index_sequence<S...>) {
			(function(std::integral_constant<size_t, Start + S>{}), ...);
		}(std::make_index_sequence<Stop - Start>{});
	}
}