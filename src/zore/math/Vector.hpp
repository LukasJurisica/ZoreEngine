#pragma once

#include "zore/utils/DataTypes.hpp"
#include <cmath>

namespace zm {

	//========================================================================
	//  2 Dimensional Vector
	//========================================================================

	template <typename T>
	struct vec2_base {
		// Constructors -------------------
		vec2_base() : x(0), y(0) {}
		vec2_base(T t) : x(t), y(t) {}
		vec2_base(T x, T y) : x(x), y(y) {}

		template <typename U>
		vec2_base(const vec2_base<U>& o) : x(static_cast<T>(o.x)), y(static_cast<T>(o.y)) {}

		// Operators ----------------------
		inline bool operator==(const vec2_base<T>& o) const {
			return (x == o.x && y == o.y);
		}
		inline bool operator!=(const vec2_base<T>& o) const {
			return (x != o.x || y != o.y);
		}

		// Addition
		inline vec2_base<T> operator+(const vec2_base<T>& o) const {
			return vec2_base<T>(x + o.x, y + o.y);
		}
		inline vec2_base<T> operator+(T o) const {
			return vec2_base<T>(x + o, y + o);
		}
		inline vec2_base<T>& operator+=(const vec2_base<T>& o) {
			x += o.x;
			y += o.y;
			return *this;
		}
		inline vec2_base<T>& operator+=(T o) {
			x += o;
			y += o;
			return *this;
		}

		// Subtraction
		inline vec2_base<T> operator-() {
			return vec2_base<T>(-x, -y);
		}
		inline vec2_base<T> operator-(const vec2_base<T>& o) const {
			return vec2_base<T>(x - o.x, y - o.y);
		}
		inline vec2_base<T> operator-(T o) const {
			return vec2_base<T>(x - o, y - o);
		}
		inline vec2_base<T>& operator-=(const vec2_base<T>& o) {
			x -= o.x;
			y -= o.y;
			return *this;
		}
		inline vec2_base<T>& operator-=(T o) {
			x -= o;
			y -= o;
			return *this;
		}

		// Multiplication
		inline vec2_base<T> operator*(T o) const {
			return vec2_base(x * o, y * o);
		}
		inline vec2_base<T>& operator*=(T o) {
			x *= o;
			y *= o;
			return *this;
		}

		// Division
		inline vec2_base<T> operator/(T o) const {
			return vec2_base(x / o, y / o);
		}
		inline vec2_base<T>& operator/=(T o) {
			x /= o;
			y /= o;
			return *this;
		}

		// Indexed Access
		inline T& operator[](uint32_t index) {
			return data[index];
		}
		inline const T& operator[](uint32_t index) const {
			return data[index];
		}

		// Other --------------------------
		inline T Dot(const vec2_base<T>& o) const {
			return (x * o.x) + (y * o.y);
		}

		inline vec2_base<T> Normalize() const {
			T length = 1 / std::sqrt((x * x) + (y * y));
			return vec2_base<T>(x * length, y * length);
		}

		// Data ---------------------------
		union {
			struct { T x, y; };
			struct { T u, v; };
			T data[2];
		};
	};

	//========================================================================
	//  3 Dimensional Vector
	//========================================================================

	template <typename T>
	struct vec3_base {
		// Constructors -------------------
		vec3_base() : x(0), y(0), z(0) {}
		vec3_base(T t) : x(t), y(t), z(t) {}
		vec3_base(T x, T y, T z) : x(x), y(y), z(z) {}

		template <typename U>
		vec3_base(const vec3_base<U>& o) : x(static_cast<T>(o.x)), y(static_cast<T>(o.y)), z(static_cast<T>(o.z)) {}

		// Operators ----------------------
		inline bool operator==(const vec3_base<T>& o) const {
			return (x == o.x && y == o.y && z == o.z);
		}
		inline bool operator!=(const vec3_base<T>& o) const {
			return (x != o.x || y != o.y || z != o.z);
		}

		// Addition
		inline vec3_base<T> operator+(const vec3_base<T>& o) const {
			return vec3_base<T>(x + o.x, y + o.y, z + o.z);
		}
		inline vec3_base<T> operator+(T o) const {
			return vec3_base<T>(x + o, y + o, z + o);
		}
		inline vec3_base<T>& operator+=(const vec3_base<T>& o) {
			x += o.x;
			y += o.y;
			z += o.z;
			return *this;
		}
		inline vec3_base<T>& operator+=(T o) {
			x += o;
			y += o;
			z += o;
			return *this;
		}

		// Subtraction
		inline vec3_base<T> operator-() const {
			return vec3_base<T>(-x, -y, -z);
		}
		inline vec3_base<T> operator-(const vec3_base<T>& o) const {
			return vec3_base<T>(x - o.x, y - o.y, z - o.z);
		}
		inline vec3_base<T> operator-(T o) const {
			return vec3_base<T>(x - o, y - o, z - o);
		}
		inline vec3_base<T>& operator-=(const vec3_base<T>& o) {
			x -= o.x;
			y -= o.y;
			z -= o.z;
			return *this;
		}
		inline vec3_base<T>& operator-=(T o) {
			x -= o;
			y -= o;
			z -= o;
			return *this;
		}

		// Multiplication
		inline vec3_base<T> operator*(T o) const {
			return vec3_base(x * o, y * o, z * o);
		}
		inline vec3_base<T>& operator*=(T o) {
			x *= o;
			y *= o;
			z *= o;
			return *this;
		}

		// Division
		inline vec3_base<T> operator/(T o) const {
			return vec3_base(x / o, y / o, z / o);
		}
		inline vec3_base<T>& operator/=(T o) {
			x /= o;
			y /= o;
			z /= o;
			return *this;
		}

		// Indexed Access
		inline T& operator[](uint32_t index) {
			return data[index];
		}
		inline const T& operator[](uint32_t index) const {
			return data[index];
		}

		// Other --------------------------
		inline T Dot(const vec3_base<T>& o) const {
			return (x * o.x) + (y * o.y) + (z * o.z);
		}

		inline vec3_base<T> Cross(const vec3_base<T>& o) const {
			return vec3_base<T>((y * o.z) - (z * o.y), (z * o.x) - (x * o.z), (x * o.y) - (y * o.x));
		}

		inline vec3_base<T> Normalize() const {
			T length = 1 / std::sqrt((x * x) + (y * y) + (z * z));
			return vec3_base<T>(x * length, y * length, z * length);
		}

		// Data ---------------------------
		union {
			struct { T x, y, z; };
			struct { T r, g, b; };
			T data[3];
		};
	};

	//========================================================================
	//  4 Dimensional Vector
	//========================================================================

	template <typename T>
	struct vec4_base {
		// Constructors -------------------
		vec4_base() : x(0), y(0), z(0), w(0) {}
		vec4_base(T t) : x(t), y(t), z(t), w(t) {}
		vec4_base(T x, T y, T z, T w) : x(x), y(y), z(z), w(w) {}

		template <typename U>
		vec4_base(const vec4_base<U>& o) : x(static_cast<T>(o.x)), y(static_cast<T>(o.y)), z(static_cast<T>(o.z)), w(static_cast<T>(o.w)) {}

		// Operators ----------------------
		inline bool operator==(const vec4_base<T>& o) const {
			return (x == o.x && y == o.y && z == o.z && w == o.w);
		}
		inline bool operator!=(const vec4_base<T>& o) const {
			return (x != o.x || y != o.y || z != o.z || w != o.w);
		}

		// Addition
		inline vec4_base<T> operator+(const vec4_base<T>& o) const {
			return vec4_base<T>(x + o.x, y + o.y, z + o.z, w + o.w);
		}
		inline vec4_base<T> operator+(T o) const {
			return vec4_base<T>(x + o, y + o, z + o, w + o);
		}
		inline vec4_base<T>& operator+=(const vec4_base<T>& o) {
			x += o.x;
			y += o.y;
			z += o.z;
			w += o.w;
			return *this;
		}
		inline vec4_base<T>& operator+=(T o) {
			x += o;
			y += o;
			z += o;
			w += o;
			return *this;
		}

		// Subtraction
		inline vec4_base<T> operator-() {
			return vec4_base<T>(-x, -y, -z, -w);
		}
		inline vec4_base<T> operator-(const vec4_base<T>& o) const {
			return vec4_base<T>(x - o.x, y - o.y, z - o.z, w - o.w);
		}
		inline vec4_base<T> operator-(T o) const {
			return vec4_base<T>(x - o, y - o, z - o, w - o);
		}
		inline vec4_base<T>& operator-=(const vec4_base<T>& o) {
			x -= o.x;
			y -= o.y;
			z -= o.z;
			w -= o.w;
			return *this;
		}
		inline vec4_base<T>& operator-=(T o) {
			x -= o;
			y -= o;
			z -= o;
			w -= o;
			return *this;
		}

		// Multiplication
		inline vec4_base<T> operator*(T o) const {
			return vec4_base(x * o, y * o, z * o, w * o);
		}
		inline vec4_base<T>& operator*=(T o) {
			x *= o;
			y *= o;
			z *= o;
			w *= o;
			return *this;
		}

		// Division
		inline vec4_base<T> operator/(T o) const {
			return vec4_base(x / o, y / o, z / o, w / o);
		}
		inline vec4_base<T>& operator/=(T o) {
			x /= o;
			y /= o;
			z /= o;
			w /= o;
			return *this;
		}

		// Indexed Access
		inline T& operator[](uint32_t index) {
			return data[index];
		}
		inline const T& operator[](uint32_t index) const {
			return data[index];
		}

		// Other --------------------------
		inline T Dot(const vec4_base<T>& o) const {
			return (x * o.x) + (y * o.y) + (z * o.z) + (w * o.w);
		}
		
		inline vec4_base<T> Normalize() const {
			T length = 1 / std::sqrt((x * x) + (y * y) + (z * z) + (w * w));
			return vec4_base<T>(x * length, y * length, z * length, w * length);
		}

		// Data ---------------------------
		union {
			struct { T x, y, z, w; };
			struct { T r, g, b, a; };
			T data[4];
		};
	};

	typedef vec2_base<float> vec2;
	typedef vec3_base<float> vec3;
	typedef vec4_base<float> vec4;

	typedef vec2_base<int> ivec2;
	typedef vec3_base<int> ivec3;
	typedef vec4_base<int> ivec4;

	typedef vec2_base<uint32_t> uvec2;
	typedef vec3_base<uint32_t> uvec3;
	typedef vec4_base<uint32_t> uvec4;

	typedef vec2_base<double> dvec2;
	typedef vec3_base<double> dvec3;
	typedef vec4_base<double> dvec4;
}