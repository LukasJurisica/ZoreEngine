#include "zore/math/noise/white_noise.hpp"
#include "zore/math/noise/noise_core.hpp"
#include <bit>

namespace zm {

	//========================================================================
	//  Utility Constants
	//========================================================================

	static inline constexpr int32_t PRIME_X =  501125321;
	static inline constexpr int32_t PRIME_Y = 1136930381;
	static inline constexpr int32_t PRIME_Z = 1720413743;
	static inline constexpr int32_t PRIME_W = 2860486313;
	static inline constexpr int32_t PRIME_S =  668265261;
	static inline constexpr float MAXINT_RECIP  = 1.f / 2147483648.f;
	static inline constexpr float MAXUINT_RECIP = 1.f / 4294967296.f;

	//========================================================================
	//  Utility Functions
	//========================================================================

	template<typename SIMD_I, typename SIMD_F>
	ALWAYS_INLINE void ShuffleSIMD(SIMD_I& v, SIMD_F& out) {
		v *= PRIME_S;
		v *= v;
		v = v ^ (v << 19);
		v = (v & 0x007FFFFF) | 0x3F800000;
		out = std::bit_cast<SIMD_F>(v) - 1.f;
	}

	ALWAYS_INLINE float Shuffle(int32_t v) {
		v *= PRIME_S;
		v *= v;
		v = v ^ (v << 19);
		v = (v & 0x007FFFFF) | 0x3F800000;
		return std::bit_cast<float>(v) - 1.f;
	}

	//========================================================================
	//  Integer White Noise
	//========================================================================

	float WhiteNoise::Eval(int32_t x) {
		return Shuffle((x * PRIME_X));
	}

	float WhiteNoise::Eval(int32_t x, int32_t y) {
		return Shuffle((x * PRIME_X) ^ (y * PRIME_Y));
	}

	float WhiteNoise::Eval(int32_t x, int32_t y, int32_t z) {
		return Shuffle((x * PRIME_X) ^ (y * PRIME_Y) ^ (z * PRIME_Z));
	}

	float WhiteNoise::Eval(int32_t x, int32_t y, int32_t z, int32_t w) {
		return Shuffle((x * PRIME_X) ^ (y * PRIME_Y) ^ (z * PRIME_Z) ^ (w * PRIME_W));
	}

	//========================================================================
	//  Array White Noise
	//========================================================================

	void WhiteNoise::Eval(int32_t* x, float* out, uint32_t count) {
#if SIMD_FLOAT32_8 == true && SIMD_INT32_8 == true
		while (count >= 8) {
			simd<float, 8> result;
			Eval(simd<int32_t, 8>(x), result);
			result.unload(out);
			UpdateStorage(&x, &out, count, 8);
		}
#endif
#if SIMD_FLOAT32_4 == true && SIMD_INT32_4 == true
		while (count >= 4) {
			simd<float, 4> result;
			Eval(simd<int32_t, 4>(x), result);
			result.unload(out);
			UpdateStorage(&x, &out, count, 4);
		}
#endif
		while (count > 0) {
			*out = Eval(*x);
			UpdateStorage(&x, &out, count, 1);
		}
	}

	void WhiteNoise::Eval(int32_t* x, int32_t* y, float* out, uint32_t count) {
#if SIMD_FLOAT32_8 == true && SIMD_INT32_8 == true
		while (count >= 8) {
			simd<float, 8> result;
			Eval(simd<int32_t, 8>(x), simd<int32_t, 8>(y), result);
			result.unload(out);
			UpdateStorage(&x, &y, &out, count, 8);
		}
#endif
#if SIMD_FLOAT32_4 == true && SIMD_INT32_4 == true
		while (count >= 4) {
			simd<float, 4> result;
			Eval(simd<int32_t, 4>(x), simd<int32_t, 4>(y), result);
			result.unload(out);
			UpdateStorage(&x, &y, &out, count, 4);
		}
#endif
		while (count > 0) {
			*out = Eval(*x, *y);
			UpdateStorage(&x, &y, &out, count, 1);
		}
	}

	void WhiteNoise::Eval(int32_t* x, int32_t* y, int32_t* z, float* out, uint32_t count) {
#if SIMD_FLOAT32_8 == true && SIMD_INT32_8 == true
		while (count >= 8) {
			simd<float, 8> result;
			Eval(simd<int32_t, 8>(x), simd<int32_t, 8>(y), simd<int32_t, 8>(z), result);
			result.unload(out);
			UpdateStorage(&x, &y, &z, &out, count, 8);
		}
#endif
#if SIMD_FLOAT32_4 == true && SIMD_INT32_4 == true
		while (count >= 4) {
			simd<float, 4> result;
			Eval(simd<int32_t, 4>(x), simd<int32_t, 4>(y), simd<int32_t, 4>(z), result);
			result.unload(out);
			UpdateStorage(&x, &y, &z, &out, count, 4);
		}
#endif
		while (count > 0) {
			*out = Eval(*x, *y, *z);
			UpdateStorage(&x, &y, &z, &out, count, 1);
		}
	}

	void WhiteNoise::Eval(int32_t* x, int32_t* y, int32_t* z, int32_t* w, float* out, uint32_t count) {
#if SIMD_FLOAT32_8 == true && SIMD_INT32_8 == true
		while (count >= 8) {
			simd<float, 8> result;
			Eval(simd<int32_t, 8>(x), simd<int32_t, 8>(y), simd<int32_t, 8>(z), simd<int32_t, 8>(w), result);
			result.unload(out);
			UpdateStorage(&x, &y, &z, &w, &out, count, 8);
		}
#endif
#if SIMD_FLOAT32_4 == true && SIMD_INT32_4 == true
		while (count >= 4) {
			simd<float, 4> result;
			Eval(simd<int32_t, 4>(x), simd<int32_t, 4>(y), simd<int32_t, 4>(z), simd<int32_t, 4>(w), result);
			result.unload(out);
			UpdateStorage(&x, &y, &z, &w, &out, count, 4);
		}
#endif
		while (count > 0) {
			*out = Eval(*x, *y, *z, *w);
			UpdateStorage(&x, &y, &z, &w, &out, count, 1);
		}
	}

	//========================================================================
	//  SIMD8 White Noise
	//========================================================================

#if SIMD_FLOAT32_8 == true && SIMD_INT32_8 == true
	void WhiteNoise::Eval(const simd<int32_t, 8>& x, simd<float, 8>& out) {
		simd<int32_t, 8> hash = x * PRIME_X;
		ShuffleSIMD(hash, out);
	}

	void WhiteNoise::Eval(const simd<int32_t, 8>& x, const simd<int32_t, 8>& y, simd<float, 8>& out) {
		simd<int32_t, 8> hash = (x * PRIME_X) ^ (y * PRIME_Y);
		ShuffleSIMD(hash, out);
	}

	void WhiteNoise::Eval(const simd<int32_t, 8>& x, const simd<int32_t, 8>& y, const simd<int32_t, 8>& z, simd<float, 8>& out) {
		simd<int32_t, 8> hash = (x * PRIME_X) ^ (y * PRIME_Y) ^ (z * PRIME_Z);
		ShuffleSIMD(hash, out);
	}

	void WhiteNoise::Eval(const simd<int32_t, 8>& x, const simd<int32_t, 8>& y, const simd<int32_t, 8>& z, const simd<int32_t, 8>& w, simd<float, 8>& out) {
		simd<int32_t, 8> hash = (x * PRIME_X) ^ (y * PRIME_Y) ^ (z * PRIME_Z) ^ (w * PRIME_W);
		ShuffleSIMD(hash, out);
	}
#endif

	//========================================================================
	//  SIMD4 White Noise
	//========================================================================

#if SIMD_FLOAT32_4 == true && SIMD_INT32_4 == true
	void WhiteNoise::Eval(const simd<int32_t, 4>& x, simd<float, 4>& out) {
		simd<int32_t, 4> hash = x * PRIME_X;
		ShuffleSIMD(hash, out);
	}

	void WhiteNoise::Eval(const simd<int32_t, 4>& x, const simd<int32_t, 4>& y, simd<float, 4>& out) {
		simd<int32_t, 4> hash = (x * PRIME_X) ^ (y * PRIME_Y);
		ShuffleSIMD(hash, out);
	}

	void WhiteNoise::Eval(const simd<int32_t, 4>& x, const simd<int32_t, 4>& y, const simd<int32_t, 4>& z, simd<float, 4>& out) {
		simd<int32_t, 4> hash = (x * PRIME_X) ^ (y * PRIME_Y) ^ (z * PRIME_Z);
		ShuffleSIMD(hash, out);
	}

	void WhiteNoise::Eval(const simd<int32_t, 4>& x, const simd<int32_t, 4>& y, const simd<int32_t, 4>& z, const simd<int32_t, 4>& w, simd<float, 4>& out) {
		simd<int32_t, 4> hash = (x * PRIME_X) ^ (y * PRIME_Y) ^ (z * PRIME_Z) ^ (w * PRIME_W);
		ShuffleSIMD(hash, out);
	}
#endif
}