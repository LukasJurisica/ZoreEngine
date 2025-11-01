#pragma once

#include "zore/utils/sized_integer.hpp"
#include "zore/utils/concepts.hpp"
#include "zore/platform.hpp"
#include <cmath>

namespace zm {

	//========================================================================
	//  2 Dimensional Vector
	//========================================================================

	template <zore::numeric T>
	struct alignas(sizeof(T) * 2) vec2_base {
	public:
		// Constructors -------------------
		ALWAYS_INLINE vec2_base() : data() {}
		template <zore::numeric U>
		ALWAYS_INLINE vec2_base(U s) : data(static_cast<T>(s), static_cast<T>(s)) {}
		template <zore::numeric U>
		ALWAYS_INLINE explicit vec2_base(U x, U y) : data(static_cast<T>(x), static_cast<T>(y)) {}
		template <zore::numeric U>
		ALWAYS_INLINE vec2_base(const vec2_base<U>& v) : vec2_base(v.data[0], v.data[1]) {}

	public:
		// Comparison ---------------------
		ALWAYS_INLINE bool operator== (const vec2_base<T>& v) const { return (x == v.x && y == v.y); }
		ALWAYS_INLINE bool operator!= (const vec2_base<T>& v) const { return (x != v.x || y != v.y); }
		// Bit Operations -----------------
		ALWAYS_INLINE vec2_base<T>  operator>> (const vec2_base<T>& v) const requires std::integral<T> { return vec2_base<T>(x >> v.x, y >> v.y); }
		ALWAYS_INLINE vec2_base<T>& operator>>=(const vec2_base<T>& v)       requires std::integral<T> { x >>= v.x; y >>= v.y; return *this; }
		ALWAYS_INLINE vec2_base<T>  operator<< (const vec2_base<T>& v) const requires std::integral<T> { return vec2_base<T>(x << v.x, y << v.y); }
		ALWAYS_INLINE vec2_base<T>& operator<<=(const vec2_base<T>& v)       requires std::integral<T> { x <<= v.x; y <<= v.y; return *this; }
		ALWAYS_INLINE vec2_base<T>  operator|  (const vec2_base<T>& v) const requires std::integral<T> { return vec2_base<T>(x | v.x, y | v.y); }
		ALWAYS_INLINE vec2_base<T>& operator|= (const vec2_base<T>& v)       requires std::integral<T> { x |= v.x; y |= v.y; return *this; }
		ALWAYS_INLINE vec2_base<T>  operator&  (const vec2_base<T>& v) const requires std::integral<T> { return vec2_base<T>(x & v.x, y & v.y); }
		ALWAYS_INLINE vec2_base<T>& operator&= (const vec2_base<T>& v)       requires std::integral<T> { x &= v.x; y &= v.y; return *this; }
		ALWAYS_INLINE vec2_base<T>  operator^  (const vec2_base<T>& v) const requires std::integral<T> { return vec2_base<T>(x ^ v.x, y ^ v.y); }
		ALWAYS_INLINE vec2_base<T>& operator^= (const vec2_base<T>& v)       requires std::integral<T> { x ^= v.x; y ^= v.y; return *this; }
		// Arithmetic ---------------------
		ALWAYS_INLINE vec2_base<T>  operator+  () { return *this; }
		ALWAYS_INLINE vec2_base<T>  operator+  (const vec2_base<T>& v) const { return vec2_base<T>(x + v.x, y + v.y); }
		ALWAYS_INLINE vec2_base<T>& operator+= (const vec2_base<T>& v) { x += v.x; y += v.y; return *this; }
		ALWAYS_INLINE vec2_base<T>  operator-  () const { return vec2_base<T>(-x, -y); }
		ALWAYS_INLINE vec2_base<T>  operator-  (const vec2_base<T>& v) const { return vec2_base<T>(x - v.x, y - v.y); }
		ALWAYS_INLINE vec2_base<T>& operator-= (const vec2_base<T>& v) { x -= v.x; y -= v.y; return *this; }
		ALWAYS_INLINE vec2_base<T>  operator*  (const vec2_base<T>& v) const { return vec2_base<T>(x * v.x, y * v.y); }
		ALWAYS_INLINE vec2_base<T>& operator*= (const vec2_base<T>& v) { x *= v.x; y *= v.y; return *this; }
		ALWAYS_INLINE vec2_base<T>  operator/  (const vec2_base<T>& v) const { return vec2_base<T>(x / v.x, y / v.y); }
		ALWAYS_INLINE vec2_base<T>& operator/= (const vec2_base<T>& v) { x /= v.x; y /= v.y; return *this; }
		ALWAYS_INLINE vec2_base<T>  operator%  (const vec2_base<T>& v) const requires std::integral<T> { return vec2_base<T>(x % v.x, y % v.y); }
		ALWAYS_INLINE vec2_base<T>& operator%= (const vec2_base<T>& v)       requires std::integral<T> { x %= v.x; y %= v.y; return *this; }
		// Other --------------------------
		ALWAYS_INLINE       T& operator[](int index) { return data[index]; }
		ALWAYS_INLINE const T& operator[](int index) const { return data[index]; }
		ALWAYS_INLINE T Sum() const { return x + y; }
		ALWAYS_INLINE T Dot(const vec2_base<T>& v) const { return ((*this) * v).Sum(); }
		ALWAYS_INLINE float Length() const { return std::sqrtf(static_cast<float>(Dot(*this))); }
		ALWAYS_INLINE vec2_base<T>& Normalize() requires std::floating_point<T> { (*this) /= Length(); return *this; }

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
	ALWAYS_INLINE T Dot(const vec2_base<T>& a, const vec2_base<T>& b) { return a.Dot(b); }
	template <zore::numeric T>
	ALWAYS_INLINE float Length(const vec2_base<T>& v) { return v.Length(); }
	template <zore::numeric T>
	ALWAYS_INLINE float Distance(const vec2_base<T>& a, const vec2_base<T>& b) { return Length(a - b); }
	template <std::floating_point T>
	ALWAYS_INLINE vec2_base<T> Normalize(const vec2_base<T>& v) { return v / v.Length(); }
	template <std::floating_point T>
	ALWAYS_INLINE vec2_base<T> Floor(const vec2_base<T>& v) { return vec2_base<T>(std::floor(v.x), std::floor(v.y)); }
	template <std::floating_point T>
	ALWAYS_INLINE vec2_base<T> Ceil(const vec2_base<T>& v) { return vec2_base<T>(std::ceil(v.x), std::ceil(v.y)); }
	template <std::floating_point T>
	ALWAYS_INLINE vec2_base<T> Round(const vec2_base<T>& v) { return vec2_base<T>(std::round(v.x), std::round(v.y)); }
	template <std::floating_point T>
	ALWAYS_INLINE vec2_base<T> Trunc(const vec2_base<T>& v) { return vec2_base<T>(std::trunc(v.x), std::trunc(v.y)); }
	template <std::floating_point T>
	ALWAYS_INLINE vec2_base<T> Fract(const vec2_base<T>& v) { return v - Trunc(v); }

	//========================================================================
	//  Predefined Named Types
	//========================================================================

	typedef vec2_base<float>     vec2;
	typedef vec2_base<int32_t>  ivec2;
	typedef vec2_base<uint32_t> uvec2;
	typedef vec2_base<double>   dvec2;
}