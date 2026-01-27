#pragma once

#include "zore/platform.hpp"
#include "zore/utils/sized_integer.hpp"
#include "zore/utils/concepts.hpp"

#define ENABLE_SIMD true

#if ENABLE_SIMD == true
#if defined(__SSE4_2__)
#define SIMD_SSE ENCODE_VERSION(4, 2, 0)
#elif defined(__SSE4_1__)
#define SIMD_SSE ENCODE_VERSION(4, 1, 0)
#elif defined(__SSSE3__)
#define SIMD_SSE ENCODE_VERSION(3, 1, 0)
#elif defined(__SSE3__)
#define SIMD_SSE ENCODE_VERSION(3, 0, 0)
#elif defined(COMPILER_MSVC) && defined(ARCHITECTURE_x86_32) && (_M_IX86_FP == 2)
#define SIMD_SSE ENCODE_VERSION(2, 0, 0)
#elif defined(COMPILER_MSVC) && defined(ARCHITECTURE_x86_64)
#define SIMD_SSE ENCODE_VERSION(2, 0, 0)
#elif defined(__SSE2__)
#define SIMD_SSE ENCODE_VERSION(2, 0, 0)
#endif

#if defined(__AVX512F__)
#define SIMD_AVX ENCODE_VERSION(3, 0, 0)
#elif defined(__AVX2__)
#define SIMD_AVX ENCODE_VERSION(2, 0, 0)
#elif defined(__AVX__)
#define SIMD_AVX ENCODE_VERSION(1, 0, 0)
#endif

namespace zm {

	template<typename T>
	struct simd_default_width;

	// Float Default SIMD width -------
	template<>
	struct simd_default_width<float> {
#if SIMD_SSE >= ENCODE_VERSION(2, 0, 0)
		static constexpr int value = 4;
#endif
	};

	// Int32 Default SIMD width -------
	template<>
	struct simd_default_width<int32_t> {
#if SIMD_SSE >= ENCODE_VERSION(2, 0, 0)
		static constexpr int value = 4;
#endif
	};
	
	// UInt32 Default SIMD width ------
	template<>
	struct simd_default_width<uint32_t> {
#if SIMD_SSE >= ENCODE_VERSION(2, 0, 0)
		static constexpr int value = 4;
#endif
	};

	template <int N>
	concept is_valid_simd_width = (N == 4 || N == 8 || N == 16);

	template <typename T, int N>
	concept simd_enabled = ENABLE_SIMD && (N <= simd_default_width<T>::value) && is_valid_simd_width<N>;

	template<typename T, int N>
	struct simd_base {
		static constexpr int size() noexcept { return N; }
	};

	template<typename T, int N = simd_default_width<T>::value>
	struct simd;
}

#endif