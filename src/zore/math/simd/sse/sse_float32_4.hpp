#pragma once

#include "zore/math/simd/sse/sse_core.hpp"

namespace zm {

	//========================================================================
	//  float32_4 SSE SIMD Vector
	//========================================================================

	template<>
	struct simd<float, 4> : simd_base<float, 4> {
	public:
		ALWAYS_INLINE explicit simd() : v(_mm_setzero_ps()) {}
		ALWAYS_INLINE explicit simd(float s) : v(_mm_set_ps1(s)) {}
		ALWAYS_INLINE explicit simd(float x, float y, float z, float w) : v(_mm_set_ps(w, z, y, x)) {}
		ALWAYS_INLINE explicit simd(const float* o) { load(o); }
		ALWAYS_INLINE explicit simd(const __m128& o) : v(o) {}
		ALWAYS_INLINE explicit simd(const simd<int32_t, 4>& o) : v(_mm_cvtepi32_ps(reinterpret_cast<const __m128i&>(o))) {}
		ALWAYS_INLINE explicit simd(const simd<uint32_t, 4>& o) : v(internal::cvt_epu32_ps(reinterpret_cast<const __m128i&>(o))) {}
		ALWAYS_INLINE void load(const float* p) { v = _mm_loadu_ps(p); }
		ALWAYS_INLINE void load_aligned(const float* p) { v = _mm_load_ps(p); }
		ALWAYS_INLINE void unload(float* p) const { _mm_storeu_ps(p, v); }
		ALWAYS_INLINE void unload_aligned(float* p) const { _mm_store_ps(p, v); }
		~simd() = default;

	public:
		// Comparison ---------------------
		ALWAYS_INLINE simd  operator== (const simd& o) const { return simd(_mm_cmpeq_ps(v, o.v)); }
		ALWAYS_INLINE simd  operator!= (const simd& o) const { return simd(_mm_cmpneq_ps(v, o.v)); }
		ALWAYS_INLINE simd  operator<  (const simd& o) const { return simd(_mm_cmplt_ps(v, o.v)); }
		ALWAYS_INLINE simd  operator<= (const simd& o) const { return simd(_mm_cmple_ps(v, o.v)); }
		ALWAYS_INLINE simd  operator>  (const simd& o) const { return simd(_mm_cmpgt_ps(v, o.v)); }
		ALWAYS_INLINE simd  operator>= (const simd& o) const { return simd(_mm_cmpge_ps(v, o.v)); }
		// Bit Operations -----------------
		template<zore::numeric T>
		ALWAYS_INLINE simd  operator&  (const T o) const { return simd(_mm_and_ps(v, _mm_set_ps1(static_cast<float>(o)))); }
		ALWAYS_INLINE simd  operator&  (const simd& o) const { return simd(_mm_and_ps(v, o.v)); }
		template<zore::numeric T>
		ALWAYS_INLINE simd& operator&= (const T o) { v = _mm_and_ps(v, _mm_set_ps1(static_cast<float>(o))); return *this; }
		ALWAYS_INLINE simd  operator&= (const simd& o) { v = _mm_and_ps(v, o.v); return *this; }
		template<zore::numeric T>
		ALWAYS_INLINE simd  operator|  (const T o) const { return simd(_mm_or_ps(v, _mm_set_ps1(static_cast<float>(o)))); }
		ALWAYS_INLINE simd  operator|  (const simd& o) const { return simd(_mm_or_ps(v, o.v)); }
		template<zore::numeric T>
		ALWAYS_INLINE simd& operator|= (const T o) { v = _mm_or_ps(v, _mm_set_ps1(static_cast<float>(o))); return *this; }
		ALWAYS_INLINE simd  operator|= (const simd& o) { v = _mm_or_ps(v, o.v); return *this; }
		template<zore::numeric T>
		ALWAYS_INLINE simd  operator^  (const T o) const { return simd(_mm_xor_ps(v, _mm_set_ps1(static_cast<float>(o)))); }
		ALWAYS_INLINE simd  operator^  (const simd& o) const { return simd(_mm_xor_ps(v, o.v)); }
		template<zore::numeric T>
		ALWAYS_INLINE simd& operator^= (const T o) { v = _mm_xor_ps(v, _mm_set_ps1(static_cast<float>(o))); return *this; }
		ALWAYS_INLINE simd  operator^= (const simd& o) { v = _mm_xor_ps(v, o.v); return *this; }
		ALWAYS_INLINE simd  operator~  () { return simd(internal::bit_not(v)); }
		// Arithmetic ---------------------
		template<zore::numeric T>
		ALWAYS_INLINE simd  operator+  (const T o) const { return simd(_mm_add_ps(v, _mm_set_ps1(static_cast<float>(o)))); }
		ALWAYS_INLINE simd  operator+  () const { return simd(v); }
		ALWAYS_INLINE simd  operator+  (const simd& o) const { return simd(_mm_add_ps(v, o.v)); }
		template<zore::numeric T>
		ALWAYS_INLINE simd& operator+= (const T o) { v = _mm_add_ps(v, _mm_set_ps1(static_cast<float>(o))); return *this; }
		ALWAYS_INLINE simd& operator+= (const simd& o) { v = _mm_add_ps(v, o.v); return *this; }
		template<zore::numeric T>
		ALWAYS_INLINE simd  operator-  (const T o) const { return simd(_mm_sub_ps(v, _mm_set_ps1(static_cast<float>(o)))); }
		ALWAYS_INLINE simd  operator-  () const { return simd(_mm_xor_ps(v, _mm_set_ps1(-0.0f))); }
		ALWAYS_INLINE simd  operator-  (const simd& o) const { return simd(_mm_sub_ps(v, o.v)); }
		template<zore::numeric T>
		ALWAYS_INLINE simd& operator-= (const T o) { v = _mm_sub_ps(v, _mm_set_ps1(static_cast<float>(o))); return *this; }
		ALWAYS_INLINE simd& operator-= (const simd& o) { v = _mm_sub_ps(v, o.v); return *this; }
		template<zore::numeric T>
		ALWAYS_INLINE simd  operator*  (const T o) const { return simd(_mm_mul_ps(v, _mm_set_ps1(static_cast<float>(o)))); }
		ALWAYS_INLINE simd  operator*  (const simd& o) const { return simd(_mm_mul_ps(v, o.v)); }
		template<zore::numeric T>
		ALWAYS_INLINE simd& operator*= (const T o) { v = _mm_mul_ps(v, _mm_set_ps1(static_cast<float>(o))); return *this; }
		ALWAYS_INLINE simd& operator*= (const simd& o) { v = _mm_mul_ps(v, o.v); return *this; }
		template<zore::numeric T>
		ALWAYS_INLINE simd  operator/  (const T o) const { return simd(_mm_div_ps(v, _mm_set_ps1(static_cast<float>(o)))); }
		ALWAYS_INLINE simd  operator/  (const simd& o) const { return simd(_mm_div_ps(v, o.v)); }
		template<zore::numeric T>
		ALWAYS_INLINE simd& operator/= (const T o) { v = _mm_div_ps(v, _mm_set_ps1(static_cast<float>(o))); return *this; }
		ALWAYS_INLINE simd& operator/= (const simd& o) { v = _mm_div_ps(v, o.v); return *this; }
		// Other --------------------------
		ALWAYS_INLINE float extract(int index) { return internal::extract(v, index); }
		ALWAYS_INLINE void insert(int index, float s) { v = internal::insert(v, index, s); }
		ALWAYS_INLINE float hsum() const { return internal::hsum(v); }
		ALWAYS_INLINE float dot(const simd& o) const { return internal::dot(v, o.v); }
		template<int x, int y = 0, int z = 0, int w = 0>
		ALWAYS_INLINE simd shuffle() const { return simd(internal::shuffle<x, y, z, w>(v)); }
		template<int x, int y = 0, int z = 0, int w = 0>
		ALWAYS_INLINE simd shuffle(const simd& o) const { return simd(internal::shuffle<x, y, z, w>(v, o.v)); }
		template<int x, int y, int z, int w>
		ALWAYS_INLINE simd blend(const simd& o) const { return simd(internal::blend<x, y, z, w>(v, o.v)); }

	public:
		__m128 v;
	};

	// Swizzling ----------------------
	template<int x, int y = 0, int z = 0, int w = 0>
	ALWAYS_INLINE simd<float, 4> shuffle(const simd<float, 4>& a) { return a.shuffle<x, y, z, w>(); }
	template<int x, int y = 0, int z = 0, int w = 0>
	ALWAYS_INLINE simd<float, 4> shuffle(const simd<float, 4>& a, const simd<float, 4>& b) { return a.shuffle<x, y, z, w>(b); }
	template<int x, int y, int z, int w>
	ALWAYS_INLINE simd<float, 4> blend(const simd<float, 4>& a, const simd<float, 4>& b) { return a.blend<x, y, z, w>(b); }
	// Logical Operations -------------
	ALWAYS_INLINE simd<float, 4> min(const simd<float, 4>& a, const simd<float, 4>& b) { return simd<float, 4>(_mm_min_ps(a.v, b.v)); }
	ALWAYS_INLINE simd<float, 4> max(const simd<float, 4>& a, const simd<float, 4>& b) { return simd<float, 4>(_mm_max_ps(a.v, b.v)); }
	ALWAYS_INLINE simd<float, 4> and_not(const simd<float, 4>& a, const simd<float, 4>& b) { return simd<float, 4>(_mm_andnot_ps(a.v, b.v)); }
	// Rounding -----------------------
	ALWAYS_INLINE simd<float, 4> trunc(const simd<float, 4>& a) { return simd<float, 4>(internal::trunc(a.v)); }
	ALWAYS_INLINE simd<float, 4> floor(const simd<float, 4>& a) { return simd<float, 4>(internal::floor(a.v)); }
	ALWAYS_INLINE simd<float, 4> ceil(const simd<float, 4>& a) { return simd<float, 4>(internal::ceil(a.v)); }
	ALWAYS_INLINE simd<float, 4> round(const simd<float, 4>& a) { return simd<float, 4>(internal::round(a.v)); }
	ALWAYS_INLINE simd<float, 4> fract(const simd<float, 4>& a) { return simd<float, 4>(_mm_sub_ps(a.v, internal::trunc(a.v))); }
	ALWAYS_INLINE simd<float, 4> abs(const simd<float, 4>& a) { return simd<float, 4>(_mm_andnot_ps(_mm_set_ps1(-0.0f), a.v)); }
	// Generic Math Operations --------
	ALWAYS_INLINE simd<float, 4> rcp(const simd<float, 4>& a) { return simd<float, 4>(_mm_rcp_ps(a.v)); }
	ALWAYS_INLINE simd<float, 4> sqrt(const simd<float, 4>& a) { return simd<float, 4>(_mm_sqrt_ps(a.v)); }
	ALWAYS_INLINE simd<float, 4> rcp_sqrt(const simd<float, 4>& a) { return simd<float, 4>(_mm_rsqrt_ps(a.v)); }
	ALWAYS_INLINE float hsum(const simd<float, 4>& a) { return a.hsum(); }
	ALWAYS_INLINE float dot(const simd<float, 4>& a, const simd<float, 4>& b) { return a.dot(b); }
	ALWAYS_INLINE int mask(const simd<float, 4>& a) { return _mm_movemask_ps(a.v); }
	ALWAYS_INLINE void transpose(simd<float, 4>& a, simd<float, 4>& b, simd<float, 4>& c, simd<float, 4>& d) { _MM_TRANSPOSE4_PS(a.v, b.v, c.v, d.v); }
	// Trigonometry -------------------
	ALWAYS_INLINE simd<float, 4> sin(const simd<float, 4>& a) { return simd<float, 4>(_mm_sin_ps(a.v)); }
	ALWAYS_INLINE simd<float, 4> cos(const simd<float, 4>& a) { return simd<float, 4>(_mm_cos_ps(a.v)); }
	ALWAYS_INLINE simd<float, 4> tan(const simd<float, 4>& a) { return simd<float, 4>(_mm_tan_ps(a.v)); }
	ALWAYS_INLINE simd<float, 4> asin(const simd<float, 4>& a) { return simd<float, 4>(_mm_asin_ps(a.v)); }
	ALWAYS_INLINE simd<float, 4> acos(const simd<float, 4>& a) { return simd<float, 4>(_mm_acos_ps(a.v)); }
	ALWAYS_INLINE simd<float, 4> atan(const simd<float, 4>& a) { return simd<float, 4>(_mm_atan_ps(a.v)); }
	ALWAYS_INLINE simd<float, 4> atan2(const simd<float, 4>& a, const simd<float, 4>& b) { return simd<float, 4>(_mm_atan2_ps(a.v, b.v)); }
}