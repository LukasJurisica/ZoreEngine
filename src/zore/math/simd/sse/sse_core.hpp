#pragma once

#include "zore/math/simd/simd_core.hpp"
#include "zore/utils/concepts.hpp"

//#if SIMD_SSE >= ENCODE_VERSION(4, 2, 0)
//#include <nmmintrin.h>
//#elif SIMD_SSE >= ENCODE_VERSION(4, 1, 0)
//#include <smmintrin.h>
//#elif SIMD_SSE >= ENCODE_VERSION(3, 0, 0)
//#include <pmmintrin.h>
//#elif SIMD_SSE >= ENCODE_VERSION(2, 0, 0)
//#include <emmintrin.h>
//#elif SIMD_SSE >= ENCODE_VERSION(1, 0, 0)
//#include <xmmintrin.h>
//#endif

#include <immintrin.h>

#if SIMD_SSE >= ENCODE_VERSION(1, 0, 0)
#define _MM_BLEND(w, z, y, x) (((w) << 3) | ((z) << 2) | ((y) << 1) | ((x)))
#define _MM_BLEND_2(w, z, y, x) (((w * 3) << 6) | ((z * 3) << 4) | ((y * 3) << 2) | ((x * 3)))
#endif

#undef min
#undef max

namespace zm::simd::internal {

	/* Conversion  --------------------
	-------------------------------- */

	ALWAYS_INLINE __m128 cvt_epu32_ps(const __m128i& a) {
#if SIMD_SSE >= ENCODE_VERSION(4, 1, 0)
		__m128i lo = _mm_unpacklo_epi32(a, _mm_setzero_si128());
		__m128i hi = _mm_unpackhi_epi32(a, _mm_setzero_si128());
		return _mm_movelh_ps(_mm_cvtepi64_ps(lo), _mm_cvtepi64_ps(hi));
#else
		const __m128i mask = _mm_set1_epi32(0x80000000);
		__m128i lo = _mm_andnot_si128(mask, a);
		__m128i hi = _mm_and_si128(a, mask);
		return _mm_add_ps(_mm_cvtepi32_ps(lo), _mm_and_ps(_mm_cmpneq_ps(_mm_castsi128_ps(hi), _mm_setzero_ps()), _mm_set1_ps(2147483648.0f)));
#endif
	}

	/* Comparison  --------------------
	-------------------------------- */

	ALWAYS_INLINE __m128i cmp_lt_u(const __m128i& a, const __m128i& b) {
		const __m128i mask = _mm_set1_epi32(0x80000000);
		return _mm_cmplt_epi32(_mm_xor_si128(a, mask), _mm_xor_si128(b, mask));
	}

	ALWAYS_INLINE __m128i cmp_gt_u(const __m128i& a, const __m128i& b) {
		const __m128i mask = _mm_set1_epi32(0x80000000);
		return _mm_cmpgt_epi32(_mm_xor_si128(a, mask), _mm_xor_si128(b, mask));
	}

	/* Bitwise Operators  -------------
	-------------------------------- */

	ALWAYS_INLINE __m128 bit_not(const __m128& a) {
		return _mm_xor_ps(a, _mm_castsi128_ps(_mm_set1_epi32(~0)));
	}

	ALWAYS_INLINE __m128i bit_not(const __m128i& a) {
		return _mm_xor_si128(a, _mm_set1_epi32(~0));
	}

	/* Rounding Operators  ------------
	-------------------------------- */

	ALWAYS_INLINE __m128 trunc(const __m128& a) {
		return _mm_cvtepi32_ps(_mm_cvttps_epi32(a));
	}

	ALWAYS_INLINE __m128 floor(const __m128& a) {
#if SIMD_SSE >= ENCODE_VERSION(4, 1, 0)
		return _mm_floor_ps(a);
#else
		__m128 mask = internal::trunc(a);
		return _mm_sub_ps(mask, _mm_and_ps(_mm_cmpgt_ps(mask, a), _mm_set_ps1(1.f)));
#endif
	}

	ALWAYS_INLINE __m128 ceil(const __m128& a) {
#if SIMD_SSE >= ENCODE_VERSION(4, 1, 0)
		return _mm_ceil_ps(a);
#else
		__m128 mask = internal::trunc(a);
		return _mm_add_ps(mask, _mm_and_ps(_mm_cmplt_ps(mask, a), _mm_set_ps1(1.f)));
#endif
	}

	ALWAYS_INLINE __m128 round(const __m128& a) {
#if SIMD_SSE >= ENCODE_VERSION(4, 1, 0)
		return _mm_round_ps(a, _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC);
#else
		__m128 mask = _mm_and_ps(a, _mm_set_ps1(-0.f));
		return internal::trunc(_mm_add_ps(a, _mm_or_ps(mask, _mm_set_ps1(0.5f))));
#endif
	}

	ALWAYS_INLINE __m128i min(const __m128i& a, const __m128i& b) {
#if SIMD_SSE >= ENCODE_VERSION(4, 1, 0)
		return _mm_min_epi32(a, b);
#else
		__m128i mask = _mm_cmplt_epi32(a, b);
		return _mm_or_si128(_mm_and_si128(mask, b), _mm_andnot_si128(mask, a));
#endif
	}

	ALWAYS_INLINE __m128i min_u(const __m128i& a, const __m128i& b) {
#if SIMD_SSE >= ENCODE_VERSION(4, 1, 0)
		return _mm_min_epu32(a, b);
#else
		__m128i mask = internal::cmp_lt_u(a, b);
		return _mm_or_si128(_mm_and_si128(mask, b), _mm_andnot_si128(mask, a));
#endif
	}

	ALWAYS_INLINE __m128i max(const __m128i& a, const __m128i& b) {
#if SIMD_SSE >= ENCODE_VERSION(4, 1, 0)
		return _mm_max_epi32(a, b);
#else
		__m128i mask = _mm_cmpgt_epi32(a, b);
		return _mm_or_si128(_mm_and_si128(mask, a), _mm_andnot_si128(mask, b));
#endif
	}

	ALWAYS_INLINE __m128i max_u(const __m128i& a, const __m128i& b) {
#if SIMD_SSE >= ENCODE_VERSION(4, 1, 0)
		return _mm_max_epu32(a, b);
#else
		__m128i mask = internal::cmp_gt_u(a, b);
		return _mm_or_si128(_mm_and_si128(mask, a), _mm_andnot_si128(mask, b));
#endif
	}

	ALWAYS_INLINE __m128i abs(const __m128i& a) {
#if SIMD_SSE >= ENCODE_VERSION(4, 1, 0)
		return _mm_abs_epi32(a);
#else
		const __m128i mask = _mm_srai_epi32(a, 31);
		return _mm_sub_epi32(_mm_xor_si128(a, mask), mask);
#endif
	}

	/* shuffle ------------------------
	   Swizzle the components of a vector
	-------------------------------- */
	template<int x, int y, int z, int w>
	ALWAYS_INLINE __m128 shuffle(const __m128& a, const __m128& b) {
		static_assert(x >= 0 && x <= 3 && y >= 0 && y <= 3 && z >= 0 && z <= 3 && w >= 0 && w <= 3, "shuffle parameters must be between 0 and 3 inclusive.");
		return _mm_shuffle_ps(a, b, _MM_SHUFFLE(w, z, y, x));
	}

	template<int x, int y, int z, int w>
	ALWAYS_INLINE __m128i shuffle(const __m128i& a) {
		static_assert(x >= 0 && x <= 3 && y >= 0 && y <= 3 && z >= 0 && z <= 3 && w >= 0 && w <= 3, "shuffle parameters must be between 0 and 3 inclusive.");
		return _mm_shuffle_epi32(a, _MM_SHUFFLE(w, z, y, x));
	}

	/* blend  -------------------------
	   Blend the components of two vectors
	-------------------------------- */
	template<int x, int y, int z, int w>
	ALWAYS_INLINE __m128 blend(const __m128& a, const __m128& b) {
		static_assert(x >= 0 && x <= 1 && y >= 0 && y <= 1 && z >= 0 && z <= 1 && w >= 0 && w <= 1, "Blend parameters must be 0 or 1.");
#if SIMD_SSE >= ENCODE_VERSION(4, 1, 0)
		return _mm_blend_ps(a, b, _MM_BLEND(w, z, y, x));
#else
		// Optimize the original data, and do an aligned load?
		constexpr float alignas(16) data[4] = { x ? -1 : 0, y ? -1 : 0, z ? -1 : 0, w ? -1 : 0 };
		const __m128 mask = _mm_load_ps(data);
		return _mm_or_ps(_mm_and_ps(a, mask), _mm_andnot_ps(mask, b));
#endif
	}

	template<int x, int y, int z, int w>
	ALWAYS_INLINE __m128i blend(const __m128i& a, const __m128i& b) {
		static_assert(x >= 0 && x <= 1 && y >= 0 && y <= 1 && z >= 0 && z <= 1 && w >= 0 && w <= 1, "Blend parameters must be 0 or 1.");
#if SIMD_SSE >= ENCODE_VERSION(4, 1, 0)
		return _mm_blend_epi16(a, b, _MM_BLEND_2(w, z, y, x));
#else
		constexpr int32_t alignas(16) data[4] = { x ? -1 : 0, y ? -1 : 0, z ? -1 : 0, w ? -1 : 0 };
		const __m128i mask = _mm_load_si128(reinterpret_cast<const __m128i*>(data));
		return _mm_or_si128(_mm_and_si128(a, mask), _mm_andnot_si128(mask, b));
#endif
	}

	/* Arithmetic  --------------------
	-------------------------------- */

	ALWAYS_INLINE __m128i mullo_32(const __m128i& a, const __m128i& b) {
#if SIMD_SSE >= ENCODE_VERSION(4, 1, 0)
		return _mm_mullo_epi32(a, b);
#else
		__m128i tmp1 = _mm_mul_epu32(a, b);
		__m128i tmp2 = _mm_mul_epu32(_mm_srli_si128(a, 4), _mm_srli_si128(b, 4));
		return _mm_unpacklo_epi32(internal::shuffle<0, 2, 0, 0>(tmp1), internal::shuffle<0, 2, 0, 0>(tmp2));
#endif
	}

	ALWAYS_INLINE __m128i div_i32(const __m128i& a, const __m128i& b) {
		int32_t x[4], y[4], z[4];
		_mm_storeu_si128(reinterpret_cast<__m128i*>(x), a);
		_mm_storeu_si128(reinterpret_cast<__m128i*>(y), b);
		for (int i = 0; i < 4; i++)
			z[i] = x[i] / y[i];
		return _mm_loadu_si128(reinterpret_cast<const __m128i*>(z));
	}

	ALWAYS_INLINE __m128i div_u32(const __m128i& a, const __m128i& b) {
		uint32_t x[4], y[4], z[4];
		_mm_storeu_si128(reinterpret_cast<__m128i*>(x), a);
		_mm_storeu_si128(reinterpret_cast<__m128i*>(y), b);
		for (int i = 0; i < 4; i++)
			z[i] = x[i] / y[i];
		return _mm_loadu_si128(reinterpret_cast<const __m128i*>(z));
	}

	/* Load / Store -------------------
	-------------------------------- */

	ALWAYS_INLINE float extract(const __m128& a, int i) {
#if SIMD_SSE >= ENCODE_VERSION(4, 1, 0)
		return std::bit_cast<float>(_mm_extract_ps(v, i));
#else
		switch (i) {
		case 0: return _mm_cvtss_f32(a);
		case 1: return _mm_cvtss_f32(internal::shuffle<1, 0, 0, 0>(a, a));
		case 2: return _mm_cvtss_f32(internal::shuffle<2, 0, 0, 0>(a, a));
		case 3: return _mm_cvtss_f32(internal::shuffle<3, 0, 0, 0>(a, a));
		default: return 0;
		}
#endif
	}

	ALWAYS_INLINE int extract(const __m128i& a, int index) {
#if SIMD_SSE >= ENCODE_VERSION(4, 1, 0)
		return _mm_extract_epi32(a, index);
#else
		switch (index) {
		case 0: return _mm_cvtsi128_si32(a);
		case 1: return _mm_cvtsi128_si32(internal::shuffle<1, 0, 0, 0>(a));
		case 2: return _mm_cvtsi128_si32(internal::shuffle<2, 0, 0, 0>(a));
		case 3: return _mm_cvtsi128_si32(internal::shuffle<3, 0, 0, 0>(a));
		default: return 0;
		}
#endif
	}

	ALWAYS_INLINE __m128 insert(const __m128& a, int i, float s) {
#if SIMD_SSE >= ENCODE_VERSION(4, 1, 0)
		return _mm_insert_ps(a, _mm_set_ps1(s), i << 4);
#else
		switch (i) {
		case 0: return internal::blend<1, 0, 0, 0>(a, _mm_set_ps1(s));
		case 1: return internal::blend<0, 1, 0, 0>(a, _mm_set_ps1(s));
		case 2: return internal::blend<0, 0, 1, 0>(a, _mm_set_ps1(s));
		case 3: return internal::blend<0, 0, 0, 1>(a, _mm_set_ps1(s));
		}
#endif
	}

	ALWAYS_INLINE __m128i insert(const __m128i& a, int i, int s) {
#if SIMD_SSE >= ENCODE_VERSION(4, 1, 0)
		v = _mm_insert_epi32(v, value, index);
#else
		switch (i) {
		case 0: return internal::blend<1, 0, 0, 0>(a, _mm_set1_epi32(s));
		case 1: return internal::blend<0, 1, 0, 0>(a, _mm_set1_epi32(s));
		case 2: return internal::blend<0, 0, 1, 0>(a, _mm_set1_epi32(s));
		case 3: return internal::blend<0, 0, 0, 1>(a, _mm_set1_epi32(s));
		}
#endif
	}

	/* hsum ---------------------------
	   Compute the horizontal sum of a vector
	-------------------------------- */
	ALWAYS_INLINE float hsum(const __m128& a) {
#if SIMD_SSE >= ENCODE_VERSION(4, 1, 0)
		return _mm_cvtss_f32(_mm_dp_ps(a, _mm_set1_ps(1.f), 0xF1));
#elif SIMD_SSE >= ENCODE_VERSION(3, 0, 0)
		return _mm_cvtss_f32(_mm_hadd_ps(_mm_hadd_ps(a, a), a));
#else
		__m128 shuf = internal::shuffle<2, 3, 0, 0>(a, a);
		__m128 sums = _mm_add_ps(a, shuf);
		shuf = internal::shuffle<1, 0, 0, 0>(sums, sums);
		return _mm_cvtss_f32(_mm_add_ss(sums, shuf));
#endif
	}

	ALWAYS_INLINE int hsum(const __m128i& a) {
#if SIMD_SSE >= ENCODE_VERSION(3, 0, 0)
		return _mm_cvtsi128_si32(_mm_hadd_epi32(_mm_hadd_epi32(a, a), a));
#else
		__m128i shuf = internal::shuffle<2, 3, 0, 0>(a);
		__m128i sums = _mm_add_epi32(a, shuf);
		shuf = internal::shuffle<1, 0, 0, 0>(sums);
		return _mm_cvtsi128_si32(_mm_add_epi32(sums, shuf));
#endif
	}

	/* dot ----------------------------
	   Compute the dot product
	-------------------------------- */
	ALWAYS_INLINE float dot(const __m128& a, const __m128& b) {
#if SIMD_SSE >= ENCODE_VERSION(4, 1, 0)
		return _mm_cvtss_f32(_mm_dp_ps(a, b, 0xF1));
#else
		return internal::hsum(_mm_mul_ps(a, b));
#endif
	}

	ALWAYS_INLINE int dot(const __m128i& a, const __m128i& b) {
		return internal::hsum(internal::mullo_32(a, b));
	}
}