#pragma once

#include <cstdint>


using std::int8_t;
using std::uint8_t;
using std::int16_t;
using std::uint16_t;
using std::int32_t;
using std::uint32_t;
using std::int64_t;
using std::uint64_t;

namespace zore {

	inline constexpr int8_t int8_max = 127;
	inline constexpr int8_t int8_min = -128;
	inline constexpr uint8_t uint8_max = 255;

	inline constexpr int16_t int16_max = 32767;
	inline constexpr int16_t int16_min = -32768;
	inline constexpr uint16_t uint16_max = 65535;

	inline constexpr int32_t int32_max = 2147483647;
	inline constexpr int32_t int32_min = -2147483648;
	inline constexpr uint32_t uint32_max = 4294967295;

	inline constexpr int64_t int64_max = 9223372036854775807;
	inline constexpr int64_t int64_min = -9223372036854775808;
	inline constexpr uint64_t uint64_max = 18446744073709551615;
}