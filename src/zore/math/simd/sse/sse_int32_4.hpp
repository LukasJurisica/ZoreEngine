#pragma once

#include "zore/math/simd/sse/sse_core.hpp"

namespace zm {

	//========================================================================
	//  simd SSE SIMD Vector
	//========================================================================

	template<>
	struct simd<int32_t, 4> : simd_base<int32_t, 4> {
	public:
		// Constructors -------------------
		ALWAYS_INLINE explicit simd() : v(_mm_setzero_si128()) {}
		ALWAYS_INLINE explicit simd(int32_t s) : v(_mm_set1_epi32(s)) {}
		ALWAYS_INLINE explicit simd(int32_t x, int32_t y, int32_t z, int32_t w) : v(_mm_set_epi32(w, z, y, x)) {}
		ALWAYS_INLINE explicit simd(const int32_t* o) { load(o); }
		ALWAYS_INLINE explicit simd(const __m128i& o) : v(o) {}
		ALWAYS_INLINE explicit simd(const simd<uint32_t, 4>& o) : v(reinterpret_cast<const __m128i&>(o)) {}
		ALWAYS_INLINE explicit simd(const simd<float, 4>& o) : v(_mm_cvttps_epi32(reinterpret_cast<const __m128&>(o))) {}
		ALWAYS_INLINE void load(const int32_t* p) { v = _mm_loadu_si128(reinterpret_cast<const __m128i*>(p)); }
		ALWAYS_INLINE void load_aligned(const int32_t* p) { v = _mm_load_si128(reinterpret_cast<const __m128i*>(p)); }
		ALWAYS_INLINE void unload(int32_t* p) const { _mm_storeu_si128(reinterpret_cast<__m128i*>(p), v); }
		ALWAYS_INLINE void unload_aligned(int32_t* p) const { _mm_store_si128(reinterpret_cast<__m128i*>(p), v); }
		operator const __m128i&() const { return v; }
		~simd() = default;

	public:
		// Comparison ---------------------
		ALWAYS_INLINE simd  operator== (const simd& o) const { return simd(_mm_cmpeq_epi32(v, o.v)); }
		ALWAYS_INLINE simd  operator!= (const simd& o) const { return simd(internal::bit_not(_mm_cmpeq_epi32(v, o.v))); }
		ALWAYS_INLINE simd  operator<  (const simd& o) const { return simd(_mm_cmplt_epi32(v, o.v)); }
		ALWAYS_INLINE simd  operator<= (const simd& o) const { return simd(internal::bit_not(_mm_cmpgt_epi32(v, o.v))); }
		ALWAYS_INLINE simd  operator>  (const simd& o) const { return simd(_mm_cmpgt_epi32(v, o.v)); }
		ALWAYS_INLINE simd  operator>= (const simd& o) const { return simd(internal::bit_not(_mm_cmplt_epi32(v, o.v))); }
		// Bit Operations -----------------
		ALWAYS_INLINE simd  operator<< (const int32_t s) const { return simd(_mm_slli_epi32(v, s)); }
		ALWAYS_INLINE simd  operator<<=(const int32_t s) { v = _mm_slli_epi32(v, s); return *this; }
		ALWAYS_INLINE simd  operator>> (const int32_t s) const { return simd(_mm_srli_epi32(v, s)); }
		ALWAYS_INLINE simd  operator>>=(const int32_t s) { v = _mm_srli_epi32(v, s); return *this; }
		template<zore::numeric T>
		ALWAYS_INLINE simd  operator&  (const T o) const { return simd(_mm_and_si128(v, _mm_set1_epi32(static_cast<int32_t>(o)))); }
		ALWAYS_INLINE simd  operator&  (const simd& o) const { return simd(_mm_and_si128(v, o.v)); }
		template<zore::numeric T>
		ALWAYS_INLINE simd& operator&= (const T o) { v = _mm_and_si128(v, _mm_set1_epi32(static_cast<int32_t>(o))); return *this; }
		ALWAYS_INLINE simd  operator&= (const simd& o) { v = _mm_and_si128(v, o.v); return *this; }
		template<zore::numeric T>
		ALWAYS_INLINE simd  operator|  (const T o) const { return simd(_mm_or_si128(v, _mm_set1_epi32(static_cast<int32_t>(o)))); }
		ALWAYS_INLINE simd  operator|  (const simd& o) const { return simd(_mm_or_si128(v, o.v)); }
		template<zore::numeric T>
		ALWAYS_INLINE simd& operator|= (const T o) { v = _mm_or_si128(v, _mm_set1_epi32(static_cast<int32_t>(o))); return *this; }
		ALWAYS_INLINE simd  operator|= (const simd& o) { v = _mm_or_si128(v, o.v); return *this; }
		template<zore::numeric T>
		ALWAYS_INLINE simd  operator^  (const T o) const { return simd(_mm_xor_si128(v, _mm_set1_epi32(static_cast<int32_t>(o)))); }
		ALWAYS_INLINE simd  operator^  (const simd& o) const { return simd(_mm_xor_si128(v, o.v)); }
		template<zore::numeric T>
		ALWAYS_INLINE simd& operator^= (const T o) { v = _mm_xor_si128(v, _mm_set1_epi32(static_cast<int32_t>(o))); return *this; }
		ALWAYS_INLINE simd  operator^= (const simd& o) { v = _mm_xor_si128(v, o.v); return *this; }
		ALWAYS_INLINE simd  operator~  () { return simd(internal::bit_not(v)); }
		// Arithmetic ---------------------
		template<zore::numeric T>
		ALWAYS_INLINE simd  operator+  (const T o) const { return simd(_mm_add_epi32(v, _mm_set1_epi32(static_cast<int32_t>(o)))); }
		ALWAYS_INLINE simd  operator+  () const { return simd(v); }
		ALWAYS_INLINE simd  operator+  (const simd& o) const { return simd(_mm_add_epi32(v, o.v)); }
		template<zore::numeric T>
		ALWAYS_INLINE simd& operator+= (const T o) { v = _mm_add_epi32(v, _mm_set1_epi32(static_cast<int32_t>(o))); return *this; }
		ALWAYS_INLINE simd& operator+= (const simd& o) { v = _mm_add_epi32(v, o.v); return *this; }
		template<zore::numeric T>
		ALWAYS_INLINE simd  operator-  (const T o) const { return simd(_mm_sub_epi32(v, _mm_set1_epi32(static_cast<int32_t>(o)))); }
		ALWAYS_INLINE simd  operator-  () const { return simd(_mm_sub_epi32(_mm_setzero_si128(), v)); }
		ALWAYS_INLINE simd  operator-  (const simd& o) const { return simd(_mm_sub_epi32(v, o.v)); }
		template<zore::numeric T>
		ALWAYS_INLINE simd& operator-= (const T o) { v = _mm_sub_epi32(v, _mm_set1_epi32(static_cast<int32_t>(o))); return *this; }
		ALWAYS_INLINE simd& operator-= (const simd& o) { v = _mm_sub_epi32(v, o.v); return *this; }
		template<zore::numeric T>
		ALWAYS_INLINE simd  operator*  (const T o) const { return simd(internal::mullo_32(v, _mm_set1_epi32(static_cast<int32_t>(o)))); }
		ALWAYS_INLINE simd  operator*  (const simd& o) const { return simd(internal::mullo_32(v, o.v)); }
		template<zore::numeric T>
		ALWAYS_INLINE simd& operator*= (const T o) { v = internal::mullo_32(v, _mm_set1_epi32(static_cast<int32_t>(o))); return *this; }
		ALWAYS_INLINE simd& operator*= (const simd& o) { v = internal::mullo_32(v, o.v); return *this; }
		template<zore::numeric T>
		ALWAYS_INLINE simd  operator/  (const T o) const { return simd(internal::div_i32(v, _mm_set1_epi32(static_cast<int32_t>(o)))); }
		ALWAYS_INLINE simd  operator/  (const simd& o) const { return simd(internal::div_i32(v, o.v)); }
		template<zore::numeric T>
		ALWAYS_INLINE simd& operator/= (const T o) { v = internal::div_i32(v, _mm_set1_epi32(static_cast<int32_t>(o))); return *this; }
		ALWAYS_INLINE simd& operator/= (const simd& o) { v = internal::div_i32(v, o.v); return *this; }
		// Other --------------------------
		ALWAYS_INLINE int32_t extract(int index) { return internal::extract(v, index); }
		ALWAYS_INLINE int32_t insert(int index, int32_t s) { v = internal::insert(v, index, s); }
		ALWAYS_INLINE int32_t hsum() const { return internal::hsum(v); }
		ALWAYS_INLINE int32_t dot(const simd& o) const { return internal::dot(v, o.v); }
		template<int x, int y = 0, int z = 0, int w = 0>
		ALWAYS_INLINE simd shuffle() const { return simd(internal::shuffle<x, y, z, w>(v)); }
		template<int x, int y = 0, int z = 0, int w = 0>
		ALWAYS_INLINE simd shuffle(const simd& o) const { return simd(internal::shuffle<x, y, z, w>(v, o.v)); }
		template<int x, int y, int z, int w>
		ALWAYS_INLINE simd blend(const simd& o) const { return simd(internal::blend<x, y, z, w>(v, o.v)); }
		ALWAYS_INLINE static constexpr int size() { return 4; }

	public:
		__m128i v;
	};

	// Swizzling ----------------------
	template<int x, int y = 0, int z = 0, int w = 0>
	ALWAYS_INLINE simd<int32_t, 4> shuffle(const simd<int32_t, 4>& a) { return a.shuffle<x, y, z, w>(); }
	template<int x, int y = 0, int z = 0, int w = 0>
	ALWAYS_INLINE simd<int32_t, 4> shuffle(const simd<int32_t, 4>& a, const simd<int32_t, 4>& b) { return a.shuffle<x, y, z, w>(b); }
	template<int x, int y, int z, int w>
	ALWAYS_INLINE simd<int32_t, 4> blend(const simd<int32_t, 4>& a, const simd<int32_t, 4>& b) { return a.blend<x, y, z, w>(b); }
	// Logical Operations -------------
	ALWAYS_INLINE simd<int32_t, 4> min(const simd<int32_t, 4>& a, const simd<int32_t, 4>& b) { return simd<int32_t, 4>(internal::min(a.v, b.v)); }
	ALWAYS_INLINE simd<int32_t, 4> max(const simd<int32_t, 4>& a, const simd<int32_t, 4>& b) { return simd<int32_t, 4>(internal::max(a.v, b.v)); }
	ALWAYS_INLINE simd<int32_t, 4> and_not(const simd<int32_t, 4>& a, const simd<int32_t, 4>& b) { return simd<int32_t, 4>(_mm_andnot_si128(a.v, b.v)); }
	// Rounding -----------------------
	ALWAYS_INLINE simd<int32_t, 4> abs(const simd<int32_t, 4>& a) { return simd<int32_t, 4>(internal::abs(a.v)); }
	// Generic Math Operations --------
	ALWAYS_INLINE int32_t hsum(const simd<int32_t, 4>& a) { return a.hsum(); }
	ALWAYS_INLINE int32_t dot(const simd<int32_t, 4>& a, const simd<int32_t, 4>& b) { return a.dot(b); }
	ALWAYS_INLINE int mask(const simd<int32_t, 4>& a) { return _mm_movemask_epi8(a.v); }
}