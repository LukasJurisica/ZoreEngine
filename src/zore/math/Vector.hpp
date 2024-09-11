#pragma once

#include <zore/math/MathUtils.hpp>

namespace zm {

	//========================================================================
	//  2 Dimensional Vector
	//========================================================================

	template <typename T>
	struct vec2_base {
		// Constructors -------------------
		explicit vec2_base() : x(0), y(0) {}
		explicit vec2_base(T t) : x(t), y(t) {}
		explicit vec2_base(T x, T y) : x(x), y(y) {}
		template <typename U>
		vec2_base(const vec2_base<U>& o) : x(static_cast<T>(o.x)), y(static_cast<T>(o.y)) {}

		// Operators ----------------------
		inline bool operator==(const vec2_base<T>& o) const { return (x == o.x && y == o.y); }
		inline bool operator!=(const vec2_base<T>& o) const { return (x != o.x || y != o.y); }

		// Addition
		inline vec2_base<T>& operator+=(const vec2_base<T>& o) { x += o.x; y += o.y; return *this; }
		inline vec2_base<T>& operator+=(T s) { x += s; y += s; return *this; }

		// Subtraction
		inline vec2_base<T>& operator-=(const vec2_base<T>& o) { x -= o.x; y -= o.y; return *this; }
		inline vec2_base<T>& operator-=(T s) { x -= s; y -= s; return *this; }

		// Multiplication
		inline vec2_base<T>& operator*=(const vec2_base<T>& o) { x *= o.x; y *= o.y; return *this; }
		inline vec2_base<T>& operator*=(T s) { x *= s; y *= s; return *this; }

		// Division
		inline vec2_base<T>& operator/=(const vec2_base<T>& o) { x /= o.x; y /= o.y; return *this; }
		inline vec2_base<T>& operator/=(T s) { x /= s; y /= s; return *this; }

		// Indexed Access
		inline T& operator[](uint32_t index) { return data[index]; }
		inline const T& operator[](uint32_t index) const { return data[index]; }

		// Other --------------------------
		inline T Length() const { return std::sqrt((x * x) + (y * y)); }
		inline vec2_base<T> Normalize() const { return (*this) * (1 / Length()); }
		inline T Dot(const vec2_base<T>& o) const { return (x * o.x) + (y * o.y); }
		static inline T Dot(const vec2_base<T>& a, const vec2_base<T>& b) { a.Dot(b); }

		// Data ---------------------------
		union {
			struct { T x, y; };
			struct { T u, v; };
			T data[2];
		};
	};

	// Unary Operators
	template <typename T>
	inline vec2_base<T> operator+(const vec2_base<T>& v) { return v; }
	template <typename T>
	inline vec2_base<T> operator-(const vec2_base<T>& v) { return vec2_base<T>(-v.x, -v.y); }

	// Addition
	template <typename T>
	inline vec2_base<T> operator+(const vec2_base<T>& l, const vec2_base<T>& r) { return vec2_base<T>(l.x + r.x, l.y + r.y); }
	template <typename T>
	inline vec2_base<T> operator+(const vec2_base<T>& v, T s) { return vec2_base<T>(v.x + s, v.y + s); }
	template <typename T>
	inline vec2_base<T> operator+(T s, const vec2_base<T>& v) { return vec2_base<T>(v.x + s, v.y + s); }

	// Subtraction
	template <typename T>
	inline vec2_base<T> operator-(const vec2_base<T>& l, const vec2_base<T>& r) { return vec2_base<T>(l.x - r.x, l.y - r.y); }
	template <typename T>
	inline vec2_base<T> operator-(const vec2_base<T>& v, T s) { return vec2_base<T>(v.x - s, v.y - s); }
	template <typename T>
	inline vec2_base<T> operator-(T s, const vec2_base<T>& v) { return vec2_base<T>(v.x - s, v.y - s); }

	// Multiplication
	template <typename T>
	inline vec2_base<T> operator*(const vec2_base<T>& l, const vec2_base<T>& r) { return vec2_base<T>(l.x * r.x, l.y * r.y); }
	template <typename T>
	inline vec2_base<T> operator*(const vec2_base<T>& v, T s) { return vec2_base<T>(v.x * s, v.y * s); }
	template <typename T>
	inline vec2_base<T> operator*(T s, const vec2_base<T>& v) { return vec2_base<T>(v.x * s, v.y * s); }

	// Division
	template <typename T>
	inline vec2_base<T> operator/(const vec2_base<T>& l, const vec2_base<T>& r) { return vec2_base<T>(l.x / r.x, l.y / r.y); }
	template <typename T>
	inline vec2_base<T> operator/(const vec2_base<T>& v, T s) { return vec2_base<T>(v.x / s, v.y / s); }
	template <typename T>
	inline vec2_base<T> operator/(T s, const vec2_base<T>& v) { return vec2_base<T>(v.x / s, v.y / s); }

	//========================================================================
	//  3 Dimensional Vector
	//========================================================================

	template <typename T>
	struct vec3_base {
		// Constructors -------------------
		explicit vec3_base() : x(0), y(0), z(0) {}
		explicit vec3_base(T t) : x(t), y(t), z(t) {}
		explicit vec3_base(T x, T y, T z) : x(x), y(y), z(z) {}
		template <typename U>
		vec3_base(const vec3_base<U>& o) : x(static_cast<T>(o.x)), y(static_cast<T>(o.y)), z(static_cast<T>(o.z)) {}

		// Operators ----------------------
		inline bool operator==(const vec3_base<T>& o) const { return (x == o.x && y == o.y && z == o.z); }
		inline bool operator!=(const vec3_base<T>& o) const { return (x != o.x || y != o.y || z != o.z); }

		// Addition
		inline vec3_base<T>& operator+=(const vec3_base<T>& o) { x += o.x; y += o.y; z += o.z; return *this; }
		inline vec3_base<T>& operator+=(T o) { x += o; y += o; z += o; return *this; }

		// Subtraction
		inline vec3_base<T>& operator-=(const vec3_base<T>& o) { x -= o.x; y -= o.y; z -= o.z; return *this; }
		inline vec3_base<T>& operator-=(T o) { x -= o; y -= o; z -= o; return *this; }

		// Multiplication
		inline vec3_base<T>& operator*=(const vec3_base<T>& o) { x *= o.x; y *= o.y; z *= o.z; return *this; }
		inline vec3_base<T>& operator*=(T o) { x *= o; y *= o; z *= o; return *this; }

		// Division
		inline vec3_base<T>& operator/=(const vec3_base<T>& o) { x /= o.x; y /= o.y; z /= o.z; return *this; }
		inline vec3_base<T>& operator/=(T o) { x /= o; y /= o; z /= o; return *this; }

		// Indexed Access
		inline T& operator[](uint32_t index) { return data[index]; }
		inline const T& operator[](uint32_t index) const { return data[index]; }

		// Other --------------------------
		inline T Length() const { return std::sqrt((x * x) + (y * y) + (z * z)); }
		inline vec3_base<T> Normalize() const { return (*this) * (1 / Length()); }
		inline T Dot(const vec3_base<T>& o) const { return (x * o.x) + (y * o.y) + (z * o.z); }
		static inline T Dot(const vec2_base<T>& a, const vec2_base<T>& b) { return a.Dot(b); }
		inline vec3_base<T> Cross(const vec3_base<T>& o) const { return vec3_base<T>((y * o.z) - (z * o.y), (z * o.x) - (x * o.z), (x * o.y) - (y * o.x)); }
		static inline vec3_base<T> Cross(const vec3_base<T>& a, const vec3_base<T>& b) { return a.Cross(b); }

		// Data ---------------------------
		union {
			struct { T x, y, z; };
			struct { T r, g, b; };
			T data[3];
		};
	};

	// Unary Operators
	template <typename T>
	inline vec3_base<T> operator+(const vec3_base<T>& v) { return v; }
	template <typename T>
	inline vec3_base<T> operator-(const vec3_base<T>& v) { return vec3_base<T>(-v.x, -v.y, -v.z); }

	// Addition
	template <typename T>
	inline vec3_base<T> operator+(const vec3_base<T>& l, const vec3_base<T>& r) { return vec3_base<T>(l.x + r.x, l.y + r.y, l.z + r.z); }
	template <typename T>
	inline vec3_base<T> operator+(const vec3_base<T>& v, T s) { return vec3_base<T>(v.x + s, v.y + s, v.z + s); }
	template <typename T>
	inline vec3_base<T> operator+(T s, const vec3_base<T>& v) { return vec3_base<T>(v.x + s, v.y + s, v.z + s); }

	// Subtraction
	template <typename T>
	inline vec3_base<T> operator-(const vec3_base<T>& l, const vec3_base<T>& r) { return vec3_base<T>(l.x - r.x, l.y - r.y, l.z - r.z); }
	template <typename T>
	inline vec3_base<T> operator-(const vec3_base<T>& v, T s) { return vec3_base<T>(v.x - s, v.y - s, v.z - s); }
	template <typename T>
	inline vec3_base<T> operator-(T s, const vec3_base<T>& v) { return vec3_base<T>(v.x - s, v.y - s, v.z - s); }

	// Multiplication
	template <typename T>
	inline vec3_base<T> operator*(const vec3_base<T>& l, const vec3_base<T>& r) { return vec3_base<T>(l.x * r.x, l.y * r.y, l.z * r.z); }
	template <typename T>
	inline vec3_base<T> operator*(const vec3_base<T>& v, T s) { return vec3_base<T>(v.x * s, v.y * s, v.z * s); }
	template <typename T>
	inline vec3_base<T> operator*(T s, const vec3_base<T>& v) { return vec3_base<T>(v.x * s, v.y * s, v.z * s); }

	// Division
	template <typename T>
	inline vec3_base<T> operator/(const vec3_base<T>& l, const vec3_base<T>& r) { return vec3_base<T>(l.x / r.x, l.y / r.y, l.z / r.z); }
	template <typename T>
	inline vec3_base<T> operator/(const vec3_base<T>& v, T s) { return vec3_base<T>(v.x / s, v.y / s, v.z / s); }
	template <typename T>
	inline vec3_base<T> operator/(T s, const vec3_base<T>& v) { return vec3_base<T>(v.x / s, v.y / s, v.z / s); }

	//========================================================================
	//  4 Dimensional Vector
	//========================================================================

	template <typename T>
	struct vec4_base {
		// Constructors -------------------
		explicit vec4_base() : x(0), y(0), z(0), w(0) {}
		explicit vec4_base(T t) : x(t), y(t), z(t), w(t) {}
		explicit vec4_base(T x, T y, T z, T w) : x(x), y(y), z(z), w(w) {}
		template <typename U>
		vec4_base(const vec4_base<U>& o) : x(static_cast<T>(o.x)), y(static_cast<T>(o.y)), z(static_cast<T>(o.z)), w(static_cast<T>(o.w)) {}

		// Operators ----------------------
		inline bool operator==(const vec4_base<T>& o) const { return (x == o.x && y == o.y && z == o.z && w == o.w); }
		inline bool operator!=(const vec4_base<T>& o) const { return (x != o.x || y != o.y || z != o.z || w != o.w); }

		// Addition
		inline vec4_base<T>& operator+=(const vec4_base<T>& o) { x += o.x; y += o.y; z += o.z; w += o.w; return *this; }
		inline vec4_base<T>& operator+=(T o) { x += o; y += o; z += o; w += o; return *this; }

		// Subtraction
		inline vec4_base<T>& operator-=(const vec4_base<T>& o) { x -= o.x; y -= o.y; z -= o.z; w -= o.w; return *this; }
		inline vec4_base<T>& operator-=(T o) { x -= o; y -= o; z -= o; w -= o.w; return *this; }

		// Multiplication
		inline vec4_base<T>& operator*=(const vec4_base<T>& o) { x *= o.x; y *= o.y; z *= o.z; w *= o.w; return *this; }
		inline vec4_base<T>& operator*=(T o) { x *= o; y *= o; z *= o; w *= o.w; return *this; }

		// Division
		inline vec4_base<T>& operator/=(const vec4_base<T>& o) { x /= o.x; y /= o.y; z /= o.z; w /= o.w; return *this; }
		inline vec4_base<T>& operator/=(T o) { x /= o; y /= o; z /= o; w /= o.w; return *this; }

		// Indexed Access
		inline T& operator[](uint32_t index) { return data[index]; }
		inline const T& operator[](uint32_t index) const { return data[index]; }

		// Other --------------------------
		inline T Length() const { return std::sqrt((x * x) + (y * y) + (z * z) + (w * w)); }
		inline vec3_base<T> Normalize() const { return (*this) * (1 / Length()); }
		inline T Dot(const vec3_base<T>& o) const { return (x * o.x) + (y * o.y) + (z * o.z) + (w * o.w); }
		static inline T Dot(const vec2_base<T>& a, const vec2_base<T>& b) { return a.Dot(b); }

		// Data ---------------------------
		union {
			struct { T x, y, z, w; };
			struct { T r, g, b, a; };
			T data[4];
		};
	};

	// Unary Operators
	template <typename T>
	inline vec4_base<T> operator+(const vec4_base<T>& v) { return v; }
	template <typename T>
	inline vec4_base<T> operator-(const vec4_base<T>& v) { return vec4_base<T>(-v.x, -v.y, -v.z, -v.w); }

	// Addition
	template <typename T>
	inline vec4_base<T> operator+(const vec4_base<T>& l, const vec4_base<T>& r) { return vec4_base<T>(l.x + r.x, l.y + r.y, l.z + r.z, l.w + r.w); }
	template <typename T>
	inline vec4_base<T> operator+(const vec4_base<T>& v, T s) { return vec4_base<T>(v.x + s, v.y + s, v.z + s, v.w + s); }
	template <typename T>
	inline vec4_base<T> operator+(T s, const vec4_base<T>& v) { return vec4_base<T>(v.x + s, v.y + s, v.z + s, v.w + s); }

	// Subtraction
	template <typename T>
	inline vec4_base<T> operator-(const vec4_base<T>& l, const vec4_base<T>& r) { return vec4_base<T>(l.x - r.x, l.y - r.y, l.z - r.z, l.w - r.w); }
	template <typename T>
	inline vec4_base<T> operator-(const vec4_base<T>& v, T s) { return vec4_base<T>(v.x - s, v.y - s, v.z - s, v.w - s); }
	template <typename T>
	inline vec4_base<T> operator-(T s, const vec4_base<T>& v) { return vec4_base<T>(v.x - s, v.y - s, v.z - s, v.w - s); }

	// Multiplication
	template <typename T>
	inline vec4_base<T> operator*(const vec4_base<T>& l, const vec4_base<T>& r) { return vec4_base<T>(l.x * r.x, l.y * r.y, l.z * r.z, l.w * r.w); }
	template <typename T>
	inline vec4_base<T> operator*(const vec4_base<T>& v, T s) { return vec4_base<T>(v.x * s, v.y * s, v.z * s, v.w * s); }
	template <typename T>
	inline vec4_base<T> operator*(T s, const vec4_base<T>& v) { return vec4_base<T>(v.x * s, v.y * s, v.z * s, v.w * s); }

	// Division
	template <typename T>
	inline vec4_base<T> operator/(const vec4_base<T>& l, const vec4_base<T>& r) { return vec4_base<T>(l.x / r.x, l.y / r.y, l.z / r.z, l.w / r.w); }
	template <typename T>
	inline vec4_base<T> operator/(const vec4_base<T>& v, T s) { return vec4_base<T>(v.x / s, v.y / s, v.z / s, v.w / s); }
	template <typename T>
	inline vec4_base<T> operator/(T s, const vec4_base<T>& v) { return vec4_base<T>(v.x / s, v.y / s, v.z / s, v.w / s); }

	//========================================================================
	//  Predefined Named Types
	//========================================================================

	typedef vec2_base<float> vec2;
	typedef vec3_base<float> vec3;
	typedef vec4_base<float> vec4;

	typedef vec2_base<int32_t> ivec2;
	typedef vec3_base<int32_t> ivec3;
	typedef vec4_base<int32_t> ivec4;

	typedef vec2_base<uint32_t> uvec2;
	typedef vec3_base<uint32_t> uvec3;
	typedef vec4_base<uint32_t> uvec4;

	typedef vec2_base<double> dvec2;
	typedef vec3_base<double> dvec3;
	typedef vec4_base<double> dvec4;
}