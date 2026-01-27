#pragma once

#include "zore/math/noise/noise_core.hpp"

namespace zm {

	//========================================================================
	//  Value Noise
	//========================================================================

	class ValueNoise : public Noise {
	public:
		// Constructors and Initializers --
		ValueNoise(int32_t seed) : Noise(seed) {}

		// Float Input --------------------
		float Eval(float x) override;
		float Eval(float x, float y) override;
		float Eval(float x, float y, float z) override;

		// Float Array Input --------------
		void Eval(float* x, float* out, uint32_t count) override;
		void Eval(float* x, float* y, float* out, uint32_t count) override;
		void Eval(float* x, float* y, float* z, float* out, uint32_t count) override;

		// Integer Array Input --------------
		void Eval(int32_t* x, float* out, uint32_t count) override;
		void Eval(int32_t* x, int32_t* y, float* out, uint32_t count) override;
		void Eval(int32_t* x, int32_t* y, int32_t* z, float* out, uint32_t count) override;

		// SIMD8 Input --------------------
#if SIMD_FLOAT32_8 == true && SIMD_INT32_8 == true
		void Eval(simd<float, 8>& x, simd<float, 8>& out) override;
		void Eval(simd<float, 8>& x, simd<float, 8>& y, simd<float, 8>& out) override;
		void Eval(simd<float, 8>& x, simd<float, 8>& y, simd<float, 8>& z, simd<float, 8>& out) override;
#endif
		// SIMD4 Input --------------------
#if SIMD_FLOAT32_4 == true && SIMD_INT32_4 == true
		void Eval(simd<float, 4>& x, simd<float, 4>& out) override;
		void Eval(simd<float, 4>& x, simd<float, 4>& y, simd<float, 4>& out) override;
		void Eval(simd<float, 4>& x, simd<float, 4>& y, simd<float, 4>& z, simd<float, 4>& out) override;
#endif
	};
}