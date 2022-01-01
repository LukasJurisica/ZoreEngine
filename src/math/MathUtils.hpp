#pragma once
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
	inline float Lerp(float a, float b, float n) { return a + n * (b - a); }

	// Performs an inverse linear interpolation of value between a and b
	inline float InvLerp(float a, float b, float value) { return (value - a) / (b - a); }

	inline float InterpHermite(float t) { return t * t * (3 - 2 * t); }

	inline float InterpQuintic(float t) { return t * t * t * (t * (t * 6 - 15) + 10); }

	inline float CubicLerp(float a, float b, float c, float d, float t) {
		float p = (d - c) - (a - b);
		return t * t * t * p + t * t * ((a - b) - p) + t * (c - a) + b;
	}

	inline float PingPong(float t) {
		t -= (int)(t * 0.5f) * 2;
		return t < 1 ? t : 2 - t;
	}

	// Used to transform a value from the range [-1, 1] to [0, 1]
	inline float NormalizeNoise(float n) { return (n + 1.f) * 0.5f; }

	// Used to transform a value from the range [0, 1] to [-1, 1]
	inline float SpreadNoise(float n) { return (n * 2.f) - 1.f; }
	
	// Returns the smaller value between a and b
	template <typename type>
	inline type Min(type a, type b) { return a < b ? a : b; }

	// Returns the larger value between a and b
	template <typename type>
	inline type Max(type a, type b) { return a > b ? a : b; }

	// Returns the absolute value of value
	template <typename type>
	inline type Abs(type value) { return value > 0 ? value : -value; }

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
	inline float Dist(type x1, type y1, type x2, type y2) {
		x2 -= x1; y2 -= y1;
		return std::sqrt((x2 * x2) + (y2 * y2));
	}

	template <typename type>
	inline float Dist(type sx, type sy, type sz, type dx, type dy, type dz) {
		dx -= sx; dy -= sy; dz -= sz;
		return std::sqrt((dx * dx) + (dy * dy) + (dz * dz));
	}

	template <typename type>
	inline type SqrDist(type x1, type y1, type x2, type y2) {
		x2 -= x1; y2 -= y1;
		return (x2 * x2) + (y2 * y2);
	}

	template <typename type>
	inline type SqrDist(type x1, type y1, type z1, type x2, type y2, type z2) {
		x2 -= x1; y2 -= y1; z2 -= z1;
		return (x2 * x2) + (y2 * y2) + (z2 * z2);
	}
}