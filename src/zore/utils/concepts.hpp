#pragma once

#include <concepts>

namespace zore {
	template <typename T>
	concept numeric = std::integral<T> || std::floating_point<T>;

	template <typename T>
	concept arithmetic = requires(T a, T b) {
		{ a + b } -> std::convertible_to<T>;
		{ a - b } -> std::convertible_to<T>;
		{ a * b } -> std::convertible_to<T>;
		{ a / b } -> std::convertible_to<T>;
		{ -a }    -> std::convertible_to<T>;
	};

	template <typename T>
	concept signed_int32 = std::integral<T> && sizeof(T) == 4 && std::is_signed_v<T>;

	template <typename T>
	concept unsigned_int32 = std::integral<T> && sizeof(T) == 4 && std::is_unsigned_v<T>;
}