#pragma once

#include "zore/utils/sized_integer.hpp"
#include "zore/utils/concepts.hpp"
#include "zore/platform.hpp"
#include <cmath>

namespace zm {

	//========================================================================
	//  3 Dimensional Vector
	//========================================================================

	template <zore::numeric T>
	struct vec3_base {
	public:
		// Constructors -------------------
		ALWAYS_INLINE explicit vec3_base() : data() {}
		template <zore::numeric U>
		ALWAYS_INLINE vec3_base(U s) : data(static_cast<T>(s), static_cast<T>(s), static_cast<T>(s)) {}
		template <zore::numeric U>
		ALWAYS_INLINE explicit vec3_base(U x, U y, U z) : data(static_cast<T>(x), static_cast<T>(y), static_cast<T>(z)) {}
		template <zore::numeric U>
		ALWAYS_INLINE vec3_base(const vec3_base<U>& v) : vec3_base(v.data[0], v.data[1], v.data[2]) {}

	public:
		// Comparison ---------------------
		ALWAYS_INLINE bool operator== (const vec3_base<T>& v) const { return (x == v.x && y == v.y && z == v.z); }
		ALWAYS_INLINE bool operator!= (const vec3_base<T>& v) const { return (x != v.x || y != v.y || z == v.z); }
		// Bit Operations -----------------
		ALWAYS_INLINE vec3_base<T>  operator>> (const vec3_base<T>& v) const requires std::integral<T> { return vec3_base<T>(x >> v.x, y >> v.y, z >> v.z); }
		ALWAYS_INLINE vec3_base<T>& operator>>=(const vec3_base<T>& v)       requires std::integral<T> { x >>= v.x; y >>= v.y; z >>= v.z; return *this; }
		ALWAYS_INLINE vec3_base<T>  operator<< (const vec3_base<T>& v) const requires std::integral<T> { return vec3_base<T>(x << v.x, y << v.y, z << v.z); }
		ALWAYS_INLINE vec3_base<T>& operator<<=(const vec3_base<T>& v)       requires std::integral<T> { x <<= v.x; y <<= v.y; z <<= v.z; return *this; }
		ALWAYS_INLINE vec3_base<T>  operator|  (const vec3_base<T>& v) const requires std::integral<T> { return vec3_base<T>(x | v.x, y | v.y, z | v.z); }
		ALWAYS_INLINE vec3_base<T>& operator|= (const vec3_base<T>& v)       requires std::integral<T> { x |= v.x; y |= v.y; z |= v.z; return *this; }
		ALWAYS_INLINE vec3_base<T>  operator&  (const vec3_base<T>& v) const requires std::integral<T> { return vec3_base<T>(x & v.x, y & v.y, z & v.z); }
		ALWAYS_INLINE vec3_base<T>& operator&= (const vec3_base<T>& v)       requires std::integral<T> { x &= v.x; y &= v.y; z &= v.z; return *this; }
		ALWAYS_INLINE vec3_base<T>  operator^  (const vec3_base<T>& v) const requires std::integral<T> { return vec3_base<T>(x ^ v.x, y ^ v.y, z ^ v.z); }
		ALWAYS_INLINE vec3_base<T>& operator^= (const vec3_base<T>& v)       requires std::integral<T> { x ^= v.x; y ^= v.y; z ^= v.z; return *this; }
		// Arithmetic ---------------------
		ALWAYS_INLINE vec3_base<T>  operator+  () { return *this; }
		ALWAYS_INLINE vec3_base<T>  operator+  (const vec3_base<T>& v) const { return vec3_base<T>(x + v.x, y + v.y, z + v.z); }
		ALWAYS_INLINE vec3_base<T>& operator+= (const vec3_base<T>& v) { x += v.x; y += v.y; z += v.z; return *this; }
		ALWAYS_INLINE vec3_base<T>  operator-  () const { return vec3_base<T>(-x, -y, -z); }
		ALWAYS_INLINE vec3_base<T>  operator-  (const vec3_base<T>& v) const { return vec3_base<T>(x - v.x, y - v.y, z - v.z); }
		ALWAYS_INLINE vec3_base<T>& operator-= (const vec3_base<T>& v) { x -= v.x; y -= v.y; z -= v.z; return *this; }
		ALWAYS_INLINE vec3_base<T>  operator*  (const vec3_base<T>& v) const { return vec3_base<T>(x * v.x, y * v.y, z * v.z); }
		ALWAYS_INLINE vec3_base<T>& operator*= (const vec3_base<T>& v) { x *= v.x; y *= v.y; z *= v.z; return *this; }
		ALWAYS_INLINE vec3_base<T>  operator/  (const vec3_base<T>& v) const { return vec3_base<T>(x / v.x, y / v.y, z / v.z); }
		ALWAYS_INLINE vec3_base<T>& operator/= (const vec3_base<T>& v) { x /= v.x; y /= v.y; z /= v.z; return *this; }
		ALWAYS_INLINE vec3_base<T>  operator%  (const vec3_base<T>& v) const requires std::integral<T> { return vec3_base<T>(x % v.x, y % v.y, z % v.z); }
		ALWAYS_INLINE vec3_base<T>& operator%= (const vec3_base<T>& v)       requires std::integral<T> { x %= v.x; y %= v.y; z %= v.z; return *this; }
		// Other --------------------------
		ALWAYS_INLINE       T& operator[](int index) { return data[index]; }
		ALWAYS_INLINE const T& operator[](int index) const { return data[index]; }
		ALWAYS_INLINE T Sum() const { return x + y + z; }
		ALWAYS_INLINE T Dot(const vec3_base<T>& v) const { return ((*this) * v).Sum(); }
		ALWAYS_INLINE float Length() const { return std::sqrtf(static_cast<float>(Dot(*this))); }
		ALWAYS_INLINE vec3_base<T>& Normalize() requires std::floating_point<T> { (*this) /= Length(); return *this; }
		ALWAYS_INLINE vec3_base<T> Cross(const vec3_base<T>& v) const { return vec3_base<T>((y * v.z) - (z * v.y), (z * v.x) - (x * v.z), (x * v.y) - (y * v.x)); }

	public:
		// Data ---------------------------
		union {
			struct { T x, y, z; };
			struct { T r, g, b; };
			T data[3];
		};
	};

	//========================================================================
	// Static Operators
	//========================================================================

	template <zore::numeric T>
	ALWAYS_INLINE T Dot(const vec3_base<T>& a, const vec3_base<T>& b) { return a.Dot(b); }
	template <zore::numeric T>
	ALWAYS_INLINE float Length(const vec3_base<T>& v) { return v.Length(); }
	template <zore::numeric T>
	ALWAYS_INLINE float Distance(const vec3_base<T>& a, const vec3_base<T>& b) { return Length(a - b); }
	template <zore::numeric T>
	ALWAYS_INLINE vec3_base<T> Cross(const vec3_base<T>& a, const vec3_base<T>& b) { return a.Cross(b); }
	template <std::floating_point T>
	ALWAYS_INLINE vec3_base<T> Normalize(const vec3_base<T>& v) { return v / v.Length(); }
	template <std::floating_point T>
	ALWAYS_INLINE vec3_base<T> Floor(const vec3_base<T>& v) { return vec3_base<T>(std::floor(v.x), std::floor(v.y), std::floor(v.z)); }
	template <std::floating_point T>
	ALWAYS_INLINE vec3_base<T> Ceil(const vec3_base<T>& v) { return vec3_base<T>(std::ceil(v.x), std::ceil(v.y), std::ceil(v.z)); }
	template <std::floating_point T>
	ALWAYS_INLINE vec3_base<T> Round(const vec3_base<T>& v) { return vec3_base<T>(std::round(v.x), std::round(v.y), std::round(v.z)); }
	template <std::floating_point T>
	ALWAYS_INLINE vec3_base<T> Trunc(const vec3_base<T>& v) { return vec3_base<T>(std::trunc(v.x), std::trunc(v.y), std::trunc(v.z)); }
	template <std::floating_point T>
	ALWAYS_INLINE vec3_base<T> Fract(const vec3_base<T>& v) { return v - Trunc(v); }

	//========================================================================
	//  Predefined Named Types
	//========================================================================

	typedef vec3_base<float>     vec3;
	typedef vec3_base<bool>     bvec3;
	typedef vec3_base<int32_t>  ivec3;
	typedef vec3_base<uint32_t> uvec3;
	typedef vec3_base<double>   dvec3;
}