#pragma once

#include "zore/platform.hpp"

#define SIMD_ENABLED true

#if SIMD_ENABLED == true
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
#endif

namespace zm::simd {
	struct float32_4;
	struct int32_4;
	struct uint32_4;
}