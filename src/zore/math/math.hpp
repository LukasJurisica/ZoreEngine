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

	// Returns the fractional component of n
	template <std::floating_point T>
	inline T Fract(T n) { return n - std::trunc(n); }

	// Performs a linear interpolation between a and b of n
	template <typename T, std::floating_point U>
	inline T Lerp(T a, T b, U t) { return (b - a) * t + a; }

	// Performs an inverse linear interpolation of t between a and b
	template <typename T>
	inline T InvLerp(T a, T b, T t) { return (t - a) / (b - a); }

	// Performs a Smoothstep Interpolation of t
	template <std::floating_point T>
	inline T Smoothstep(T t) { return Clamp(t * t * (3 - 2 * t), static_cast<T>(0), static_cast<T>(1)); }
	
	// Performs a Quintic Smoothstep Interpolation of t
	template <std::floating_point T>
	inline float QuinticSmoothstep(T t) { return Clamp(t * t * t * (t * (t * 6 - 15) + 10), static_cast<T>(0), static_cast<T>(1)); }

	// Performs a Ramped exponetiated Interpolation of t between 0 and 1, with a strength of a
	template <std::floating_point T>
	inline float ExpSmoothstep(T t, T a) { return static_cast<T>(1) - pow(sqrt(static_cast<T>(1) - pow(t, a * static_cast<T>(0.5))), a); }

	// Used to transform a value from the range [-1, 1] to [0, 1]
	template <std::floating_point T>
	inline T Normalize(T n) { return (n + static_cast<T>(1)) * static_cast<T>(0.5); }

	// Used to transform a value from the range [0, 1] to [-1, 1]
	template <std::floating_point T>
	inline float Spread(float n) { return (n * static_cast<T>(2)) - static_cast<T>(1); }

	// Packs a floating representation of a radian in a uint8_t
	inline uint8_t PackRadians(float r) {
		static const float mult = 128.f / zm::PI; // 256 / 2pi
		return static_cast<uint8_t>(std::floor(r * mult));
	};

	// Unpacks a uint8_t representation of a radian
	inline float UnpackRadians(uint8_t r) {
		static const float mult = zm::PI / 128.f; // 2pi / 256
		return r * mult;
	};

	// Packs a floating representation of a degree in a uint8_t
	inline uint8_t PackDegrees(float r) {
		static const float mult = 256.f / 360.f;
		return static_cast<uint8_t>(std::floor(r * mult));
	};

	// Unpacks a uint8_t representation of a degree
	inline float UnpackDegrees(uint8_t r) {
		static const float mult = 360.f / 256;
		return r * mult;
	};
	
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

	// Ensures value is clamped within the range [min, max]
	template <zore::numeric T>
	inline T Clamp(T value, T min, T max) { return value < min ? min : (value > max ? max : value); }

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

	// Wraps value within the range [min, max]
	template <zore::numeric T>
	inline T WrapClamp(T value, T min, T max) {
		T delta = max - min;
		while (value < min)
			value += delta;
		while (value >= max)
			value -= delta;
		return value;
	}
}