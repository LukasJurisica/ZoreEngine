#pragma once
#include "zore/utils/DataTypes.hpp"
#include <cmath>

namespace zm {

	//------------------------------------------------------------------------
	//  Math Constants
	static constexpr float E     = 2.71828182845904523536f;
	static constexpr float PI    = 3.14159265358979323846f;
	static constexpr float PI_2  = 1.57079632679489661923f;
	static constexpr float PI_4  = 0.78539816339744830961f;
	static constexpr float SQRT2 = 1.41421356237309504880f;
	static constexpr float SQRT3 = 1.73205080756887729352f;
	//------------------------------------------------------------------------

	// Returns the next integer smaller than n
	inline int Floor(float n) { return static_cast<int>(n) - (n < 0 ? 1 : 0); }

	// Returns the next integer larger than n
	inline int Ceil(float n) { return static_cast<int>(n) + (n > 0 ? 1 : 0); }

	// Returns the nearest integer to n
	inline int Round(float n) { return static_cast<int>(n + (n < 0 ? -0.5f : 0.5f)); }

	// Returns the fractional component of n
	inline float Fract(float n) { return n - (n > 0 ? static_cast<int>(n) : -static_cast<int>(n)); }

	// Performs a linear interpolation between a and b of n
	template <typename type>
	inline type Lerp(type a, type b, float n) { return (b - a) * n + a; }

	// Performs an inverse linear interpolation of t between a and b
	template <typename type>
	inline float InvLerp(type a, type b, float t) { return (t - a) / (b - a); }

	// Performs a Smoothstep Intrpolation of t between 0 and 1
	inline float InterpHermite(float t) { return t * t * (3 - 2 * t); }
	
	// Performs a Smoothstep Intrpolation of t between a and b
	template <typename type>
	inline float InterpHermite(type a, type b, float t) {
		t = Clamp(InvLerp(a, b, t), 0, 1);
		return t * t * (3.0 - 2.0 * t);
	}

	// Performs a Smoothstep Intrpolation of t between 0 and 1
	inline float InterpQuintic(float t) { return t * t * t * (t * (t * 6 - 15) + 10); }

	// Performs a Ramped exponetiated Intrpolation of t between 0 and 1, with a strength of a
	inline float InterpExp(float t, float a) { return 1 - pow(sqrt(1.f - pow(t, a * 0.5f)), a); }

	inline float CubicLerp(float a, float b, float c, float d, float t) {
		float p = (d - c) - (a - b);
		return t * t * t * p + t * t * ((a - b) - p) + t * (c - a) + b;
	}

	inline float PingPong(float t) {
		t -= static_cast<int>(t * 0.5f) >> 1;
		return t < 1 ? t : 2 - t;
	}

	// Used to transform a value from the range [-1, 1] to [0, 1]
	inline float NormalizeNoise(float n) { return (n + 1.f) * 0.5f; }

	// Used to transform a value from the range [0, 1] to [-1, 1]
	inline float SpreadNoise(float n) { return (n * 2.f) - 1.f; }

	// Packs a floating representation of a radian in a uint8_t
	inline uint8_t PackRadians(float r) {
		static const float mult = 128.f / zm::PI; // 256 / 2pi
		return static_cast<uint8_t>(Floor(r * mult));
	};

	// Unpacks a uint8_t representation of a radian
	inline float UnpackRadians(uint8_t r) {
		static const float mult = zm::PI / 128.f; // 2pi / 256
		return r * mult;
	};

	// Packs a floating representation of a degree in a uint8_t
	inline uint8_t PackDegrees(float r) {
		static const float mult = 256.f / 360.f;
		return static_cast<uint8_t>(Floor(r * mult));
	};

	// Unpacks a uint8_t representation of a degree
	inline float UnpackDegrees(uint8_t r) {
		static const float mult = 360.f / 256;
		return r * mult;
	};
	
	// Returns the smaller value between a and b
	template <typename type>
	inline type Min(type a, type b) { return a < b ? a : b; }

	// Returns the larger value between a and b
	template <typename type>
	inline type Max(type a, type b) { return a > b ? a : b; }

	// Returns the absolute value of t
	template <typename type>
	inline type Abs(type t) { return t > 0 ? t : -t; }

	// Returns the sign of t
	template <typename type>
	inline int Sign(type t) { return t < 0 ? -1 : (t > 0 ? 1 : 0); }

	// Ensures value is clamped within the range [min, max]
	template <typename type>
	inline type Clamp(type value, type min, type max) { return value < min ? min : (value > max ? max : value); }

	// Returns the smaller value between a and b, smoothed around their intersection
	inline float SmoothMin(float a, float b, float t) {
		float h = zm::Clamp(0.5f + 0.5f * (b - a) / t, 0.f, 1.f);
		return zm::Lerp(b, a, h) - t * h * (1.f - h);
	}

	// Returns the larger value between a and b, smoothed around their intersection
	inline float SmoothMax(float a, float b, float t) {
		float h = zm::Clamp(0.5f - 0.5f * (a - b) / t, 0.f, 1.f);
		return zm::Lerp(a, b, h) + t * h * (1.f - h);
	}

	// Wraps value within the range [min, max]
	template <typename type>
	inline type WrapClamp(type value, type min, type max) {
		type delta = max - min;
		while (value < min)
			value += delta;
		while (value >= max)
			value -= delta;
		return value;
	}

	template <typename type>
	inline void SwapIfLarger(type& a, type& b) {
		if (a > b) {
			type t = a;
			a = b;
			b = t;
		}
	}

	// Returns the length of the vector (x, y)
	template <typename type>
	inline float Length(type x, type y) {
		return std::sqrt((x * x) + (y * y));
	}

	// Returns the length of the vector (x, y, z)
	template <typename type>
	inline float Length(type x, type y, type z) {
		return std::sqrt((x * x) + (y * y) + (z * z));
	}

	// Returns the squared length of the vector (x, y)
	template <typename type>
	inline float SqrLength(type x, type y) {
		return (x * x) + (y * y);
	}

	// Returns the squared length of the vector (x, y, z)
	template <typename type>
	inline float SqrLength(type x, type y, type z) {
		return (x * x) + (y * y) + (z * z);
	}

	// Returns the distance between points (x1, y1) and (x2, y2)
	template <typename type>
	inline float Dist(type x1, type y1, type x2, type y2) {
		return Length(x2 - x1, y2 - y1);
	}

	// Returns the distance between points (x1, y1, z1) and (x2, y2, z2)
	template <typename type>
	inline float Dist(type x1, type y1, type z1, type x2, type y2, type z2) {
		return Length(x2 - x1, y2 - y1, z2 - z1);
	}

	// Returns the squared distance between points (x1, y1) and (x2, y2)
	template <typename type>
	inline type SqrDist(type x1, type y1, type x2, type y2) {
		return SqrLength(x2 - x1, y2 - y1);
	}

	// Returns the squared distance between points (x1, y1, z1) and (x2, y2, z2)
	template <typename type>
	inline type SqrDist(type x1, type y1, type z1, type x2, type y2, type z2) {
		return SqrLength(x2 - x1, y2 - y1, z2 - z1);
	}
}