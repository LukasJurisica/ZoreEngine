#pragma once

#include "zore/math/simd/sse/sse_core.hpp"
#include <bit>

namespace zm::simd {
#if SIMD_SSE >= ENCODE_VERSION(2, 0, 0)

	//========================================================================
	//  float32_4 SSE SIMD Vector
	//========================================================================

	struct alignas(16) float32_4 {
	public:
		// Constructors -------------------
		ALWAYS_INLINE explicit float32_4() : v(_mm_setzero_ps()) {}
		ALWAYS_INLINE explicit float32_4(float s) : v(_mm_set_ps1(s)) {}
		ALWAYS_INLINE explicit float32_4(float x, float y, float z, float w) : v(_mm_set_ps(w, z, y, x)) {}
		ALWAYS_INLINE explicit float32_4(const __m128& o) : v(o) {}
		ALWAYS_INLINE explicit float32_4(const int32_4& o) : v(_mm_cvtepi32_ps(reinterpret_cast<const __m128i&>(o))) {}
		ALWAYS_INLINE explicit float32_4(const uint32_4& o) : v(internal::cvt_epu32_ps(reinterpret_cast<const __m128i&>(o))) {}
		ALWAYS_INLINE void load(const float* p) { v = _mm_loadu_ps(p); }
		ALWAYS_INLINE void load_aligned(const float* p) { v = _mm_load_ps(p); }
		ALWAYS_INLINE void unload(float* p) const { _mm_storeu_ps(p, v); }
		ALWAYS_INLINE void unload_aligned(float* p) const { _mm_store_ps(p, v); }
		~float32_4() = default;

	public:
		// Comparison ---------------------
		ALWAYS_INLINE float32_4  operator== (const float32_4& o) const { return float32_4(_mm_cmpeq_ps(v, o.v)); }
		ALWAYS_INLINE float32_4  operator!= (const float32_4& o) const { return float32_4(_mm_cmpneq_ps(v, o.v)); }
		ALWAYS_INLINE float32_4  operator<  (const float32_4& o) const { return float32_4(_mm_cmplt_ps(v, o.v)); }
		ALWAYS_INLINE float32_4  operator<= (const float32_4& o) const { return float32_4(_mm_cmple_ps(v, o.v)); }
		ALWAYS_INLINE float32_4  operator>  (const float32_4& o) const { return float32_4(_mm_cmpgt_ps(v, o.v)); }
		ALWAYS_INLINE float32_4  operator>= (const float32_4& o) const { return float32_4(_mm_cmpge_ps(v, o.v)); }
		// Bit Operations -----------------
		ALWAYS_INLINE float32_4  operator&  (const float32_4& o) const { return float32_4(_mm_and_ps(v, o.v)); }
		ALWAYS_INLINE float32_4  operator&= (const float32_4& o) { v = _mm_and_ps(v, o.v); return *this; }
		ALWAYS_INLINE float32_4  operator|  (const float32_4& o) const { return float32_4(_mm_or_ps(v, o.v)); }
		ALWAYS_INLINE float32_4  operator|= (const float32_4& o) { v = _mm_or_ps(v, o.v); return *this; }
		ALWAYS_INLINE float32_4  operator^  (const float32_4& o) const { return float32_4(_mm_xor_ps(v, o.v)); }
		ALWAYS_INLINE float32_4  operator^= (const float32_4& o) { v = _mm_xor_ps(v, o.v); return *this; }
		ALWAYS_INLINE float32_4  operator~  () { return float32_4(internal::bit_not(v)); }
		// Arithmetic ---------------------
		ALWAYS_INLINE float32_4  operator+  () const { return float32_4(v); }
		ALWAYS_INLINE float32_4  operator+  (const float32_4& o) const { return float32_4(_mm_add_ps(v, o.v)); }
		ALWAYS_INLINE float32_4& operator+= (const float32_4& o) { v = _mm_add_ps(v, o.v); return *this; }
		ALWAYS_INLINE float32_4  operator-  () const { return float32_4(_mm_xor_ps(v, _mm_set_ps1(-0.0f))); }
		ALWAYS_INLINE float32_4  operator-  (const float32_4& o) const { return float32_4(_mm_sub_ps(v, o.v)); }
		ALWAYS_INLINE float32_4& operator-= (const float32_4& o) { v = _mm_sub_ps(v, o.v); return *this; }
		ALWAYS_INLINE float32_4  operator*  (const float32_4& o) const { return float32_4(_mm_mul_ps(v, o.v)); }
		ALWAYS_INLINE float32_4& operator*= (const float32_4& o) { v = _mm_mul_ps(v, o.v); return *this; }
		ALWAYS_INLINE float32_4  operator/  (const float32_4& o) const { return float32_4(_mm_div_ps(v, o.v)); }
		ALWAYS_INLINE float32_4& operator/= (const float32_4& o) { v = _mm_div_ps(v, o.v); return *this; }
		// Other --------------------------
		ALWAYS_INLINE float extract(int index) { return internal::extract(v, index); }
		ALWAYS_INLINE void insert(int index, float s) { v = internal::insert(v, index, s); }
		ALWAYS_INLINE float hsum() const { return internal::hsum(v); }
		ALWAYS_INLINE float dot(const float32_4& o) const { return internal::dot(v, o.v); }
		template<int x, int y = 0, int z = 0, int w = 0>
		ALWAYS_INLINE float32_4 shuffle() const { return float32_4(internal::shuffle<x, y, z, w>(v)); }
		template<int x, int y = 0, int z = 0, int w = 0>
		ALWAYS_INLINE float32_4 shuffle(const float32_4& o) const { return float32_4(internal::shuffle<x, y, z, w>(v, o.v)); }
		template<int x, int y, int z, int w>
		ALWAYS_INLINE float32_4 blend(const float32_4& o) const { return float32_4(internal::blend<x, y, z, w>(v, o.v)); }
		ALWAYS_INLINE static int size() { return 4; }

	public:
		__m128 v;
	};

	// Swizzling ----------------------
	template<int x, int y = 0, int z = 0, int w = 0>
	ALWAYS_INLINE float32_4 shuffle(const float32_4& a) { return a.shuffle<x, y, z, w>(); }
	template<int x, int y = 0, int z = 0, int w = 0>
	ALWAYS_INLINE float32_4 shuffle(const float32_4& a, const float32_4& b) { return a.shuffle<x, y, z, w>(b); }
	template<int x, int y, int z, int w>
	ALWAYS_INLINE float32_4 blend(const float32_4& a, const float32_4& b) { return a.blend<x, y, z, w>(b); }
	// Logical Operations -------------
	ALWAYS_INLINE float32_4 min(const float32_4& a, const float32_4& b) { return float32_4(_mm_min_ps(a.v, b.v)); }
	ALWAYS_INLINE float32_4 max(const float32_4& a, const float32_4& b) { return float32_4(_mm_max_ps(a.v, b.v)); }
	ALWAYS_INLINE float32_4 and_not(const float32_4& a, const float32_4& b) { return float32_4(_mm_andnot_ps(a.v, b.v)); }
	// Rounding -----------------------
	ALWAYS_INLINE float32_4 trunc(const float32_4& a) { return float32_4(internal::trunc(a.v)); }
	ALWAYS_INLINE float32_4 floor(const float32_4& a) { return float32_4(internal::floor(a.v)); }
	ALWAYS_INLINE float32_4 ceil(const float32_4& a) { return float32_4(internal::ceil(a.v)); }
	ALWAYS_INLINE float32_4 round(const float32_4& a) { return float32_4(internal::round(a.v)); }
	ALWAYS_INLINE float32_4 fract(const float32_4& a) { return float32_4(_mm_sub_ps(a.v, internal::trunc(a.v))); }
	ALWAYS_INLINE float32_4 abs(const float32_4& a) { return float32_4(_mm_andnot_ps(_mm_set_ps1(-0.0f), a.v)); }
	// Generic Math Operations --------
	ALWAYS_INLINE float32_4 rcp(const float32_4& a) { return float32_4(_mm_rcp_ps(a.v)); }
	ALWAYS_INLINE float32_4 sqrt(const float32_4& a) { return float32_4(_mm_sqrt_ps(a.v)); }
	ALWAYS_INLINE float32_4 rcp_sqrt(const float32_4& a) { return float32_4(_mm_rsqrt_ps(a.v)); }
	ALWAYS_INLINE float hsum(const float32_4& a) { return a.hsum(); }
	ALWAYS_INLINE float dot(const float32_4& a, const float32_4& b) { return a.dot(b); }
	ALWAYS_INLINE int mask(const float32_4& a) { return _mm_movemask_ps(a.v); }
	ALWAYS_INLINE void transpose(float32_4& a, float32_4& b, float32_4& c, float32_4& d) { _MM_TRANSPOSE4_PS(a.v, b.v, c.v, d.v); }

#endif
}