#include "math/CoherentNoise.hpp"
#include "math/MathUtils.hpp"

#define GLM_SWIZZLE_XYZW
#include "glm/glm.hpp"

#include "debug/Debug.hpp"
#include <random>

namespace zm {

	//========================================================================
	//  Noise Generator Base Class
	//========================================================================

	NoiseGenerator::NoiseGenerator(unsigned int seed) {
		SetSeed(seed);
	}

	//------------------------------------------------------------------------
	//	2D single octave evaluation
	//------------------------------------------------------------------------

	template <typename T>
	float NoiseGenerator::Eval(T x, T y) {
		return Noise(0, x * frequency, y * frequency);
	}
	template float NoiseGenerator::Eval(int x, int y);
	template float NoiseGenerator::Eval(float x, float y);

	//------------------------------------------------------------------------
	//	3D single octave evaluation
	//------------------------------------------------------------------------

	template <typename T>
	float NoiseGenerator::Eval(T x, T y, T z) {
		return Noise(0, x * frequency, y * frequency, z * frequency);
	}
	template float NoiseGenerator::Eval(int x, int y, int z);
	template float NoiseGenerator::Eval(float x, float y, float z);

	//------------------------------------------------------------------------
	//	2D multi octave evaluation
	//------------------------------------------------------------------------

	template <typename T>
	float NoiseGenerator::Fractal(T x, T y) {
		float amplitude = 1.f;
		float sum = 0.f;
		float fractalBound = 0.f;
		float locFreq = frequency;

		for (int i = 0; i < octaves; i++) {
			sum += amplitude * Noise(perm[i], x * locFreq, y * locFreq);

			fractalBound += amplitude;
			amplitude *= persistence;
			locFreq *= lacunarity;
		}

		return sum / fractalBound;
	}
	template float NoiseGenerator::Fractal(int x, int y);
	template float NoiseGenerator::Fractal(float x, float y);

	//------------------------------------------------------------------------
	//	3D multi octave evaluation
	//------------------------------------------------------------------------

	template <typename T>
	float NoiseGenerator::Fractal(T x, T y, T z) {
		float amplitude = 1.f;
		float sum = 0.f;
		float fractalBound = 0.f;
		float locFreq = frequency;

		for (int i = 0; i < octaves; i++) {
			sum += amplitude * Noise(perm[i], x * locFreq, y * locFreq, z * locFreq);

			fractalBound += amplitude;
			amplitude *= persistence;
			locFreq *= lacunarity;
		}

		return sum / fractalBound;
	}
	template float NoiseGenerator::Fractal(int x, int y, int z);
	template float NoiseGenerator::Fractal(float x, float y, float z);

	//------------------------------------------------------------------------
	//	Noise Generator Parameter setting Functions
	//------------------------------------------------------------------------

	void NoiseGenerator::SetSeed(unsigned int seed) {
		for (int i = 0; i < 256; i++)
			perm[i] = i;
		std::default_random_engine engine(seed);
		std::shuffle(perm.begin(), perm.end(), engine);
	}

	void NoiseGenerator::SetFrequency(float value) {
		frequency = value;
	}

	void NoiseGenerator::SetReturnNeg(bool value) {
		returnNeg = value;
	}

	void NoiseGenerator::SetOctaves(byte value) {
		octaves = value;
	}

	void NoiseGenerator::SetPersistence(float value) {
		persistence = value;
	}

	void NoiseGenerator::SetLacunarity(float value) {
		lacunarity = value;
	}

	byte NoiseGenerator::P(int index) {
		return perm[index & 255];
	}

	//========================================================================
	//  Simplex Noise Generator
	//========================================================================

	const float X_GRAD[] = { 1,-1, 1,-1, 1,-1, 1,-1, 0, 0, 0, 0 };
	const float Y_GRAD[] = { 1, 1,-1,-1, 0, 0, 0, 0, 1,-1, 1,-1 };
	const float Z_GRAD[] = { 0, 0, 0, 0, 1, 1,-1,-1, 1, 1,-1,-1 };
	//const float X_GRAD4[] = { 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1,-1,-1,-1,-1, 1, 1, 1, 1,-1,-1,-1,-1, 1, 1, 1, 1,-1,-1,-1,-1 };
	//const float Y_GRAD4[] = { 1, 1, 1, 1,-1,-1,-1,-1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1,-1,-1, 1, 1,-1,-1, 1, 1,-1,-1, 1, 1,-1,-1 };
	//const float Z_GRAD4[] = { 1, 1,-1,-1, 1, 1,-1,-1, 1, 1,-1,-1, 1, 1,-1,-1, 0, 0, 0, 0, 0, 0, 0, 0, 1,-1, 1,-1, 1,-1, 1,-1 };
	//const float W_GRAD4[] = { 1,-1, 1,-1, 1,-1, 1,-1, 1,-1, 1,-1, 1,-1, 1,-1, 1,-1, 1,-1, 1,-1, 1,-1, 0, 0, 0, 0, 0, 0, 0, 0 };
	const float F2 = (std::sqrt(3.f) - 1.f) / 2.f;
	const float G2 = (3.f - std::sqrt(3.f)) / 6.f;
	const float F3 = 1.f / 3.f;
	const float G3 = 1.f / 6.f;
	//const float F4 = (std::sqrt(5.f) - 1.f) / 4.f;
	//const float G4 = (5.f - std::sqrt(5.f)) / 20.f;

	SimplexGenerator::SimplexGenerator(unsigned int seed) : NoiseGenerator(seed), perm12{} {
		for (int i = 0; i < 256; i++)
			perm12[i] = perm[i] % 12;
	}

	float SimplexGenerator::Noise(byte offset, float x, float y) {
		float n0, n1, n2, t;

		t = (x + y) * F2;
		int i = zm::Floor(x + t);
		int j = zm::Floor(y + t);

		t = (i + j) * G2;
		float x0 = x - (i - t);
		float y0 = y - (j - t);

		int i1, j1;
		if (x0 > y0) { i1 = 1; j1 = 0; }
		else { i1 = 0; j1 = 1; }

		float x1 = x0 - i1 + G2;
		float y1 = y0 - j1 + G2;
		float x2 = x0 - 1 + 2 * G2;
		float y2 = y0 - 1 + 2 * G2;

		t = 0.5f - x0 * x0 - y0 * y0;
		if (t < 0.f) n0 = 0.f;
		else {
			t *= t;
			n0 = t * t * Grad(offset, i, j, x0, y0);
		}

		t = 0.5f - x1 * x1 - y1 * y1;
		if (t < 0.f) n1 = 0.f;
		else {
			t *= t;
			n1 = t * t * Grad(offset, i + i1, j + j1, x1, y1);
		}

		t = 0.5f - x2 * x2 - y2 * y2;
		if (t < 0.f) n2 = 0.f;
		else {
			t *= t;
			n2 = t * t * Grad(offset, i + 1, j + 1, x2, y2);
		}

		return 70 * (n0 + n1 + n2);
	}

	float SimplexGenerator::Noise(byte offset, float x, float y, float z) {
		float n0, n1, n2, n3, t;

		t = (x + y + z) * F3;
		int i = zm::Floor(x + t);
		int j = zm::Floor(y + t);
		int k = zm::Floor(z + t);

		t = (i + j + k) * G3;
		float x0 = x - (i - t);
		float y0 = y - (j - t);
		float z0 = z - (k - t);

		int i1, j1, k1;
		int i2, j2, k2;

		if (x0 >= y0) {
			if (y0 >= z0) { i1 = 1; j1 = 0; k1 = 0; i2 = 1; j2 = 1; k2 = 0; }
			else if (x0 >= z0) { i1 = 1; j1 = 0; k1 = 0; i2 = 1; j2 = 0; k2 = 1; }
			else { i1 = 0; j1 = 0; k1 = 1; i2 = 1; j2 = 0; k2 = 1; }
		}
		else {
			if (y0 < z0) { i1 = 0; j1 = 0; k1 = 1; i2 = 0; j2 = 1; k2 = 1; }
			else if (x0 < z0) { i1 = 0; j1 = 1; k1 = 0; i2 = 0; j2 = 1; k2 = 1; }
			else { i1 = 0; j1 = 1; k1 = 0; i2 = 1; j2 = 1; k2 = 0; }
		}

		float x1 = x0 - i1 + G3;
		float y1 = y0 - j1 + G3;
		float z1 = z0 - k1 + G3;
		float x2 = x0 - i2 + 2 * G3;
		float y2 = y0 - j2 + 2 * G3;
		float z2 = z0 - k2 + 2 * G3;
		float x3 = x0 - 1 + 3 * G3;
		float y3 = y0 - 1 + 3 * G3;
		float z3 = z0 - 1 + 3 * G3;

		t = 0.6f - x0 * x0 - y0 * y0 - z0 * z0;
		if (t < 0.f) n0 = 0.f;
		else {
			t *= t;
			n0 = t * t * Grad(offset, i, j, k, x0, y0, z0);
		}

		t = 0.6f - x1 * x1 - y1 * y1 - z1 * z1;
		if (t < 0.f) n1 = 0.f;
		else {
			t *= t;
			n1 = t * t * Grad(offset, i + i1, j + j1, k + k1, x1, y1, z1);
		}

		t = 0.6f - x2 * x2 - y2 * y2 - z2 * z2;
		if (t < 0.f) n2 = 0.f;
		else {
			t *= t;
			n2 = t * t * Grad(offset, i + i2, j + j2, k + k2, x2, y2, z2);
		}

		t = 0.6f - x3 * x3 - y3 * y3 - z3 * z3;
		if (t < 0.f) n3 = 0.f;
		else {
			t *= t;
			n3 = t * t * Grad(offset, i + 1, j + 1, k + 1, x3, y3, z3);
		}

		return 32.f * (n0 + n1 + n2 + n3);
	}

	float SimplexGenerator::Grad(byte offset, int x, int y, float xd, float yd) {
		byte pos = perm12[(x + perm[(y + offset) & 255]) & 255];
		return xd * X_GRAD[pos] + yd * Y_GRAD[pos];
	}

	float SimplexGenerator::Grad(byte offset, int x, int y, int z, float xd, float yd, float zd) {
		byte pos = perm12[(x + perm[(y + perm[(z + offset) & 255]) & 255]) & 255];
		return xd * X_GRAD[pos] + yd * Y_GRAD[pos] + zd * Z_GRAD[pos];
	}

	//========================================================================
	//  Perlin Noise Generator
	//========================================================================

	PerlinGenerator::PerlinGenerator(unsigned int seed) : NoiseGenerator(seed) {}

	float PerlinGenerator::Noise(byte offset, float x, float y) {
		int xi = zm::Floor(x);
		int yi = zm::Floor(y);
		x -= xi;
		y -= yi;
		float u = Fade(x);
		float v = Fade(y);

		int a = P(xi);
		int b = P(xi + 1);

		float g1 = Grad(P(a + yi), x, y);
		float g2 = Grad(P(b + yi), x - 1, y);
		float g3 = Grad(P(a + yi + 1), x, y - 1);
		float g4 = Grad(P(b + yi + 1), x - 1, y - 1);

		float i1 = zm::Lerp(g1, g2, u);
		float i2 = zm::Lerp(g3, g4, u);
		return zm::Lerp(i1, i2, v);
	}

	float PerlinGenerator::Noise(byte offset, float x, float y, float z) {
		int xi = zm::Floor(x);
		int yi = zm::Floor(y);
		int zi = zm::Floor(z);
		x -= xi;
		y -= yi;
		z -= zi;
		float u = Fade(x);
		float v = Fade(y);
		float w = Fade(z);

		int a = P(xi) + yi;
		int b = P(xi + 1) + yi;
		int aa = P(a) + zi;
		int ba = P(b) + zi;
		int ab = P(a + 1) + zi;
		int bb = P(b + 1) + zi;

		float g1 = Grad(P(aa), x, y, z);
		float g2 = Grad(P(ba), x - 1, y, z);
		float g3 = Grad(P(ab), x, y - 1, z);
		float g4 = Grad(P(bb), x - 1, y - 1, z);
		float g5 = Grad(P(aa + 1), x, y, z - 1);
		float g6 = Grad(P(ba + 1), x - 1, y, z - 1);
		float g7 = Grad(P(ab + 1), x, y - 1, z - 1);
		float g8 = Grad(P(bb + 1), x - 1, y - 1, z - 1);

		float i1 = zm::Lerp(g1, g2, u);
		float i2 = zm::Lerp(g3, g4, u);
		float i3 = zm::Lerp(g5, g6, u);
		float i4 = zm::Lerp(g7, g8, u);
		float i5 = zm::Lerp(i1, i2, v);
		float i6 = zm::Lerp(i3, i4, v);
		return zm::Lerp(i5, i6, w);
	}

	float PerlinGenerator::Fade(float t) {
		return t * t * t * (t * (t * 6 - 15) + 10);
	}

	float PerlinGenerator::Grad(int hash, float x, float y) {
		switch (hash & 3) {
		case 0: return  x + y;
		case 1: return -x + y;
		case 2: return  x - y;
		case 3: return -x - y;
		default: return 0;
		}
	}

	float PerlinGenerator::Grad(int hash, float x, float y, float z) {
		hash &= 15;
		float u = hash < 8 ? x : y,
			v = hash < 4 ? y : hash == 12 || hash == 14 ? x : z;
		return ((hash & 1) == 0 ? u : -u) + ((hash & 2) == 0 ? v : -v);
	}
}