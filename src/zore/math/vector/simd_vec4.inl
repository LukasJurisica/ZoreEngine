#pragma once

#ifdef __INTELLISENSE__
#include "zore/math/vector/vec4.hpp"
#endif

#include "zore/math/simd/simd_float32_4.hpp"
#include "zore/math/simd/simd_int32_4.hpp"
#include "zore/math/simd/simd_uint32_4.hpp"

namespace zm {

	//========================================================================
	//  Vector 4 with SIMD backing
	//========================================================================

	template <zore::numeric T>
		requires simd_enabled<T, 4>
	struct alignas(sizeof(T) * 4) vec_base<T, 4> {
	public:
		// Constructors -------------------
		ALWAYS_INLINE vec_base() : v() {}
		template <zore::numeric U>
		ALWAYS_INLINE vec_base(U s) : v(static_cast<T>(s)) {}
		template <zore::numeric U, zore::numeric V, zore::numeric W, zore::numeric X>
		ALWAYS_INLINE vec_base(U x, V y, W z, X w) : data(static_cast<T>(x), static_cast<T>(y), static_cast<T>(z), static_cast<T>(w)) {}
		template <zore::numeric U, zore::numeric V>
		ALWAYS_INLINE vec_base(U x, const vec_base<V, 3>& yzw) : data(static_cast<T>(x), static_cast<T>(yzw[0]), static_cast<T>(yzw[1]), static_cast<T>(yzw[2])) {}
		template <zore::numeric U, zore::numeric V>
		ALWAYS_INLINE vec_base(const vec_base<V, 3>& xyz, U w) : data(static_cast<T>(xyz[0]), static_cast<T>(xyz[1]), static_cast<T>(xyz[2]), static_cast<T>(w)) {}
		template <zore::numeric U, zore::numeric V, zore::numeric W>
		ALWAYS_INLINE vec_base(const vec_base<U, 2>& xy, V z, W w) : data(static_cast<T>(xy[0]), static_cast<T>(xy[1]), static_cast<T>(z), static_cast<T>(w)) {}
		template <zore::numeric U, zore::numeric V, zore::numeric W>
		ALWAYS_INLINE vec_base(U x, const vec_base<V, 2>& yz, W w) : data(static_cast<T>(x), static_cast<T>(yz[0]), static_cast<T>(yz[1]), static_cast<T>(w)) {}
		template <zore::numeric U, zore::numeric V, zore::numeric W>
		ALWAYS_INLINE vec_base(U x, V y, const vec_base<W, 2>& zw) : data(static_cast<T>(x), static_cast<T>(y), static_cast<T>(zw[0]), static_cast<T>(zw[1])) {}
		template <zore::numeric U, zore::numeric V>
		ALWAYS_INLINE vec_base(const vec_base<U, 2>& xy, const vec_base<V, 2>& zw) : data{ static_cast<T>(xy[0]), static_cast<T>(xy[1]), static_cast<T>(zw[0]), static_cast<T>(zw[1]) } {}
		template <zore::numeric U>
		ALWAYS_INLINE vec_base(const vec_base<U, 4>& o) {
			if constexpr (simd_enabled<U, 4>)
				v = simd<T, 4>(o.v);
			else
				v = { o.x, o.y, o.z, o.w };
		}
		ALWAYS_INLINE vec_base(const simd<T, 4>& o) : v(o) {}

	public:
		// Comparison ---------------------
		ALWAYS_INLINE bool operator== (const vec_base& o) const { return mask(v == o.v) == 0xFFFF; }
		ALWAYS_INLINE bool operator!= (const vec_base& o) const { return mask(v == o.v) != 0xFFFF; }
		// Bit Operations -----------------
		ALWAYS_INLINE vec_base  operator>> (T s) const requires std::integral<T> { return { v >> s }; }
		ALWAYS_INLINE vec_base& operator>>=(T s)       requires std::integral<T> { v >>= s; return *this; }
		ALWAYS_INLINE vec_base  operator<< (T s) const requires std::integral<T> { return { v << s }; }
		ALWAYS_INLINE vec_base& operator<<=(T s)       requires std::integral<T> { v <<= s; return *this; }
		// Arithmetic ---------------------
		ALWAYS_INLINE vec_base& operator+  () const { return *this; }
		ALWAYS_INLINE vec_base  operator+  (const vec_base& o) const { return { v + o.v }; }
		ALWAYS_INLINE vec_base& operator+= (const vec_base& o) { v += o.v; return *this; }
		ALWAYS_INLINE vec_base  operator-  () const { return -v; }
		ALWAYS_INLINE vec_base  operator-  (const vec_base& o) const { return { v - o.v }; }
		ALWAYS_INLINE vec_base& operator-= (const vec_base& o) { v -= o.v; return *this; }
		ALWAYS_INLINE vec_base  operator*  (const vec_base& o) const { return { v * o.v }; }
		ALWAYS_INLINE vec_base& operator*= (const vec_base& o) { v *= o.v; return *this; }
		ALWAYS_INLINE vec_base  operator/  (const vec_base& o) const { return { v / o.v }; }
		ALWAYS_INLINE vec_base& operator/= (const vec_base& o) { v /= o.v; return *this; }
		// Other --------------------------
		ALWAYS_INLINE       T& operator[](int index) { return data[index]; }
		ALWAYS_INLINE const T& operator[](int index) const { return data[index]; }
		ALWAYS_INLINE T Sum() const { return v.hsum(); }
		ALWAYS_INLINE T Dot(const vec_base& o) const { return v.dot(o.v); }
		ALWAYS_INLINE float Length() const { return std::sqrtf(static_cast<float>(v.dot(v))); }
		ALWAYS_INLINE vec_base& Normalize() requires std::floating_point<T> { (*this) *= (1.f / Length()); return *this; }

	public:
		// Data ---------------------------
		union {
			struct { T x, y, z, w; };
			struct { T r, g, b, a; };
			T data[4];
			simd<T, 4> v;
		};
	};
}