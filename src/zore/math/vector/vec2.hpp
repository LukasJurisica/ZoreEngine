#pragma once

#include "zore/math/vector/vec_base.hpp"

namespace zm {

	//========================================================================
	//  2 Dimensional Vector
	//========================================================================

	template <zore::numeric T>
	struct alignas(sizeof(T) * 2) vec_base<T, 2> {
	public:
		// Constructors -------------------
		ALWAYS_INLINE vec_base() : data{} {}
		template <zore::numeric U>
		ALWAYS_INLINE vec_base(U s) : data{ static_cast<T>(s), static_cast<T>(s) } {}
		template <zore::numeric U, zore::numeric V>
		ALWAYS_INLINE vec_base(U x, V y) : data{ static_cast<T>(x), static_cast<T>(y) } {}
		template <zore::numeric U>
		ALWAYS_INLINE vec_base(const vec_base<U, 2>& v) : data{ static_cast<T>(v.data[0]), static_cast<T>(v.data[1]) } {}
	public:
		// Comparison ---------------------
		ALWAYS_INLINE bool operator== (const vec_base& v) const { return (x == v.x && y == v.y); }
		ALWAYS_INLINE bool operator!= (const vec_base& v) const { return (x != v.x || y != v.y); }
		// Bit Operations -----------------
		ALWAYS_INLINE vec_base  operator>> (T s) const requires std::integral<T> { return { x >> s, y >> s }; }
		ALWAYS_INLINE vec_base& operator>>=(T s)       requires std::integral<T> { x >>= s; y >>= s; return *this; }
		ALWAYS_INLINE vec_base  operator<< (T s) const requires std::integral<T> { return { x << s, y << s }; }
		ALWAYS_INLINE vec_base& operator<<=(T s)       requires std::integral<T> { x <<= s; y <<= s; return *this; }
		// Arithmetic ---------------------
		ALWAYS_INLINE vec_base  operator+  () const { return *this; }
		ALWAYS_INLINE vec_base  operator+  (const vec_base& v) const { return { x + v.x, y + v.y }; }
		ALWAYS_INLINE vec_base& operator+= (const vec_base& v) { x += v.x; y += v.y; return *this; }
		ALWAYS_INLINE vec_base  operator-  () const { return { -x, -y }; }
		ALWAYS_INLINE vec_base  operator-  (const vec_base& v) const { return { x - v.x, y - v.y }; }
		ALWAYS_INLINE vec_base& operator-= (const vec_base& v) { x -= v.x; y -= v.y; return *this; }
		ALWAYS_INLINE vec_base  operator*  (const vec_base& v) const { return { x * v.x, y * v.y }; }
		ALWAYS_INLINE vec_base& operator*= (const vec_base& v) { x *= v.x; y *= v.y; return *this; }
		ALWAYS_INLINE vec_base  operator/  (const vec_base& v) const { return { x / v.x, y / v.y }; }
		ALWAYS_INLINE vec_base& operator/= (const vec_base& v) { x /= v.x; y /= v.y; return *this; }
		// Other --------------------------
		ALWAYS_INLINE       T& operator[](int index)       { return data[index]; }
		ALWAYS_INLINE const T& operator[](int index) const { return data[index]; }
		ALWAYS_INLINE T Sum() const { return x + y; }
		ALWAYS_INLINE T Dot(const vec_base& v) const { return ((*this) * v).Sum(); }
		ALWAYS_INLINE float Length() const { return std::sqrtf(static_cast<float>(Dot(*this))); }
		ALWAYS_INLINE vec_base& Normalize() requires std::floating_point<T> { (*this) *= (1.f / Length()); return *this; }

	public:
		// Data ---------------------------
		union {
			struct { T x, y; };
			struct { T u, v; };
			T data[2];
		};
	};

	//========================================================================
	// Static Operators
	//========================================================================

	template <zore::numeric T>
	ALWAYS_INLINE T Dot(const vec_base<T, 2>& a, const vec_base<T, 2>& b) { return a.Dot(b); }
	template <zore::numeric T>
	ALWAYS_INLINE float Length(const vec_base<T, 2>& v) { return v.Length(); }
	template <zore::numeric T>
	ALWAYS_INLINE float Distance(const vec_base<T, 2>& a, const vec_base<T, 2>& b) { return Length(a - b); }
	template <std::floating_point T>
	ALWAYS_INLINE vec_base<T, 2> Normalize(const vec_base<T, 2>& v) { return v * (1.f / v.Length()); }
	template <std::floating_point T>
	ALWAYS_INLINE vec_base<T, 2> Floor(const vec_base<T, 2>& v) { return { std::floor(v.x), std::floor(v.y) }; }
	template <std::floating_point T>
	ALWAYS_INLINE vec_base<T, 2> Ceil(const vec_base<T, 2>& v) { return { std::ceil(v.x), std::ceil(v.y) }; }
	template <std::floating_point T>
	ALWAYS_INLINE vec_base<T, 2> Round(const vec_base<T, 2>& v) { return { std::round(v.x), std::round(v.y) }; }
	template <std::floating_point T>
	ALWAYS_INLINE vec_base<T, 2> Trunc(const vec_base<T, 2>& v) { return { std::trunc(v.x), std::trunc(v.y) }; }
	template <std::floating_point T>
	ALWAYS_INLINE vec_base<T, 2> Fract(const vec_base<T, 2>& v) { return v - Trunc(v); }

	//========================================================================
	//  Predefined Named Types
	//========================================================================

	typedef vec_base<float, 2>     vec2;
	typedef vec_base<bool, 2>     bvec2;
	typedef vec_base<int32_t, 2>  ivec2;
	typedef vec_base<uint32_t, 2> uvec2;
	typedef vec_base<double, 2>   dvec2;

	template <zore::numeric T>
	std::ostream& operator<<(std::ostream& os, const vec_base<T, 2>& v) {
		return os << "[" << v.x << ", " << v.y << "]";
	}
}



//template <int x, int y>
//ALWAYS_INLINE vec_base<T, 2> swizzle() {
//	static_assert(x >= 0 && x < 2 && y >= 0 && y < 2);
//	return vec_base<T, 2>(data[x], data[y]);
//}
//template <int x, int y, int z>
//ALWAYS_INLINE vec_base<T, 3> swizzle() {
//	static_assert(x >= 0 && x < 2 && y >= 0 && y < 2 && z >= 0 && z < 2);
//	return vec_base<T, 3>(data[x], data[y], data[z]);
//}
//template <int x, int y, int z, int w>
//ALWAYS_INLINE vec_base<T, 4> swizzle() {
//	static_assert(x >= 0 && x < 2 && y >= 0 && y < 2 && z >= 0 && z < 2 && w >= 0 && w < 2);
//	return vec_base<T, 4>(data[x], data[y], data[z], data[w]);
//}