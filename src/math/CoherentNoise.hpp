#pragma once

#include "utils/DataTypes.hpp"
#include <array>

namespace zm {

	//========================================================================
	//  Noise Generator Base Class
	//========================================================================

	class NoiseGenerator {
	public:
		template <typename T>
		float Eval(T x, T y);
		template <typename T>
		float Eval(T x, T y, T z);
		template <typename T>
		float Fractal(T x, T y);
		template <typename T>
		float Fractal(T x, T y, T z);

		virtual void SetSeed(unsigned int seed);
		void SetOctaves(byte value);
		void SetFrequency(float value);
		void SetLacunarity(float value);
		void SetPersistence(float value);
		void SetReturnNeg(bool value);

	protected:
		NoiseGenerator(unsigned int seed);
		virtual float Noise(byte offset, float x, float y) = 0;
		virtual float Noise(byte offset, float x, float y, float z) = 0;
		inline byte P(int index);

		std::array<byte, 256> perm;
		byte octaves = 1;
		float frequency = 1.f;
		float persistence = 0.5f;
		float lacunarity = 2.f;
		bool returnNeg = false;
	};

	//========================================================================
	//  Simplex Noise Generator
	//========================================================================

	class SimplexGenerator : public NoiseGenerator {
	public:
		SimplexGenerator(unsigned int seed = 12345);
		~SimplexGenerator() = default;

	private:
		float Noise(byte offset, float x, float y) override;
		float Noise(byte offset, float x, float y, float z) override;

		float Grad(byte offset, int x, int y, float xd, float yd);
		float Grad(byte offset, int x, int y, int z, float xd, float yd, float zd);

		std::array<byte, 256> perm12;
	};

	//========================================================================
	//  Perlin Noise Generator
	//========================================================================

	class PerlinGenerator : public NoiseGenerator {
	public:
		PerlinGenerator(unsigned int seed = 12345);
		~PerlinGenerator() = default;

	private:
		float Noise(byte offset, float x, float y) override;
		float Noise(byte offset, float x, float y, float z) override;

		float Fade(float t);
		float Grad(int hash, float x, float y);
		float Grad(int hash, float x, float y, float z);
	};
}