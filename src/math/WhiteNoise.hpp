#pragma once
#include "math/MathUtils.hpp"
#include "utils/DataTypes.hpp"
#include "glm/glm.hpp"

namespace zm {

	class WhiteNoise {
	public:

		//========================================================================
		//  Floating Point Output
		//========================================================================

		static inline float GetNoise(int x, int seed) {
			return GetINoise(x, seed) * MAXUINT_RECIP;
		}

		static inline float GetNoise(int x, int y, int seed) {
			return GetINoise(x, y, seed) * MAXUINT_RECIP;
		}

		static inline float GetNoise(int x, int y, int z, int seed) {
			return GetINoise(x, y, z, seed) * MAXUINT_RECIP;
		}

		static inline void GetMultiNoise(int x, int seed, float* out, int count) {
			int base = (x * PrimeX);
			for (int i = 0; i < count; i++) {
				seed += PrimeW;
				out[i] = Shuffle(Hash(base, seed)) * MAXUINT_RECIP;
			}
		}

		static inline void GetMultiNoise(int x, int y, int seed, float* out, int count) {
			int base = (x * PrimeX) ^ (y * PrimeY);
			for (int i = 0; i < count; i++) {
				seed += PrimeW;
				out[i] = Shuffle(Hash(base, seed)) * MAXUINT_RECIP;
			}
		}

		static inline void GetMultiNoise(int x, int y, int z, int seed, float* out, int count) {
			int base = (x * PrimeX) ^ (y * PrimeY) ^ (z * PrimeZ);
			for (int i = 0; i < count; i++) {
				seed += PrimeW;
				out[i] = Shuffle(Hash(base, seed)) * MAXUINT_RECIP;
			}
		}

		//========================================================================
		//  Integer Output
		//========================================================================

		static inline uint GetINoise(int x, int seed) {
			return Shuffle(Hash(x * PrimeX, seed));
		}

		static inline uint GetINoise(int x, int y, int seed) {
			return Shuffle(Hash(x * PrimeX, y * PrimeY, seed));
		}

		static inline uint GetINoise(int x, int y, int z, int seed) {
			return Shuffle(Hash(x * PrimeX, y * PrimeY, z * PrimeZ, seed));
		}

		static inline void GetMultiINoise(int x, int seed, uint* out, int count) {
			int base = (x * PrimeX);
			for (int i = 0; i < count; i++) {
				seed += PrimeW;
				out[i] = Shuffle(Hash(base, seed));
			}
		}

		static inline void GetMultiINoise(int x, int y, int seed, uint* out, int count) {
			int base = (x * PrimeX) ^ (y * PrimeY);
			for (int i = 0; i < count; i++) {
				seed += PrimeW;
				out[i] = Shuffle(Hash(base, seed));
			}
		}

		static inline void GetMultiINoise(int x, int y, int z, int seed, uint* out, int count) {
			int base = (x * PrimeX) ^ (y * PrimeY) ^ (z * PrimeZ);
			for (int i = 0; i < count; i++) {
				seed += PrimeW;
				out[i] = Shuffle(Hash(base, seed));
			}
		}

		//========================================================================
		//  Utility Functions
		//========================================================================

		static inline void PrimeSquareLattice(int& x1, int& y1, int& x2, int& y2) {
			x1 *= PrimeX;
			y1 *= PrimeY;
			x2 = x1 + PrimeX;
			y2 = y1 + PrimeY;
		}

		static inline void PrimeCubeLattice(int& x1, int& y1, int& z1, int& x2, int& y2, int& z2) {
			x1 *= PrimeX;
			y1 *= PrimeY;
			z1 *= PrimeZ;
			x2 = x1 + PrimeX;
			y2 = y1 + PrimeY;
			z2 = y1 + PrimeZ;
		}

		static inline uint Hash(int xPrimed, int seed) {
			return (xPrimed ^ seed) * 668265261;
		}

		static inline uint Hash(int xPrimed, int yPrimed, int seed) {
			return (xPrimed ^ yPrimed ^ seed) * 668265261;
		}

		static inline uint Hash(int xPrimed, int yPrimed, int zPrimed, int seed) {
			return (xPrimed ^ yPrimed ^ zPrimed ^ seed) * 668265261;
		}

		static inline uint Shuffle(uint hash) {
			hash *= hash;
			return hash ^ (hash << 19);
		}

	private:
		static constexpr int PrimeX =  501125321;
		static constexpr int PrimeY = 1136930381;
		static constexpr int PrimeZ = 1720413743;
		static constexpr int PrimeW = -699602503;
		static constexpr float MAXINT_RECIP  = 1.f / 2147483648.f;
		static constexpr float MAXUINT_RECIP = 1.f / 4294967296.f;

	public:

		//========================================================================
		//  Single Float Output
		//========================================================================

		static inline float Eval1(float p) {
			p = zm::Fract(p * .1031f);
			p *= p + 33.33f;
			p *= p + p;
			return zm::Fract(p);
		}

		static inline float Eval1(glm::vec2 p) {
			glm::vec3 p3 = glm::fract(glm::vec3(p.x, p.y, p.x) * .1031f);
			p3 += glm::dot(p3, glm::vec3(p3.y, p3.z, p3.x) + 33.33f);
			return glm::fract((p3.x + p3.y) * p3.z);
		}

		static inline float Eval1(glm::vec3 p3) {
			p3 = glm::fract(p3 * .1031f);
			p3 += glm::dot(p3, glm::vec3(p3.y, p3.z, p3.x) + 31.32f);
			return glm::fract((p3.x + p3.y) * p3.z);
		}
		
		//========================================================================
		//  Vector2 Output
		//========================================================================
		
		static inline glm::vec2 Eval2(float p) {
			glm::vec3 p3 = glm::fract(glm::vec3(p) * glm::vec3(.1031f, .1030f, .0973f));
			p3 += glm::dot(p3, glm::vec3(p3.y, p3.z, p3.x) + 33.33f);
			return glm::fract((glm::vec2(p3.x) + glm::vec2(p3.y, p3.z)) * glm::vec2(p3.z, p3.y));
		}

		static inline glm::vec2 Eval2(glm::vec2 p) {
			glm::vec3 p3 = glm::fract(glm::vec3(p.x, p.y, p.x) * glm::vec3(.1031f, .1030f, .0973f));
			p3 += glm::dot(p3, glm::vec3(p3.y, p3.z, p3.x) + 33.33f);
			return glm::fract((glm::vec2(p3.x) + glm::vec2(p3.y, p3.z)) * glm::vec2(p3.z, p3.y));
		}

		static inline glm::vec2 Eval2(glm::vec3 p3) {
			p3 = glm::fract(p3 * glm::vec3(.1031f, .1030f, .0973f));
			p3 += glm::dot(p3, glm::vec3(p3.y, p3.z, p3.x) + 33.33f);
			return glm::fract((glm::vec2(p3.x) + glm::vec2(p3.y, p3.z)) * glm::vec2(p3.z, p3.y));
		}

		//========================================================================
		//  Vector3 Output
		//========================================================================

		static inline glm::vec3 Eval3(float p) {
			glm::vec3 p3 = glm::fract(glm::vec3(p) * glm::vec3(.1031f, .1030f, .0973f));
			p3 += glm::dot(p3, glm::vec3(p3.y, p3.z, p3.x) + 33.33f);
			return glm::fract((glm::vec3(p3.x, p3.x, p3.y) + glm::vec3(p3.y, p3.z, p3.z)) * glm::vec3(p3.z, p3.y, p3.x));
		}

		static inline glm::vec3 Eval3(glm::vec2 p) {
			glm::vec3 p3 = glm::fract(glm::vec3(p.x, p.y, p.x) * glm::vec3(.1031f, .1030f, .0973f));
			p3 += glm::dot(p3, glm::vec3(p3.y, p3.z, p3.x) + 33.33f);
			return glm::fract((glm::vec3(p3.x, p3.x, p3.y) + glm::vec3(p3.y, p3.z, p3.z)) * glm::vec3(p3.z, p3.y, p3.x));
		}

		static inline glm::vec3 Eval3(glm::vec3 p3) {
			p3 = glm::fract(p3 * glm::vec3(.1031f, .1030f, .0973f));
			p3 += glm::dot(p3, glm::vec3(p3.y, p3.z, p3.x) + 33.33f);
			return glm::fract((glm::vec3(p3.x, p3.x, p3.y) + glm::vec3(p3.y, p3.z, p3.z)) * glm::vec3(p3.z, p3.y, p3.x));
		}

		//========================================================================
		//  Vector4 Output
		//========================================================================

		static inline glm::vec4 Eval4(float p) {
			glm::vec4 p4 = glm::fract(glm::vec4(p) * glm::vec4(.1031f, .1030f, .0973f, .1099f));
			p4 += glm::dot(p4, glm::vec4(p4.w, p4.z, p4.x, p4.y) + 33.33f);
			return glm::fract((glm::vec4(p4.x, p4.x, p4.y, p4.z) + glm::vec4(p4.y, p4.z, p4.z, p4.w)) * glm::vec4(p4.z, p4.y, p4.w, p4.x));
		}

		static inline glm::vec4 Eval4(glm::vec2 p) {
			glm::vec4 p4 = glm::fract(glm::vec4(p.x, p.y, p.x, p.y) * glm::vec4(.1031f, .1030f, .0973f, .1099f));
			p4 += glm::dot(p4, glm::vec4(p4.w, p4.z, p4.x, p4.y) + 33.33f);
			return glm::fract((glm::vec4(p4.x, p4.x, p4.y, p4.z) + glm::vec4(p4.y, p4.z, p4.z, p4.w)) * glm::vec4(p4.z, p4.y, p4.w, p4.x));
		}

		static inline glm::vec4 Eval4(glm::vec3 p) {
			glm::vec4 p4 = glm::fract(glm::vec4(p.x, p.y, p.z, p.x) * glm::vec4(.1031f, .1030f, .0973f, .1099f));
			p4 += glm::dot(p4, glm::vec4(p4.w, p4.z, p4.x, p4.y) + 33.33f);
			return glm::fract((glm::vec4(p4.x, p4.x, p4.y, p4.z) + glm::vec4(p4.y, p4.z, p4.z, p4.w)) * glm::vec4(p4.z, p4.y, p4.w, p4.x));
		}

		static inline glm::vec4 Eval4(glm::vec4 p4) {
			p4 = glm::fract(p4 * glm::vec4(.1031f, .1030f, .0973f, .1099f));
			p4 += glm::dot(p4, glm::vec4(p4.w, p4.z, p4.x, p4.y) + 33.33f);
			return glm::fract((glm::vec4(p4.x, p4.x, p4.y, p4.z) + glm::vec4(p4.y, p4.z, p4.z, p4.w)) * glm::vec4(p4.z, p4.y, p4.w, p4.x));
		}
	};
}