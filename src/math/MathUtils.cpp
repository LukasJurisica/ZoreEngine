#include "MathUtils.hpp"
#define _USE_MATH_DEFINES
#include <cmath>

namespace zm {

	int LTWrap(int value, int min, int max) {
		if (value < min)
			value += (max - min);
		return value;
	}

	int GTWrap(int value, int min, int max) {
		if (value >= max)
			value -= (max - min);
		return value;
	}

	float Lerp(float a, float b, float t) {
		return a + t * (b - a);
	}

	float InvLerp(float a, float b, float t) {
		return (t - a) / (b - a);
	}

	float Smoothstep1(float a, float b, float t) {
		t = Clamp((t - a) / (b - a), 0.f, 1.f);
		return t * t * (3 - (2 * t));
	}

	float Smoothstep2(float a, float b, float t) {
		t = Clamp((t - a) / (b - a), 0.f, 1.f);
		return t * t * t * (t * (t * 6 - 15) + 10);
	}

	float Fract(float t) {
		return t - Floor(t);
	}

	float Dist(float sx, float sy, float dx, float dy) {
		dx -= sx; dy -= sy;
		return std::sqrt((dx * dx) + (dy * dy));
	}

	float Dist(float sx, float sy, float sz, float dx, float dy, float dz) {
		dx -= sx; dy -= sy; dz -= sz;
		return std::sqrt((dx * dx) + (dy * dy) + (dz * dz));
	}
	
	int Floor(float value) {
		int i = static_cast<int>(value);
		return i - (i > value);
	}

	int Ceil(float value) {
		int i = static_cast<int>(value);
		return i + (i < value);
	}

	int Round(float value) {
		return Floor(value + 0.5f);
	}

	float PI() {
		return static_cast<float>(M_PI);
	}
}