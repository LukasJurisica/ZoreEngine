#pragma once

#include "zore/utils/sized_integer.hpp"
#include "zore/utils/concepts.hpp"
#include "zore/platform.hpp"
#include <cmath>

namespace zm {

	//========================================================================
	//  4 Dimensional Vector
	//========================================================================

	template <zore::numeric T>
	struct alignas(sizeof(T) * 4) vec4_base {
	public:
		// Constructors -------------------
		ALWAYS_INLINE vec4_base() : data() {}
		template <zore::numeric U>
		ALWAYS_INLINE vec4_base(U s) : data(static_cast<T>(s), static_cast<T>(s), static_cast<T>(s), static_cast<T>(s)) {}
		template <zore::numeric U>
		ALWAYS_INLINE explicit vec4_base(U x, U y, U z, U w) : data(static_cast<T>(x), static_cast<T>(y), static_cast<T>(z), static_cast<T>(w)) {}
		template <zore::numeric U>
		ALWAYS_INLINE vec4_base(const vec4_base<U>& v) : vec4_base(v.data[0], v.data[1], v.data[2], v.data[3]) {}

	public:
		// Comparison ---------------------
		ALWAYS_INLINE bool operator== (const vec4_base<T>& v) const { return (x == v.x && y == v.y && z == v.z && w == v.w); }
		ALWAYS_INLINE bool operator!= (const vec4_base<T>& v) const { return (x != v.x || y != v.y || z != v.z || w != v.w); }
		// Bit Operations -----------------
		ALWAYS_INLINE vec4_base<T>  operator>> (const vec4_base<T>& v) const requires std::integral<T> { return vec4_base<T>(x >> v.x, y >> v.y, z >> v.z, w >> v.w); }
		ALWAYS_INLINE vec4_base<T>& operator>>=(const vec4_base<T>& v)       requires std::integral<T> { x >>= v.x; y >>= v.y; z >>= v.z; w >>= v.w; return *this; }
		ALWAYS_INLINE vec4_base<T>  operator<< (const vec4_base<T>& v) const requires std::integral<T> { return vec4_base<T>(x << v.x, y << v.y, z << v.z, w << v.w); }
		ALWAYS_INLINE vec4_base<T>& operator<<=(const vec4_base<T>& v)       requires std::integral<T> { x <<= v.x; y <<= v.y; z <<= v.z; w <<= v.w; return *this; }
		ALWAYS_INLINE vec4_base<T>  operator|  (const vec4_base<T>& v) const requires std::integral<T> { return vec4_base<T>(x | v.x, y | v.y, z | v.z, w | v.w); }
		ALWAYS_INLINE vec4_base<T>& operator|= (const vec4_base<T>& v)       requires std::integral<T> { x |= v.x; y |= v.y; z |= v.z; w |= v.w; return *this; }
		ALWAYS_INLINE vec4_base<T>  operator&  (const vec4_base<T>& v) const requires std::integral<T> { return vec4_base<T>(x & v.x, y & v.y, z & v.z, w & v.w); }
		ALWAYS_INLINE vec4_base<T>& operator&= (const vec4_base<T>& v)       requires std::integral<T> { x &= v.x; y &= v.y; z &= v.z; w &= v.w; return *this; }
		ALWAYS_INLINE vec4_base<T>  operator^  (const vec4_base<T>& v) const requires std::integral<T> { return vec4_base<T>(x ^ v.x, y ^ v.y, z ^ v.z, w ^ v.w); }
		ALWAYS_INLINE vec4_base<T>& operator^= (const vec4_base<T>& v)       requires std::integral<T> { x ^= v.x; y ^= v.y; z ^= v.z; w ^= v.w; return *this; }
		// Arithmetic ---------------------
		ALWAYS_INLINE vec4_base<T>  operator+  () { return *this; }
		ALWAYS_INLINE vec4_base<T>  operator+  (const vec4_base<T>& v) const { return vec4_base<T>(x + v.x, y + v.y, z + v.z, w + v.w); }
		ALWAYS_INLINE vec4_base<T>& operator+= (const vec4_base<T>& v) { x += v.x; y += v.y; z += v.z; w += v.w; return *this; }
		ALWAYS_INLINE vec4_base<T>  operator-  () const { return vec4_base<T>(-x, -y, -z, -w); }
		ALWAYS_INLINE vec4_base<T>  operator-  (const vec4_base<T>& v) const { return vec4_base<T>(x - v.x, y - v.y, z - v.z, w - v.w); }
		ALWAYS_INLINE vec4_base<T>& operator-= (const vec4_base<T>& v) { x -= v.x; y -= v.y; z -= v.z; w -= v.w; return *this; }
		ALWAYS_INLINE vec4_base<T>  operator*  (const vec4_base<T>& v) const { return vec4_base<T>(x * v.x, y * v.y, z * v.z, w * v.w); }
		ALWAYS_INLINE vec4_base<T>& operator*= (const vec4_base<T>& v) { x *= v.x; y *= v.y; z *= v.z; w *= v.w; return *this; }
		ALWAYS_INLINE vec4_base<T>  operator/  (const vec4_base<T>& v) const { return vec4_base<T>(x / v.x, y / v.y, z / v.z, w / v.w); }
		ALWAYS_INLINE vec4_base<T>& operator/= (const vec4_base<T>& v) { x /= v.x; y /= v.y; z /= v.z; w /= v.w; return *this; }
		ALWAYS_INLINE vec4_base<T>  operator%  (const vec4_base<T>& v) const requires std::integral<T> { return vec4_base<T>(x % v.x, y % v.y, z % v.z, w % v.w); }
		ALWAYS_INLINE vec4_base<T>& operator%= (const vec4_base<T>& v)       requires std::integral<T> { x %= v.x; y %= v.y; z %= v.z; w %= v.w; return *this; }
		// Other --------------------------
		ALWAYS_INLINE       T& operator[](int index) { return data[index]; }
		ALWAYS_INLINE const T& operator[](int index) const { return data[index]; }
		ALWAYS_INLINE T Sum() const { return x + y + z + w; }
		ALWAYS_INLINE T Dot(const vec4_base<T>& v) const { return ((*this) * v).Sum(); }
		ALWAYS_INLINE float Length() const { return std::sqrtf(static_cast<float>(Dot(*this))); }
		ALWAYS_INLINE vec4_base<T>& Normalize() requires std::floating_point<T> { (*this) /= Length(); return *this; }

	public:
		// Data ---------------------------
		union {
			struct { T x, y, z, w; };
			struct { T r, g, b, a; };
			T data[4];
		};
	};

	//========================================================================
	// Static Operators
	//========================================================================

	template <zore::numeric T>
	ALWAYS_INLINE T Dot(const vec4_base<T>& a, const vec4_base<T>& b) { return a.Dot(b); }
	template <zore::numeric T>
	ALWAYS_INLINE float Length(const vec4_base<T>& v) { return v.Length(); }
	template <zore::numeric T>
	ALWAYS_INLINE float Distance(const vec4_base<T>& a, const vec4_base<T>& b) { return Length(a - b); }
	template <std::floating_point T>
	ALWAYS_INLINE vec4_base<T> Normalize(const vec4_base<T>& v) { return v / v.Length(); }
	template <std::floating_point T>
	ALWAYS_INLINE vec4_base<T> Floor(const vec4_base<T>& v) { return vec4_base<T>(std::floor(v.x), std::floor(v.y), std::floor(v.z), std::floor(v.w)); }
	template <std::floating_point T>
	ALWAYS_INLINE vec4_base<T> Ceil(const vec4_base<T>& v) { return vec4_base<T>(std::ceil(v.x), std::ceil(v.y), std::ceil(v.z), std::ceil(v.w)); }
	template <std::floating_point T>
	ALWAYS_INLINE vec4_base<T> Round(const vec4_base<T>& v) { return vec4_base<T>(std::round(v.x), std::round(v.y), std::round(v.z), std::round(v.w)); }
	template <std::floating_point T>
	ALWAYS_INLINE vec4_base<T> Trunc(const vec4_base<T>& v) { return vec4_base<T>(std::trunc(v.x), std::trunc(v.y), std::trunc(v.z), std::trunc(v.w)); }
	template <std::floating_point T>
	ALWAYS_INLINE vec4_base<T> Fract(const vec4_base<T>& v) { return v - Trunc(v); }

	//========================================================================
	//  Predefined Named Types
	//========================================================================

	typedef vec4_base<float>     vec4;
	typedef vec4_base<int32_t>  ivec4;
	typedef vec4_base<uint32_t> uvec4;
	typedef vec4_base<double>   dvec4;
}

#include "zore/math/vector/simd_vec4.inl"