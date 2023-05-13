//#pragma once
//#include "zore/debug/Debug.hpp"
//#include <string>
//
//namespace zm {
//
//	template <typename T>
//	struct vec2_base {
//
//		// Constructors -------------------
//		vec2_base() : x(0), y(0) {};
//		vec2_base(T t) : x(t), y(t) {};
//		vec2_base(T x, T y) : x(x), y(y) {};
//		template <typename U>
//		vec2_base(const vec2_base<U>& o) : x(static_cast<T>(o.x)), y(static_cast<T>(o.y)) {};
//		// --------------------------------
//		
//
//		// Data ---------------------------
//		union {
//			struct { T x, y; };
//			T data[2];
//		};
//		// --------------------------------
//
//
//		// Operators ----------------------
//		// Equality
//		bool operator==(const vec2_base<T>& o) const {
//			return (x == o.x && y == o.y);
//		}
//
//		// Copy
//		vec2_base<T>& operator=(const vec2_base<T>& o) {
//			x = o.x;
//			y = o.y;
//			return *this;
//		}
//
//		// Addition
//		vec2_base<T> operator+(const vec2_base<T>& o) {
//			return vec2_base<T>(x + o.x, y + o.y);
//		}
//		vec2_base<T> operator+(T o) {
//			return vec2_base<T>(x + o, y + o);
//		}
//		vec2_base<T>& operator+=(const vec2_base<T>& o) {
//			x += o.x;
//			y += o.y;
//			return *this;
//		}
//		vec2_base<T>& operator+=(T o) {
//			x += o;
//			y += o;
//			return *this;
//		}
//
//		// Subtraction
//		vec2_base<T> operator-(const vec2_base<T>& o) {
//			return vec2_base<T>(x - o.x, y - o.y);
//		}
//		vec2_base<T> operator-(T o) {
//			return vec2_base<T>(x - o, y - o);
//		}
//		vec2_base<T>& operator-=(const vec2_base<T>& o) {
//			x -= o.x;
//			y -= o.y;
//			return *this;
//		}
//		vec2_base<T>& operator-=(T o) {
//			x -= o;
//			y -= o;
//			return *this;
//		}
//
//		// Multiplication
//		vec2_base<T> operator*(const vec2_base<T>& o) {
//			return vec2_base(x * o.x, y * o.y);
//		}
//		vec2_base<T> operator*(T o) {
//			return vec2_base(x * o, y * o);
//		}
//		vec2_base<T>& operator*=(const vec2_base<T>& o) {
//			x *= o.x;
//			y *= o.y;
//			return *this;
//		}
//		vec2_base<T>& operator*=(T o) {
//			x *= o;
//			y *= o;
//			return *this;
//		}
//
//		// Division
//		vec2_base<T> operator/(const vec2_base& o) {
//			return vec2_base(x / o.x, y / o.y);
//		}
//		vec2_base<T> operator/(T o) {
//			return vec2_base(x / o, y / o);
//		}
//		vec2_base<T>& operator/=(const vec2_base& o) {
//			x /= o.x;
//			y /= o.y;
//			return *this;
//		}
//		vec2_base<T>& operator/=(T o) {
//			x /= o;
//			y /= o;
//			return *this;
//		}
//
//		// Indexed Access
//		T& operator[](std::size_t index) {
//			assert(index >= 0 && index < 2, "Index out of range");
//			return data[index];
//		}
//		const T& operator[](std::size_t index) const {
//			assert(index >= 0 && index < 2, "Index out of range");
//			return data[index];
//		}
//		// --------------------------------
//
//		// Other --------------------------
//		T Dot(const vec2_base<T>& o) {
//			return (x * o.x) + (y * o.y);
//		}
//
//		//vec2_base<T> Cross(const vec2_base<T>& o) {
//		//	return (x * o.x) + (y * o.y);
//		//}
//
//		std::string to_string() const {
//			return std::to_string(x) + " " + std::to_string(y);
//		}
//		// --------------------------------
//	};
//
//
//
//	/*
//	template <typename T>
//	struct vec3_base {
//		union {
//			struct { T x, y, z; };
//			struct { T r, g, b; };
//			T data[3];
//		};
//	};
//
//	template <typename T>
//	struct vec4_base {
//		union {
//			struct { T x, y, z, w; };
//			struct { T r, g, b, a; };
//			T data[4];
//		};
//	};
//	*/
//	
//	typedef vec2_base<float> vec2;
//	//typedef vec_base<float, 3> vec3;
//	//typedef vec_base<float, 4> vec4;
//	
//	typedef vec2_base<int> ivec2;
//	//typedef vec<int, 3> ivec3;
//	//typedef vec<int, 4> ivec4;
//
//	typedef vec2_base<unsigned int> uvec2;
//	//typedef vec<unsigned int, 3> uvec3;
//	//typedef vec<unsigned int, 4> uvec4;
//
//	typedef vec2_base<double> dvec2;
//	//typedef vec<double, 3> dvec3;
//	//typedef vec<double, 4> dvec4;
//}