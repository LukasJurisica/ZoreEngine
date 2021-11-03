#pragma once

namespace zm {
	int LTWrap(int value, int min, int max);
	int GTWrap(int value, int min, int max);
	float Lerp(float a, float b, float t);
	float InvLerp(float a, float b, float t);

	// Faster, Less slope
	float Smoothstep1(float a, float b, float t);
	// Slower, More slope
	float Smoothstep2(float a, float b, float t);

	float Fract(float t);
	float Dist(float sx, float sy, float dx, float dy);
	float Dist(float sx, float sy, float sz, float dx, float dy, float dz);

	int Floor(float value);
	int Ceil(float value);
	int Round(float value);
	float PI();

	template <class type>
	type Min(const type& a, const type& b) { return a < b ? a : b; }

	template <class type>
	type Max(const type& a, const type& b) { return a > b ? a : b; }

	template <class type>
	type Clamp(const type& value, const type& min, const type& max) { return Max(Min(value, max), min); }

	template <class type>
	type WrapClamp(type value, const type& min, const type& max) {
		type delta = max - min;
		while (value < min)
			value += delta;
		while (value >= max)
			value -= delta;
		return value;
	}

	template <class type>
	type SqrDist(type x1, type y1, type x2, type y2) {
		x2 -= x1; y2 -= y1;
		return (x2 * x2) + (y2 * y2);
	}

	template <class type>
	type SqrDist(type x1, type y1, type z1, type x2, type y2, type z2) {
		x2 -= x1; y2 -= y1; z2 -= z1;
		return (x2 * x2) + (y2 * y2) + (z2 * z2);
	}

	template <class type>
	type Abs(type value) { return value > 0 ? value : -value; }
}