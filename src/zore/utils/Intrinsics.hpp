#pragma once

#include "zore/platform/Platform.hpp"
#include "zore/utils/DataTypes.hpp"

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
	inline uint32_t popcnt_32(uint32_t x) {
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
	inline uint32_t popcnt_64(uint64_t x) {
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
}