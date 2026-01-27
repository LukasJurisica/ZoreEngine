#pragma once

#include "zore/utils/sized_integer.hpp"
#include "zore/utils/concepts.hpp"
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

	// Ensures value is clamped within the range [min, max]
	template <zore::numeric T>
	inline T Clamp(T value, T min, T max) { return value < min ? min : (value > max ? max : value); }

	// Wraps value within the range [min, max]
	template <std::floating_point T>
	inline T WrapClamp(T value, T min, T max) {
		T delta = max - min;
		value = std::fmod(value, delta);
		if (value < 0)
			value += delta;
		return value + min;
	}

	template <std::integral T>
	inline T WrapClamp(T value, T min, T max) {
		T delta = max - min;
		value = value % delta;
		if (value < 0)
			value += delta;
		return value + min;
	}

	// Returns the fractional component of n
	template <std::floating_point T>
	inline T Fract(T n) { return n - std::trunc(n); }

	// Performs a linear interpolation between a and b of n
	template <zore::arithmetic T, zore::arithmetic U>
	inline T Lerp(T a, T b, U t) { return (b - a) * t + a; }

	// Performs an inverse linear interpolation of t between a and b
	template <zore::arithmetic T>
	inline T InvLerp(T a, T b, T t) { return (t - a) / (b - a); }

	// Performs a Smoothstep Interpolation of t
	template <zore::arithmetic T>
	inline T Smoothstep(T t) { return t * t * (T(3) - T(2) * t); }
	
	// Performs a Quintic Smoothstep Interpolation of t
	template <zore::arithmetic T>
	inline T QuinticSmoothstep(T t) { return t * t * t * (t * (t * T(6) - T(15)) + T(10)); }

	// Performs a Ramped exponetiated Interpolation of t between 0 and 1, with a strength of a
	template <zore::arithmetic T>
	inline T ExpSmoothstep(T t, T a) { return (T)1 - pow(sqrt((T)1 - pow(t, a * (T)0.5)), a); }

	// Used to transform a value from the range [-1, 1] to [0, 1]
	template <zore::arithmetic T>
	inline T Normalize(T n) { return (n + (T)1) * (T)0.5; }

	// Used to transform a value from the range [0, 1] to [-1, 1]
	template <zore::arithmetic T>
	inline T Spread(float n) { return (n * (T)2) - (T)1; }

	template <zore::numeric T>
	inline T Radians(T d) { return static_cast<float>(d) * PI / 180.f; }

	template <zore::numeric T>
	inline T Degrees(T r) { return static_cast<float>(r) * 180.f / PI; }

	// Returns the smaller value between a and b
	template <zore::numeric T>
	inline T Min(T a, T b) { return a < b ? a : b; }

	// Returns the larger value between a and b
	template <zore::numeric T>
	inline T Max(T a, T b) { return a > b ? a : b; }

	// Returns the absolute value of t
	template <zore::numeric T>
	inline T Abs(T t) { return t > 0 ? t : -t; }

	// Returns the sign of t
	template <zore::numeric T>
	inline int Sign(T t) { return t < 0 ? -1 : (t > 0 ? 1 : 0); }

	// Returns the smaller value between a and b, smoothed around their intersection
	template <std::floating_point T, std::floating_point U>
	inline float SmoothMin(T a, T b, U t) {
		T h = zm::Clamp(static_cast<T>(0.5) + static_cast<T>(0.5) * (b - a) / t, static_cast<T>(0), static_cast<T>(1));
		return zm::Lerp(b, a, h) - t * h * (static_cast<T>(1) - h);
	}

	// Returns the larger value between a and b, smoothed around their intersection
	template <std::floating_point T, std::floating_point U>
	inline float SmoothMax(T a, T b, U t) {
		T h = zm::Clamp(static_cast<T>(0.5) - static_cast<T>(0.5) * (a - b) / t, static_cast<T>(0), static_cast<T>(1));
		return zm::Lerp(a, b, h) + t * h * (static_cast<T>(1) - h);
	}
}