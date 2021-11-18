#pragma once
#define _USE_MATH_DEFINES
#include <cmath>

namespace zm {

	// Returns the next interger smaller than value
	inline int Floor(float value) { return static_cast<int>(value) - (value < 0); }

	// Returns the next interger larger than value
	inline int Ceil(float value) { return static_cast<int>(value) + (value > 0); }

	// Returns the nearest integer to value
	inline int Round(float value) { return Floor(value + 0.5f); }

	// Returns the fractional component of value
	inline float Fract(float value) { return value - static_cast<int>(value); }

	// Returns the smaller value between a and b
	template <typename type>
	inline type Min(type a, type b) { return a < b ? a : b; }

	// Returns the larger value between a and b
	template <typename type>
	inline type Max(type a, type b) { return a > b ? a : b; }

	// Returns the absolute value of value
	template <typename type>
	type Abs(type value) { return value > 0 ? value : -value; }

	// Ensures value is clamped within the range [min, max]
	template <typename type>
	inline type Clamp(type value, type min, type max) { return Max(Min(value, max), min); }

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

	// Performs a linear interpolation of value between a and b
	template <typename type>
	inline float Lerp(type a, type b, float value) { return a + value * (b - a); }

	// Performs an inverse linear interpolation of value between a and b
	template <typename type>
	inline float InvLerp(type a, type b, float value) { return (value - a) / (b - a); }

	template <typename type>
	inline float Smoothstep1(type a, type b, float t) {
		t = Clamp((t - a) / (b - a), 0.f, 1.f);
		return t * t * (3 - (2 * t));
	}

	template <typename type>
	inline float Smoothstep2(type a, type b, float t) {
		t = Clamp((t - a) / (b - a), 0.f, 1.f);
		return t * t * t * (t * (t * 6 - 15) + 10);
	}

	template <typename type>
	inline float Dist(type sx, type sy, type dx, type dy) {
		dx -= sx; dy -= sy;
		return std::sqrt((dx * dx) + (dy * dy));
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