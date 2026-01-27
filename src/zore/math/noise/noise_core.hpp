#pragma once

#include "zore/utils/concepts.hpp"
#include "zore/utils/sized_integer.hpp"
#include "zore/math/simd.hpp"

namespace zm {

	//========================================================================
	//  Noise Base
	//========================================================================

	class Noise {
	public:
		// Constructors and Initializers --
		Noise(int32_t seed) : m_seed(seed), m_frequency(1.f) {};
		Noise& SetSeed(int32_t seed) { m_seed = seed;  return *this; }
		Noise& SetFrequency(float frequency) { m_frequency = frequency; return *this; }

		// Float Input --------------------
		virtual float Eval(float x) = 0;
		virtual float Eval(float x, float y) = 0;
		virtual float Eval(float x, float y, float z) = 0;

		// Float Array Input --------------
		virtual void Eval(float* x, float* out, uint32_t count) = 0;
		virtual void Eval(float* x, float* y, float* out, uint32_t count) = 0;
		virtual void Eval(float* x, float* y, float* z, float* out, uint32_t count) = 0;

		// Integer Array Input --------------
		virtual void Eval(int32_t* x, float* out, uint32_t count) = 0;
		virtual void Eval(int32_t* x, int32_t* y, float* out, uint32_t count) = 0;
		virtual void Eval(int32_t* x, int32_t* y, int32_t* z, float* out, uint32_t count) = 0;

		// SIMD8 Input --------------------
#if SIMD_INT32_8 == true
		virtual void Eval(simd<float, 8>& x, simd<float, 8>& out) = 0;
		virtual void Eval(simd<float, 8>& x, simd<float, 8>& y, simd<float, 8>& out) = 0;
		virtual void Eval(simd<float, 8>& x, simd<float, 8>& y, simd<float, 8>& z, simd<float, 8>& out) = 0;
#endif
		// SIMD4 Input --------------------
#if SIMD_INT32_4 == true
		virtual void Eval(simd<float, 4>& x, simd<float, 4>& out) = 0;
		virtual void Eval(simd<float, 4>& x, simd<float, 4>& y, simd<float, 4>& out) = 0;
		virtual void Eval(simd<float, 4>& x, simd<float, 4>& y, simd<float, 4>& z, simd<float, 4>& out) = 0;
#endif

	protected:
		int32_t m_seed;
		float m_frequency;
	};

	//========================================================================
	//  SIMD Array Input Utility
	//========================================================================

	template<typename D, typename R>
	ALWAYS_INLINE void UpdateStorage(D** x, R** o, uint32_t& remaining, uint32_t count) {
		(*x) += count;
		(*o) += count;
		remaining -= count;
	}

	template<typename D, typename R>
	ALWAYS_INLINE void UpdateStorage(D** x, D** y, R** o, uint32_t& remaining, uint32_t count) {
		(*y) += count;
		UpdateStorage(x, o, remaining, count);
	}

	template<typename D, typename R>
	ALWAYS_INLINE void UpdateStorage(D** x, D** y, D** z, R** o, uint32_t& remaining, uint32_t count) {
		(*z) += count;
		UpdateStorage(x, y, o, remaining, count);
	}

	template<typename D, typename R>
	ALWAYS_INLINE void UpdateStorage(D** x, D** y, D** z, D** w, R** o, uint32_t& remaining, uint32_t count) {
		(*w) += count;
		UpdateStorage(x, y, z, o, remaining, count);
	}
}