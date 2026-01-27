#pragma once

#include "zore/math/vector/vec_base.hpp"

namespace zm {

	//========================================================================
	//  3 Dimensional Vector
	//========================================================================

	template <zore::numeric T>
	struct vec_base<T, 3> {
	public:
		// Constructors -------------------
		ALWAYS_INLINE vec_base() : data() {}
		template <zore::numeric U>
		ALWAYS_INLINE vec_base(U s) : data(static_cast<T>(s), static_cast<T>(s), static_cast<T>(s)) {}
		template <zore::numeric U, zore::numeric V, zore::numeric W>
		ALWAYS_INLINE vec_base(U x, V y, W z) : data(static_cast<T>(x), static_cast<T>(y), static_cast<T>(z)) {}
		template <zore::numeric U, zore::numeric V>
		ALWAYS_INLINE vec_base(U x, const vec_base<V, 2>& yz) : data(static_cast<T>(x), static_cast<T>(yz[0]), static_cast<T>(yz[1])) {}
		template <zore::numeric U, zore::numeric V>
		ALWAYS_INLINE vec_base(const vec_base<U, 2>& xy, V z) : data(static_cast<T>(xy[0]), static_cast<T>(xy[1]), static_cast<T>(z)) {}
		template <zore::numeric U>
		ALWAYS_INLINE vec_base(const vec_base<U, 3>& v) : data(static_cast<T>(v.data[0]), static_cast<T>(v.data[1]), static_cast<T>(v.data[2])) {}

	public:
		// Comparison ---------------------
		ALWAYS_INLINE bool operator== (const vec_base& v) const { return (x == v.x && y == v.y && z == v.z); }
		ALWAYS_INLINE bool operator!= (const vec_base& v) const { return (x != v.x || y != v.y || z != v.z); }
		// Bit Operations -----------------
		ALWAYS_INLINE vec_base  operator>> (T s) const requires std::integral<T> { return { x >> s, y >> s, z >> s }; }
		ALWAYS_INLINE vec_base& operator>>=(T s)       requires std::integral<T> { x >>= s; y >>= s; z >>= s; return *this; }
		ALWAYS_INLINE vec_base  operator<< (T s) const requires std::integral<T> { return { x << s, y << s, z << s }; }
		ALWAYS_INLINE vec_base& operator<<=(T s)       requires std::integral<T> { x <<= s; y <<= s; z <<= s; return *this; }
		// Arithmetic ---------------------
		ALWAYS_INLINE vec_base  operator+  () const { return *this; }
		ALWAYS_INLINE vec_base  operator+  (const vec_base& v) const { return { x + v.x, y + v.y, z + v.z }; }
		ALWAYS_INLINE vec_base& operator+= (const vec_base& v) { x += v.x; y += v.y; z += v.z; return *this; }
		ALWAYS_INLINE vec_base  operator-  () const { return { -x, -y, -z }; }
		ALWAYS_INLINE vec_base  operator-  (const vec_base& v) const { return { x - v.x, y - v.y, z - v.z }; }
		ALWAYS_INLINE vec_base& operator-= (const vec_base& v) { x -= v.x; y -= v.y; z -= v.z; return *this; }
		ALWAYS_INLINE vec_base  operator*  (const vec_base& v) const { return { x * v.x, y * v.y, z * v.z }; }
		ALWAYS_INLINE vec_base& operator*= (const vec_base& v) { x *= v.x; y *= v.y; z *= v.z; return *this; }
		ALWAYS_INLINE vec_base  operator/  (const vec_base& v) const { return { x / v.x, y / v.y, z / v.z }; }
		ALWAYS_INLINE vec_base& operator/= (const vec_base& v) { x /= v.x; y /= v.y; z /= v.z; return *this; }
		// Other --------------------------
		ALWAYS_INLINE       T& operator[](int index) { return data[index]; }
		ALWAYS_INLINE const T& operator[](int index) const { return data[index]; }
		ALWAYS_INLINE T Sum() const { return x + y + z; }
		ALWAYS_INLINE T Dot(const vec_base& v) const { return ((*this) * v).Sum(); }
		ALWAYS_INLINE float Length() const { return std::sqrtf(static_cast<float>(Dot(*this))); }
		ALWAYS_INLINE vec_base& Normalize() requires std::floating_point<T> { (*this) *= (1.f / Length()); return *this; }
		ALWAYS_INLINE vec_base Cross(const vec_base& v) const { return { (y * v.z) - (z * v.y), (z * v.x) - (x * v.z), (x * v.y) - (y * v.x) }; }

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
	ALWAYS_INLINE T Dot(const vec_base<T, 3>& a, const vec_base<T, 3>& b) { return a.Dot(b); }
	template <zore::numeric T>
	ALWAYS_INLINE float Length(const vec_base<T, 3>& v) { return v.Length(); }
	template <zore::numeric T>
	ALWAYS_INLINE float Distance(const vec_base<T, 3>& a, const vec_base<T, 3>& b) { return Length(a - b); }
	template <zore::numeric T>
	ALWAYS_INLINE vec_base<T, 3> Cross(const vec_base<T, 3>& a, const vec_base<T, 3>& b) { return a.Cross(b); }
	template <std::floating_point T>
	ALWAYS_INLINE vec_base<T, 3> Normalize(const vec_base<T, 3>& v) { return v / v.Length(); }
	template <std::floating_point T>
	ALWAYS_INLINE vec_base<T, 3> Floor(const vec_base<T, 3>& v) { return { std::floor(v.x), std::floor(v.y), std::floor(v.z) }; }
	template <std::floating_point T>
	ALWAYS_INLINE vec_base<T, 3> Ceil(const vec_base<T, 3>& v) { return { std::ceil(v.x), std::ceil(v.y), std::ceil(v.z) }; }
	template <std::floating_point T>
	ALWAYS_INLINE vec_base<T, 3> Round(const vec_base<T, 3>& v) { return { std::round(v.x), std::round(v.y), std::round(v.z) }; }
	template <std::floating_point T>
	ALWAYS_INLINE vec_base<T, 3> Trunc(const vec_base<T, 3>& v) { return { std::trunc(v.x), std::trunc(v.y), std::trunc(v.z) }; }
	template <std::floating_point T>
	ALWAYS_INLINE vec_base<T, 3> Fract(const vec_base<T, 3>& v) { return v - Trunc(v); }

	//========================================================================
	//  Predefined Named Types
	//========================================================================

	typedef vec_base<float, 3>     vec3;
	typedef vec_base<bool, 3>     bvec3;
	typedef vec_base<int32_t, 3>  ivec3;
	typedef vec_base<uint32_t, 3> uvec3;
	typedef vec_base<double, 3>   dvec3;

	template <zore::numeric T>
	std::ostream& operator<<(std::ostream& os, const vec_base<T, 3>& v) {
		return os << "[" << v.x << ", " << v.y << ", " << v.z << "]";
	}
}