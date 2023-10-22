#pragma once
/*
<mmintrin.h>  MMX
<xmmintrin.h> SSE
<emmintrin.h> SSE2
<pmmintrin.h> SSE3
<tmmintrin.h> SSSE3
<smmintrin.h> SSE4.1
<nmmintrin.h> SSE4.2
<ammintrin.h> SSE4A
<wmmintrin.h> AES
<immintrin.h> AVX, AVX2, FMA, AVX-512
*/
#include <immintrin.h>

/*
__SSE__
__SSE2__
__SSE3__
__AVX__
__AVX2__
*/

namespace zore::simd {
	typedef __m128 FLOAT_4;
	typedef __m256 FLOAT_8;
	typedef __m256d DOUBLE_4;
}



//struct QuadFloat {
//	union {
//		__m128 fr;
//		float fa[4];
//	};
//
//	QuadFloat(float a, float b, float c, float d) {
//		fr = _mm_set_ps(d, c, b, a);
//	}
//
//	QuadFloat(float a) {
//		fr = _mm_set_ps1(a);
//	}
//
//	QuadFloat() {
//		fr = _mm_setzero_ps();
//	}
//
//	static __m128 add(__m128 a, __m128 b) {
//		return _mm_add_ps(a, b);
//	}
//
//	static __m128 sub(__m128 a, __m128 b) {
//		return _mm_sub_ps(a, b);
//	}
//
//	static __m128 mul(__m128 a, __m128 b) {
//		return _mm_mul_ps(a, b);
//	}
//
//	static __m128 div(__m128 a, __m128 b) {
//		return _mm_div_ps(a, b);
//	}
//
//	static __m128 sqrt(__m128 a) {
//		return _mm_sqrt_ps(a);
//	}
//
//	static __m128 rcp(__m128 a) {
//		return _mm_rcp_ps(a);
//	}
//};
//
//struct OctFloat {
//	union {
//		__m256 fr;
//		float fa[8];
//	};
//
//	OctFloat(float a, float b, float c, float d, float e, float f, float g, float h) {
//		fr = _mm256_set_ps(h, g, f, e, d, c, b, a);
//	}
//
//	OctFloat(float a) {
//		fr = _mm256_set1_ps(a);
//	}
//
//	OctFloat() {
//		fr = _mm256_setzero_ps();
//	}
//
//	static __m256 add(__m256 a, __m256 b) {
//		return _mm256_add_ps(a, b);
//	}
//
//	static __m256 sub(__m256 a, __m256 b) {
//		return _mm256_sub_ps(a, b);
//	}
//
//	static __m256 mul(__m256 a, __m256 b) {
//		return _mm256_mul_ps(a, b);
//	}
//
//	static __m256 div(__m256 a, __m256 b) {
//		return _mm256_div_ps(a, b);
//	}
//
//	static __m256 sqrt(__m256 a) {
//		return _mm256_sqrt_ps(a);
//	}
//
//	static __m256 rcp(__m256 a) {
//		return _mm256_rcp_ps(a);
//	}
//};
//
//struct HexaDecaFloat {
//	union {
//		__m512 fr;
//		float fa[16];
//	};
//
//	HexaDecaFloat(float a, float b, float c, float d, float e, float f, float g, float h, float i, float j, float k, float l, float m, float n, float o, float p) {
//		fr = _mm512_set_ps(p, o, n, m, l, k, j, i, h, g, f, e, d, c, b, a);
//	}
//
//	HexaDecaFloat(float a) {
//		fr = _mm512_set1_ps(a);
//	}
//
//	HexaDecaFloat() {
//		fr = _mm512_setzero_ps();
//	}
//
//	static __m512 add(__m512 a, __m512 b) {
//		return _mm512_add_ps(a, b);
//	}
//
//	static __m512 sub(__m512 a, __m512 b) {
//		return _mm512_sub_ps(a, b);
//	}
//
//	static __m512 mul(__m512 a, __m512 b) {
//		return _mm512_mul_ps(a, b);
//	}
//
//	static __m512 div(__m512 a, __m512 b) {
//		return _mm512_div_ps(a, b);
//	}
//
//	static __m512 sqrt(__m512 a) {
//		return _mm512_sqrt_ps(a);
//	}
//
//	static __m512 rcp(__m512 a) {
//		return _mm512_rcp14_ps(a);
//	}
//};