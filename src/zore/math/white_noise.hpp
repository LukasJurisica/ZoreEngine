#pragma once

#include "zore/utils/sized_integer.hpp"
#include "zore/math/vector.hpp"
#include "zore/math/math.hpp"

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

		static inline uint32_t GetINoise(int x, int seed) {
			return Shuffle(Hash(x * PrimeX, seed));
		}

		static inline uint32_t GetINoise(int x, int y, int seed) {
			return Shuffle(Hash(x * PrimeX, y * PrimeY, seed));
		}

		static inline uint32_t GetINoise(int x, int y, int z, int seed) {
			return Shuffle(Hash(x * PrimeX, y * PrimeY, z * PrimeZ, seed));
		}

		static inline void GetMultiINoise(int x, int seed, uint32_t* out, int count) {
			int base = (x * PrimeX);
			for (int i = 0; i < count; i++) {
				seed += PrimeW;
				out[i] = Shuffle(Hash(base, seed));
			}
		}

		static inline void GetMultiINoise(int x, int y, int seed, uint32_t* out, int count) {
			int base = (x * PrimeX) ^ (y * PrimeY);
			for (int i = 0; i < count; i++) {
				seed += PrimeW;
				out[i] = Shuffle(Hash(base, seed));
			}
		}

		static inline void GetMultiINoise(int x, int y, int z, int seed, uint32_t* out, int count) {
			int base = (x * PrimeX) ^ (y * PrimeY) ^ (z * PrimeZ);
			for (int i = 0; i < count; i++) {
				seed += PrimeW;
				out[i] = Shuffle(Hash(base, seed));
			}
		}

		//========================================================================
		//  Utility Functions
		//========================================================================

		static inline uint32_t Hash(int x, int seed) {
			return (x ^ seed) * 668265261;
		}

		static inline uint32_t Hash(int x, int y, int seed) {
			return (x ^ y ^ seed) * 668265261;
		}

		static inline uint32_t Hash(int x, int y, int z, int seed) {
			return (x ^ y ^ z ^ seed) * 668265261;
		}

		static inline uint32_t Shuffle(uint32_t hash) {
			hash *= hash;
			return hash ^ (hash << 19);
		}

	private:
		static inline constexpr int PrimeX =  501125321;
		static inline constexpr int PrimeY = 1136930381;
		static inline constexpr int PrimeZ = 1720413743;
		static inline constexpr int PrimeW = -699602503;
		static inline constexpr float MAXINT_RECIP  = 1.f / 2147483648.f;
		static inline constexpr float MAXUINT_RECIP = 1.f / 4294967296.f;

	public:

		//========================================================================
		//  Single Float Output
		//========================================================================

		static inline float Eval1(float p) {
			p = zm::Fract(p * .1031f);
			p *= p + 33.33f;
			return zm::Fract((p + p) * p);
		}

		static inline float Eval1(zm::vec2 p) {
			zm::vec3 p3 = zm::Fract(zm::vec3(p.x, p.y, p.x) * .1031f);
			p3 += zm::Dot(p3, zm::vec3(p3.y, p3.z, p3.x) + 33.33f);
			return zm::Fract((p3.x + p3.y) * p3.z);
		}

		static inline float Eval1(zm::vec3 p3) {
			p3 = zm::Fract(p3 * .1031f);
			p3 += zm::Dot(p3, zm::vec3(p3.y, p3.z, p3.x) + 31.32f);
			return zm::Fract((p3.x + p3.y) * p3.z);
		}
		
		//========================================================================
		//  Vector2 Output
		//========================================================================
		
		static inline zm::vec2 Eval2(float p) {
			zm::vec3 p3 = zm::Fract(zm::vec3(p) * zm::vec3(.1031f, .1030f, .0973f));
			p3 += zm::Dot(p3, zm::vec3(p3.y, p3.z, p3.x) + 33.33f);
			return zm::Fract((zm::vec2(p3.x) + zm::vec2(p3.y, p3.z)) * zm::vec2(p3.z, p3.y));
		}

		static inline zm::vec2 Eval2(zm::vec2 p) {
			zm::vec3 p3 = zm::Fract(zm::vec3(p.x, p.y, p.x) * zm::vec3(.1031f, .1030f, .0973f));
			p3 += zm::Dot(p3, zm::vec3(p3.y, p3.z, p3.x) + 33.33f);
			return zm::Fract((zm::vec2(p3.x) + zm::vec2(p3.y, p3.z)) * zm::vec2(p3.z, p3.y));
		}

		static inline zm::vec2 Eval2(zm::vec3 p3) {
			p3 = zm::Fract(p3 * zm::vec3(.1031f, .1030f, .0973f));
			p3 += zm::Dot(p3, zm::vec3(p3.y, p3.z, p3.x) + 33.33f);
			return zm::Fract((zm::vec2(p3.x) + zm::vec2(p3.y, p3.z)) * zm::vec2(p3.z, p3.y));
		}

		//========================================================================
		//  Vector3 Output
		//========================================================================

		static inline zm::vec3 Eval3(float p) {
			zm::vec3 p3 = zm::Fract(zm::vec3(p) * zm::vec3(.1031f, .1030f, .0973f));
			p3 += zm::Dot(p3, zm::vec3(p3.y, p3.z, p3.x) + 33.33f);
			return zm::Fract((zm::vec3(p3.x, p3.x, p3.y) + zm::vec3(p3.y, p3.z, p3.z)) * zm::vec3(p3.z, p3.y, p3.x));
		}

		static inline zm::vec3 Eval3(zm::vec2 p) {
			zm::vec3 p3 = zm::Fract(zm::vec3(p.x, p.y, p.x) * zm::vec3(.1031f, .1030f, .0973f));
			p3 += zm::Dot(p3, zm::vec3(p3.y, p3.z, p3.x) + 33.33f);
			return zm::Fract((zm::vec3(p3.x, p3.x, p3.y) + zm::vec3(p3.y, p3.z, p3.z)) * zm::vec3(p3.z, p3.y, p3.x));
		}

		static inline zm::vec3 Eval3(zm::vec3 p3) {
			p3 = zm::Fract(p3 * zm::vec3(.1031f, .1030f, .0973f));
			p3 += zm::Dot(p3, zm::vec3(p3.y, p3.z, p3.x) + 33.33f);
			return zm::Fract((zm::vec3(p3.x, p3.x, p3.y) + zm::vec3(p3.y, p3.z, p3.z)) * zm::vec3(p3.z, p3.y, p3.x));
		}

		//========================================================================
		//  Vector4 Output
		//========================================================================

		static inline zm::vec4 Eval4(float p) {
			zm::vec4 p4 = zm::Fract(zm::vec4(p) * zm::vec4(.1031f, .1030f, .0973f, .1099f));
			p4 += zm::Dot(p4, zm::vec4(p4.w, p4.z, p4.x, p4.y) + 33.33f);
			return zm::Fract((zm::vec4(p4.x, p4.x, p4.y, p4.z) + zm::vec4(p4.y, p4.z, p4.z, p4.w)) * zm::vec4(p4.z, p4.y, p4.w, p4.x));
		}

		static inline zm::vec4 Eval4(zm::vec2 p) {
			zm::vec4 p4 = zm::Fract(zm::vec4(p.x, p.y, p.x, p.y) * zm::vec4(.1031f, .1030f, .0973f, .1099f));
			p4 += zm::Dot(p4, zm::vec4(p4.w, p4.z, p4.x, p4.y) + 33.33f);
			return zm::Fract((zm::vec4(p4.x, p4.x, p4.y, p4.z) + zm::vec4(p4.y, p4.z, p4.z, p4.w)) * zm::vec4(p4.z, p4.y, p4.w, p4.x));
		}

		static inline zm::vec4 Eval4(zm::vec3 p) {
			zm::vec4 p4 = zm::Fract(zm::vec4(p.x, p.y, p.z, p.x) * zm::vec4(.1031f, .1030f, .0973f, .1099f));
			p4 += zm::Dot(p4, zm::vec4(p4.w, p4.z, p4.x, p4.y) + 33.33f);
			return zm::Fract((zm::vec4(p4.x, p4.x, p4.y, p4.z) + zm::vec4(p4.y, p4.z, p4.z, p4.w)) * zm::vec4(p4.z, p4.y, p4.w, p4.x));
		}

		static inline zm::vec4 Eval4(zm::vec4 p4) {
			p4 = zm::Fract(p4 * zm::vec4(.1031f, .1030f, .0973f, .1099f));
			p4 += zm::Dot(p4, zm::vec4(p4.w, p4.z, p4.x, p4.y) + 33.33f);
			return zm::Fract((zm::vec4(p4.x, p4.x, p4.y, p4.z) + zm::vec4(p4.y, p4.z, p4.z, p4.w)) * zm::vec4(p4.z, p4.y, p4.w, p4.x));
		}
	};
}