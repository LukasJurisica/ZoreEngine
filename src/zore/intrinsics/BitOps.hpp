#pragma once

#include "zore/Platform.hpp"
#include "zore/utils/DataTypes.hpp"
#include <bit>

#if defined(COMPILER_MSVC)
#	include <intrin.h>
#	if defined(ARCHITECTURE_x86_64)
#		pragma intrinsic(_tzcnt_u32)
#		pragma intrinsic(_tzcnt_u64)
#		pragma intrinsic(_lzcnt_u32)
#		pragma intrinsic(_lzcnt_u64)
#	elif defined(ARCHITECTURE_ARM)
#		pragma intrinsic(_BitScanForward)
#		pragma intrinsic(_BitScanForward64)
#		pragma intrinsic(_CountLeadingZeros)
#		pragma intrinsic(_CountLeadingZeros64)
#	endif
#elif defined(ARCHITECTURE_x86_64) && defined(COMPILER_GCC)
#	include <x86intrin.h>
#endif

namespace zore {

	/**
	 * Count trailing zeros in the binary representation of a 32 bit number (index of least significant set bit).
	 *
	 * @param[in] x         - 32 bit integer.
	 * @return the number of trailing zeros.
	 */
	inline uint32_t ctz_u32(uint32_t x) {
#if defined(ARCHITECTURE_x86_64) && defined(COMPILER_MSVC)
		return _tzcnt_u32(x);
#elif defined(ARCHITECTURE_ARM) && defined(COMPILER_MSVC)
		unsigned long result;
		return _BitScanForward(&result, x) ? result : 32;
#elif defined(COMPILER_GCC)
		return x ? __builtin_ctz(x) : 32;
#else
		if (x == 0) return 32;
		uint32_t count = 0;
		while (!(x & 1)) {
			++count;
			x >>= 1;
		}
		return count;
#endif
	}

	/**
	 * Count trailing zeros in the binary representation of a 64 bit number (index of least significant set bit).
	 *
	 * @param[in] x         - 64 bit unsigned integer.
	 * @return the number of trailing zeros.
	 */
	inline uint32_t ctz_u64(uint64_t x) {
#if defined(ARCHITECTURE_x86_64) && defined(COMPILER_MSVC)
		return _tzcnt_u64(x);
#elif defined(ARCHITECTURE_ARM) && defined(COMPILER_MSVC)
		unsigned long result;
		return _BitScanForward64(&result, x) ? result : 64;
#elif defined(COMPILER_GCC)
		return x ? __builtin_ctzll(x) : 64;
#else
		if (x == 0) return 64;
		uint32_t count = 0;
		while (!(x & 1)) {
			++count;
			x >>= 1;
		}
		return count;
#endif
	}

	/**
	 * Count leading zeros in the binary representation of a 32 bit number.
	 *
	 * @param[in] x         - 32 bit unsigned integer.
	 * @return the number of leading zeros.
	 */
	inline uint32_t clz_u32(uint32_t x)
	{
#if defined(ARCHITECTURE_x86_64) && defined(COMPILER_MSVC)
		return _lzcnt_u32(x);
#elif defined(ARCHITECTURE_ARM) && defined(COMPILER_MSVC)
		return _CountLeadingZeros(x);
#elif defined(COMPILER_GCC)
		return x ? __builtin_clz(x) : 32;
#else
		if (x == 0) return 32;
		static uint32_t mask = (1 << 31);
		uint32_t count = 0;
		while (!(x & mask)) {
			++count;
			x <<= 1;
		}
		return count;
#endif
	}

	/**
	 * Count leading zeros in the binary representation of a 64 bit number.
	 *
	 * @param[in] x         - 64 bit unsigned integer.
	 * @return the number of leading zeros.
	 */
	inline uint32_t clz_u64(uint64_t x)
	{
#if defined(ARCHITECTURE_x86_64) && defined(COMPILER_MSVC)
		return _lzcnt_u64(x);
#elif defined(ARCHITECTURE_ARM) && defined(COMPILER_MSVC)
		return _CountLeadingZeros64(x);
#elif defined(COMPILER_GCC)
		return x ? __builtin_clzll(x) : 64;
#else
		if (x == 0) return 64;
		static uint32_t mask = (1 << 31);
		uint32_t count = 0;
		while (!(x & mask)) {
			++count;
			x <<= 1;
		}
		return count;
#endif
	}

	/**
	 * Count the number of 1 bits (population count) in a 32 bit number.
	 *
	 * @param[in] x         - 32 bit unsigned integer.
	 * @return the number of 1 bits.
	 */
	inline uint32_t popcnt_u32(uint32_t x) {
#if defined(ARCHITECTURE_x86_64) && defined(COMPILER_MSVC)
		return __popcnt(x);
#elif defined(COMPILER_GCC)
		return __builtin_popcount(x);
#else
		uint32_t c = 0;
		while (x) {
			c += x & 1;
			x >>= 1;
		}
		return c;
#endif
	}

	/**
	 * Count the number of 1 bits (population count) in a 64 bit number.
	 *
	 * @param[in] x         - 64 bit integer.
	 * @return the number of 1 bits.
	 */
	inline uint32_t popcnt_u64(uint64_t x) {
#if defined(ARCHITECTURE_x86_64) && defined(COMPILER_MSVC)
		return __popcnt64(x);
#elif defined(COMPILER_GCC)
		return __builtin_popcountll(x);
#else
		uint32_t c = 0;
		while (x) {
			c += x & 1;
			x >>= 1;
		}
		return c;
#endif
	}

	inline uint16_t byteswap_u16(uint16_t x) {
		if constexpr (std::endian::native == std::endian::little) {
#if defined(COMPILER_MSVC)
			return _byteswap_ushort(x);
#elif defined(COMPILER_GCC)
			return __builtin_bswap16(x);
#else
			return (x << 8) | (x >> 8);
#endif
		}
		else {
			return x;
		}
	}

	inline uint32_t byteswap_u32(uint32_t x) {
		if constexpr (std::endian::native == std::endian::little) {
#if defined(COMPILER_MSVC)
			return _byteswap_ulong(x);
#elif defined(COMPILER_GCC)
			return __builtin_bswap32(x);
#else
			return ((x << 24) & 0xff000000) |
				   ((x << 8)  & 0x00ff0000) |
				   ((x >> 8)  & 0x0000ff00) |
				   ((x >> 24) & 0x000000ff);
#endif
		}
		else {
			return x;
		}
	}

	inline uint64_t byteswap_u64(uint64_t x) {
		if constexpr (std::endian::native == std::endian::little) {
#if defined(COMPILER_MSVC)
			return _byteswap_uint64(x);
#elif defined(COMPILER_GCC)
			return __builtin_bswap64(x);
#else
			return ((x << 56) & 0xff00000000000000) |
				   ((x << 40) & 0x00ff000000000000) |
				   ((x << 24) & 0x0000ff0000000000) |
				   ((x << 8)  & 0x000000ff00000000) |
				   ((x >> 8)  & 0x00000000ff000000) |
				   ((x >> 24) & 0x0000000000ff0000) |
				   ((x >> 40) & 0x000000000000ff00) |
				   ((x >> 56) & 0x00000000000000ff);
#endif
		}
		else {
			return x;
		}
	}
}