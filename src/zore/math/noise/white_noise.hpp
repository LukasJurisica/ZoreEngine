#pragma once

#include "zore/math/vector.hpp"
#include "zore/math/simd.hpp"
#include "zore/math/math.hpp"

namespace zm {

	//========================================================================
	//  White Noise
	//========================================================================

	class WhiteNoise {
	public:
		// Integer Input ------------------
		static float Eval(int32_t x);
		static float Eval(int32_t x, int32_t y);
		static float Eval(int32_t x, int32_t y, int32_t z);
		static float Eval(int32_t x, int32_t y, int32_t z, int32_t w);

		// Array Input --------------------
		static void Eval(int32_t* x, float* out, uint32_t count);
		static void Eval(int32_t* x, int32_t* y, float* out, uint32_t count);
		static void Eval(int32_t* x, int32_t* y, int32_t* z, float* out, uint32_t count);
		static void Eval(int32_t* x, int32_t* y, int32_t* z, int32_t* w, float* out, uint32_t count);

		// SIMD8 Input --------------------
#if SIMD_FLOAT32_8 == true && SIMD_INT32_8 == true
		static void Eval(const simd<int32_t, 8>& x, simd<float, 8>& out);
		static void Eval(const simd<int32_t, 8>& x, const simd<int32_t, 8>& y, simd<float, 8>& out);
		static void Eval(const simd<int32_t, 8>& x, const simd<int32_t, 8>& y, const simd<int32_t, 8>& z, simd<float, 8>& out);
		static void Eval(const simd<int32_t, 8>& x, const simd<int32_t, 8>& y, const simd<int32_t, 8>& z, const simd<int32_t, 8>& w, simd<float, 8>& out);
#endif
		// SIMD4 Input --------------------
#if SIMD_FLOAT32_4 == true && SIMD_INT32_4 == true
		static void Eval(const simd<int32_t, 4>& x, simd<float, 4>& out);
		static void Eval(const simd<int32_t, 4>& x, const simd<int32_t, 4>& y, simd<float, 4>& out);
		static void Eval(const simd<int32_t, 4>& x, const simd<int32_t, 4>& y, const simd<int32_t, 4>& z, simd<float, 4>& out);
		static void Eval(const simd<int32_t, 4>& x, const simd<int32_t, 4>& y, const simd<int32_t, 4>& z, const simd<int32_t, 4>& w, simd<float, 4>& out);
#endif

	public:
		
		//------------------------------------------------------------------------
		//  Single Float Output
		//------------------------------------------------------------------------
		
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
				
		//------------------------------------------------------------------------
		//  Vector2 Output
		//------------------------------------------------------------------------
				
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
		
		//------------------------------------------------------------------------
		//  Vector3 Output
		//------------------------------------------------------------------------
		
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
		
		//------------------------------------------------------------------------
		//  Vector4 Output
		//------------------------------------------------------------------------
		
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