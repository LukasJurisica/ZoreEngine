#pragma once

#include "zore/math/vector/vec_base.hpp"

namespace zm {

	//========================================================================
	//  4 Dimensional Vector
	//========================================================================

	template <zore::numeric T>
	struct alignas(sizeof(T) * 4) vec_base<T, 4> {
	public:
		// Constructors -------------------
		ALWAYS_INLINE vec_base() : data() {}
		template <zore::numeric U>
		ALWAYS_INLINE vec_base(U s) : data(static_cast<T>(s), static_cast<T>(s), static_cast<T>(s), static_cast<T>(s)) {}
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
		ALWAYS_INLINE vec_base(const vec_base<U, 4>& o) : data(o.data[0], o.data[1], o.data[2], o.data[3]) {}

	public:
		// Comparison ---------------------
		ALWAYS_INLINE bool operator== (const vec_base& o) const { return (x == o.x && y == o.y && z == o.z && w == o.w); }
		ALWAYS_INLINE bool operator!= (const vec_base& o) const { return (x != o.x || y != o.y || z != o.z || w != o.w); }
		// Bit Operations -----------------
		ALWAYS_INLINE vec_base  operator>> (T s) const requires std::integral<T> { return { x >> s, y >> s, z >> s, w >> s }; }
		ALWAYS_INLINE vec_base& operator>>=(T s)       requires std::integral<T> { x >>= s; y >>= s; z >>= s; w >>= s; return *this; }
		ALWAYS_INLINE vec_base  operator<< (T s) const requires std::integral<T> { return { x << s, y << s, z << s, w << s }; }
		ALWAYS_INLINE vec_base& operator<<=(T s)       requires std::integral<T> { x <<= s; y <<= s; z <<= s; w <<= s; return *this; }
		// Arithmetic ---------------------
		ALWAYS_INLINE vec_base  operator+  () const { return *this; }
		ALWAYS_INLINE vec_base  operator+  (const vec_base& o) const { return { x + o.x, y + o.y, z + o.z, w + o.w }; }
		ALWAYS_INLINE vec_base& operator+= (const vec_base& o) { x += o.x; y += o.y; z += o.z; w += o.w; return *this; }
		ALWAYS_INLINE vec_base  operator-  () const { return { -x, -y, -z, -w }; }
		ALWAYS_INLINE vec_base  operator-  (const vec_base& o) const { return { x - o.x, y - o.y, z - o.z, w - o.w }; }
		ALWAYS_INLINE vec_base& operator-= (const vec_base& o) { x -= o.x; y -= o.y; z -= o.z; w -= o.w; return *this; }
		ALWAYS_INLINE vec_base  operator*  (const vec_base& o) const { return { x * o.x, y * o.y, z * o.z, w * o.w }; }
		ALWAYS_INLINE vec_base& operator*= (const vec_base& o) { x *= o.x; y *= o.y; z *= o.z; w *= o.w; return *this; }
		ALWAYS_INLINE vec_base  operator/  (const vec_base& o) const { return { x / o.x, y / o.y, z / o.z, w / o.w }; }
		ALWAYS_INLINE vec_base& operator/= (const vec_base& o) { x /= o.x; y /= o.y; z /= o.z; w /= o.w; return *this; }
		// Other --------------------------
		ALWAYS_INLINE       T& operator[](int index) { return data[index]; }
		ALWAYS_INLINE const T& operator[](int index) const { return data[index]; }
		ALWAYS_INLINE T Sum() const { return x + y + z + w; }
		ALWAYS_INLINE T Dot(const vec_base& o) const { return ((*this) * o).Sum(); }
		ALWAYS_INLINE float Length() const { return std::sqrtf(static_cast<float>(Dot(*this))); }
		ALWAYS_INLINE vec_base& Normalize() requires std::floating_point<T> { (*this) *= (1.f / Length()); return *this; }

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
	ALWAYS_INLINE T Dot(const vec_base<T, 4>& a, const vec_base<T, 4>& b) { return a.Dot(b); }
	template <zore::numeric T>
	ALWAYS_INLINE float Length(const vec_base<T, 4>& v) { return v.Length(); }
	template <zore::numeric T>
	ALWAYS_INLINE float Distance(const vec_base<T, 4>& a, const vec_base<T, 4>& b) { return Length(a - b); }
	template <zore::numeric T>
	ALWAYS_INLINE vec_base<T, 4> Min(const vec_base<T, 4>& a, const vec_base<T, 4>& b) { return { zm::Min(a.x, b.x), zm::Min(a.y, b.y), zm::Min(a.z, b.z), zm::Min(a.w, b.w) }; }
	template <zore::numeric T>
	ALWAYS_INLINE vec_base<T, 4> Max(const vec_base<T, 4>& a, const vec_base<T, 4>& b) { return { zm::Max(a.x, b.x), zm::Max(a.y, b.y), zm::Max(a.z, b.z), zm::Max(a.w, b.w) }; }
	template <std::floating_point T>
	ALWAYS_INLINE vec_base<T, 4> Normalize(const vec_base<T, 4>& v) { return v * (1.f / v.Length()); }
	template <std::floating_point T>
	ALWAYS_INLINE vec_base<T, 4> Floor(const vec_base<T, 4>& v) { return { std::floor(v.x), std::floor(v.y), std::floor(v.z), std::floor(v.w) }; }
	template <std::floating_point T>
	ALWAYS_INLINE vec_base<T, 4> Ceil(const vec_base<T, 4>& v) { return { std::ceil(v.x), std::ceil(v.y), std::ceil(v.z), std::ceil(v.w) }; }
	template <std::floating_point T>
	ALWAYS_INLINE vec_base<T, 4> Round(const vec_base<T, 4>& v) { return { std::round(v.x), std::round(v.y), std::round(v.z), std::round(v.w) }; }
	template <std::floating_point T>
	ALWAYS_INLINE vec_base<T, 4> Trunc(const vec_base<T, 4>& v) { return { std::trunc(v.x), std::trunc(v.y), std::trunc(v.z), std::trunc(v.w) }; }
	template <std::floating_point T>
	ALWAYS_INLINE vec_base<T, 4> Fract(const vec_base<T, 4>& v) { return v - Trunc(v); }

	//========================================================================
	//  Predefined Named Types
	//========================================================================

	typedef vec_base<float, 4>     vec4;
	typedef vec_base<bool, 4>     bvec4;
	typedef vec_base<int32_t, 4>  ivec4;
	typedef vec_base<uint32_t, 4> uvec4;
	typedef vec_base<double, 4>   dvec4;

	template <zore::numeric T>
	std::ostream& operator<<(std::ostream& os, const vec_base<T, 4>& v) {
		return os << "[" << v.x << ", " << v.y << ", " << v.z << ", " << v.w << "]";
	}
}

#include "zore/math/vector/simd_vec4.inl"