#include "zore/math/noise/value_noise.hpp"
#include "zore/math/noise/white_noise.hpp"
#include "zore/math/math.hpp"

namespace zm {

	//========================================================================
	//  Floating Point Value Noise
	//========================================================================

	float ValueNoise::Eval(float x) {
		x *= m_frequency;
		float x_floor = std::floor(x);
		int32_t x_i = static_cast<int32_t>(x_floor);
		float p0 = WhiteNoise::Eval(x_i + 0, m_seed);
		float p1 = WhiteNoise::Eval(x_i + 1, m_seed);
		float x_interp = Smoothstep(x - x_floor);
		return zm::Lerp(p0, p1, x_interp);
	}

	float ValueNoise::Eval(float x, float y) {
		x *= m_frequency;
		y *= m_frequency;
		float x_floor = std::floor(x);
		float y_floor = std::floor(y);
		int32_t x_i = static_cast<int32_t>(x_floor);
		int32_t y_i = static_cast<int32_t>(y_floor);
		alignas(16) int32_t x_a[4] = { x_i + 0, x_i + 1, x_i + 0, x_i + 1 };
		alignas(16) int32_t y_a[4] = { y_i + 0, y_i + 0, y_i + 1, y_i + 1 };
		alignas(16) int32_t seed_a[4] = { m_seed, m_seed, m_seed, m_seed };
		alignas(16) float p[4];
		WhiteNoise::Eval(x_a, y_a, seed_a, p, 4);
		float x_interp = Smoothstep(x - x_floor);
		float y_interp = Smoothstep(y - y_floor);
		float a = zm::Lerp(p[0], p[1], x_interp);
		float b = zm::Lerp(p[2], p[3], x_interp);
		return zm::Lerp(a, b, y_interp);
	}

	float ValueNoise::Eval(float x, float y, float z) {
		x *= m_frequency;
		y *= m_frequency;
		z *= m_frequency;
		float x_floor = std::floor(x);
		float y_floor = std::floor(y);
		float z_floor = std::floor(z);
		int32_t x_i = static_cast<int32_t>(x_floor);
		int32_t y_i = static_cast<int32_t>(y_floor);
		int32_t z_i = static_cast<int32_t>(z_floor);
		alignas(32) int32_t x_a[8] = { x_i + 0, x_i + 1, x_i + 0, x_i + 1, x_i + 0, x_i + 1, x_i + 0, x_i + 1 };
		alignas(32) int32_t y_a[8] = { y_i + 0, y_i + 0, y_i + 1, y_i + 1, y_i + 0, y_i + 0, y_i + 1, y_i + 1 };
		alignas(32) int32_t z_a[8] = { z_i + 0, z_i + 0, z_i + 0, z_i + 0, z_i + 1, z_i + 1, z_i + 1, z_i + 1 };
		alignas(32) int32_t seed_a[8] = { m_seed, m_seed, m_seed, m_seed, m_seed, m_seed, m_seed, m_seed };
		alignas(32) float p[8];
		WhiteNoise::Eval(x_a, y_a, z_a, seed_a, p, 8);
		float x_interp = Smoothstep(x - x_floor);
		float y_interp = Smoothstep(y - y_floor);
		float z_interp = Smoothstep(z - z_floor);
		float a0 = zm::Lerp(p[0], p[1], x_interp);
		float a1 = zm::Lerp(p[2], p[3], x_interp);
		float b0 = zm::Lerp(p[4], p[5], x_interp);
		float b1 = zm::Lerp(p[6], p[7], x_interp);
		float a = zm::Lerp(a0, a1, y_interp);
		float b = zm::Lerp(b0, b1, y_interp);
		return zm::Lerp(a, b, z_interp);
	}

	//========================================================================
	//  Float Array Value Noise
	//========================================================================

	void ValueNoise::Eval(float* x, float* out, uint32_t count) {
#if SIMD_FLOAT32_8 == true && SIMD_INT32_8 == true
		while (count >= 8) {
			simd::float32_8 x_simd(x);
			simd::float32_8 out_simd;
			Eval(x_simd, out_simd);
			out_simd.unload(out);
			UpdateStorage(&x, &out, count, 8);
		}
#endif
#if SIMD_FLOAT32_4 == true && SIMD_INT32_4 == true
		while (count >= 4) {
			simd::float32_4 x_simd(x);
			simd::float32_4 out_simd;
			Eval(x_simd, out_simd);
			out_simd.unload(out);
			UpdateStorage(&x, &out, count, 4);
		}
#endif
		while (count >= 0) {
			*out = Eval(*x);
			UpdateStorage(&x, &out, count, 1);
		}
	}

	void ValueNoise::Eval(float* x, float* y, float* out, uint32_t count) {
#if SIMD_FLOAT32_8 == true && SIMD_INT32_8 == true
		while (count >= 8) {
			simd::float32_8 x_simd(x);
			simd::float32_8 y_simd(y);
			simd::float32_8 out_simd;
			Eval(x_simd, y_simd, out_simd);
			out_simd.unload(out);
			UpdateStorage(&x, &y, &out, count, 8);
		}
#endif
#if SIMD_FLOAT32_4 == true && SIMD_INT32_4 == true
		while (count >= 4) {
			simd::float32_4 x_simd(x);
			simd::float32_4 y_simd(y);
			simd::float32_4 out_simd;
			Eval(x_simd, y_simd, out_simd);
			out_simd.unload(out);
			UpdateStorage(&x, &y, &out, count, 4);
		}
#endif
		while (count >= 0) {
			*out = Eval(*x);
			UpdateStorage(&x, &y, &out, count, 1);
		}
	}

	void ValueNoise::Eval(float* x, float* y, float* z, float* out, uint32_t count) {
#if SIMD_FLOAT32_8 == true && SIMD_INT32_8 == true
		while (count >= 8) {
			simd::float32_8 x_simd(x);
			simd::float32_8 y_simd(y);
			simd::float32_8 z_simd(z);
			simd::float32_8 out_simd;
			Eval(x_simd, y_simd, z_simd, out_simd);
			out_simd.unload(out);
			UpdateStorage(&x, &y, &z, &out, count, 8);
		}
#endif
#if SIMD_FLOAT32_4 == true && SIMD_INT32_4 == true
		while (count >= 4) {
			simd::float32_4 x_simd(x);
			simd::float32_4 y_simd(y);
			simd::float32_4 z_simd(z);
			simd::float32_4 out_simd;
			Eval(x_simd, y_simd, z_simd, out_simd);
			out_simd.unload(out);
			UpdateStorage(&x, &y, &z, &out, count, 4);
		}
#endif
		while (count >= 0) {
			*out = Eval(*x);
			UpdateStorage(&x, &y, &z, &out, count, 1);
		}
	}

	//========================================================================
	//  Integer Array Value Noise
	//========================================================================

	void ValueNoise::Eval(int32_t* x, float* out, uint32_t count) {
#if SIMD_FLOAT32_8 == true && SIMD_INT32_8 == true
		while (count >= 8) {
			simd::float32_8 x_simd = simd::float32_8(simd::int32_8(x));
			simd::float32_8 out_simd;
			Eval(x_simd, out_simd);
			out_simd.unload(out);
			UpdateStorage(&x, &out, count, 8);
		}
#endif
#if SIMD_FLOAT32_4 == true && SIMD_INT32_4 == true
		while (count >= 4) {
			simd::float32_4 x_simd = simd::float32_4(simd::int32_4(x));
			simd::float32_4 out_simd;
			Eval(x_simd, out_simd);
			out_simd.unload(out);
			UpdateStorage(&x, &out, count, 4);
		}
#endif
		while (count >= 0) {
			*out = Eval(static_cast<float>(*x));
			UpdateStorage(&x, &out, count, 1);
		}
	}

	void ValueNoise::Eval(int32_t* x, int32_t* y, float* out, uint32_t count) {
#if SIMD_FLOAT32_8 == true && SIMD_INT32_8 == true
		while (count >= 8) {
			simd::float32_8 x_simd = simd::float32_8(simd::int32_8(x));
			simd::float32_8 y_simd = simd::float32_8(simd::int32_8(y));
			simd::float32_8 out_simd;
			Eval(x_simd, y_simd, out_simd);
			out_simd.unload(out);
			UpdateStorage(&x, &y, &out, count, 8);
		}
#endif
#if SIMD_FLOAT32_4 == true && SIMD_INT32_4 == true
		while (count >= 4) {
			simd::float32_4 x_simd = simd::float32_4(simd::int32_4(x));
			simd::float32_4 y_simd = simd::float32_4(simd::int32_4(y));
			simd::float32_4 out_simd;
			Eval(x_simd, y_simd, out_simd);
			out_simd.unload(out);
			UpdateStorage(&x, &y, &out, count, 4);
		}
#endif
		while (count >= 0) {
			*out = Eval(*x);
			UpdateStorage(&x, &y, &out, count, 1);
		}
	}

	void ValueNoise::Eval(int32_t* x, int32_t* y, int32_t* z, float* out, uint32_t count) {
#if SIMD_FLOAT32_8 == true && SIMD_INT32_8 == true
		while (count >= 8) {
			simd::float32_8 x_simd = simd::float32_8(simd::int32_8(x));
			simd::float32_8 y_simd = simd::float32_8(simd::int32_8(y));
			simd::float32_8 z_simd = simd::float32_8(simd::int32_8(z));
			simd::float32_8 out_simd;
			Eval(x_simd, y_simd, z_simd, out_simd);
			out_simd.unload(out);
			UpdateStorage(&x, &y, &z, &out, count, 8);
		}
#endif
#if SIMD_FLOAT32_4 == true && SIMD_INT32_4 == true
		while (count >= 4) {
			simd::float32_4 x_simd = simd::float32_4(simd::int32_4(x));
			simd::float32_4 y_simd = simd::float32_4(simd::int32_4(y));
			simd::float32_4 z_simd = simd::float32_4(simd::int32_4(z));
			simd::float32_4 out_simd;
			Eval(x_simd, y_simd, z_simd, out_simd);
			out_simd.unload(out);
			UpdateStorage(&x, &y, &z, &out, count, 4);
		}
#endif
		while (count >= 0) {
			*out = Eval(*x);
			UpdateStorage(&x, &y, &z, &out, count, 1);
		}
	}

	//========================================================================
	//  SIMD8 Value Noise
	//========================================================================

#if SIMD_FLOAT32_8 == true && SIMD_INT32_8 == true
	void ValueNoise::Eval(const simd::float32_8& x, simd::float32_8& out) {
		x *= m_frequency;
		simd::float32_8 x_floor = simd::floor(x);
		simd::int32_8 x_i(x_floor);
		simd::int32_8 seed(m_seed);
		simd::float32_8 p0, p1;
		WhiteNoise::Eval(x_i + 0, seed, p0);
		WhiteNoise::Eval(x_i + 1, seed, p1);
		simd::float32_8 x_interp = Smoothstep(x - x_floor);
		out = Lerp(p0, p1, x_interp);
	}

	void ValueNoise::Eval(const simd::float32_8& x, const simd::float32_8& y, simd::float32_8& out) {
		x *= m_frequency;
		y *= m_frequency;
		simd::float32_8 x_floor = simd::floor(x);
		simd::float32_8 y_floor = simd::floor(y);
		simd::int32_8 x_i(x_floor);
		simd::int32_8 y_i(y_floor);
		simd::int32_8 seed(m_seed);
		simd::float32_8 p0, p1, p2, p3;
		WhiteNoise::Eval(x_i + 0, y_i + 0, seed, p0);
		WhiteNoise::Eval(x_i + 1, y_i + 0, seed, p1);
		WhiteNoise::Eval(x_i + 0, y_i + 1, seed, p2);
		WhiteNoise::Eval(x_i + 1, y_i + 1, seed, p3);
		simd::float32_8 x_interp = Smoothstep(x - x_floor);
		simd::float32_8 y_interp = Smoothstep(y - y_floor);
		simd::float32_8 a = zm::Lerp(p0, p1, x_interp);
		simd::float32_8 b = zm::Lerp(p2, p3, x_interp);
		out = zm::Lerp(a, b, y_interp);
	}

	void ValueNoise::Eval(const simd::float32_8& x, const simd::float32_8& y, const simd::float32_8& z, simd::float32_8& out) {
		x *= m_frequency;
		y *= m_frequency;
		z *= m_frequency;
		simd::float32_8 x_floor = simd::floor(x);
		simd::float32_8 y_floor = simd::floor(y);
		simd::float32_8 z_floor = simd::floor(z);
		simd::int32_8 x_i(x_floor);
		simd::int32_8 y_i(y_floor);
		simd::int32_8 z_i(z_floor);
		simd::int32_8 seed(m_seed);
		simd::float32_8 p0, p1, p2, p3, p4, p5, p6, p7;
		WhiteNoise::Eval(x_i + 0, y_i + 0, z_i + 0, seed, p0);
		WhiteNoise::Eval(x_i + 1, y_i + 0, z_i + 0, seed, p1);
		WhiteNoise::Eval(x_i + 0, y_i + 1, z_i + 0, seed, p2);
		WhiteNoise::Eval(x_i + 1, y_i + 1, z_i + 0, seed, p3);
		WhiteNoise::Eval(x_i + 0, y_i + 0, z_i + 1, seed, p4);
		WhiteNoise::Eval(x_i + 1, y_i + 0, z_i + 1, seed, p5);
		WhiteNoise::Eval(x_i + 0, y_i + 1, z_i + 1, seed, p6);
		WhiteNoise::Eval(x_i + 1, y_i + 1, z_i + 1, seed, p7);
		simd::float32_8 x_interp = Smoothstep(x - x_floor);
		simd::float32_8 y_interp = Smoothstep(y - y_floor);
		simd::float32_8 z_interp = Smoothstep(z - z_floor);
		simd::float32_8 a0 = zm::Lerp(p0, p1, x_interp);
		simd::float32_8 a1 = zm::Lerp(p2, p3, x_interp);
		simd::float32_8 b0 = zm::Lerp(p4, p5, x_interp);
		simd::float32_8 b1 = zm::Lerp(p6, p7, x_interp);
		simd::float32_8 a = zm::Lerp(a0, a1, y_interp);
		simd::float32_8 b = zm::Lerp(b0, b1, y_interp);
		out = zm::Lerp(a, b, z_interp);
	}
#endif

	//========================================================================
	//  SIMD4 Value Noise
	//========================================================================

#if SIMD_FLOAT32_4 == true && SIMD_INT32_4 == true
	void ValueNoise::Eval(simd::float32_4& x, simd::float32_4& out) {
		x *= m_frequency;
		simd::float32_4 x_floor = simd::floor(x);
		simd::int32_4 x_i(x_floor);
		simd::int32_4 seed(m_seed);
		simd::float32_4 p0, p1;
		WhiteNoise::Eval(x_i + 0, seed, p0);
		WhiteNoise::Eval(x_i + 1, seed, p1);
		simd::float32_4 x_interp = Smoothstep(x - x_floor);
		out = Lerp(p0, p1, x_interp);
	}

	void ValueNoise::Eval(simd::float32_4& x, simd::float32_4& y, simd::float32_4& out) {
		x *= m_frequency;
		y *= m_frequency;
		simd::float32_4 x_floor = simd::floor(x);
		simd::float32_4 y_floor = simd::floor(y);
		simd::int32_4 x_i(x_floor);
		simd::int32_4 y_i(y_floor);
		simd::int32_4 seed(m_seed);
		simd::float32_4 p0, p1, p2, p3;
		WhiteNoise::Eval(x_i + 0, y_i + 0, seed, p0);
		WhiteNoise::Eval(x_i + 1, y_i + 0, seed, p1);
		WhiteNoise::Eval(x_i + 0, y_i + 1, seed, p2);
		WhiteNoise::Eval(x_i + 1, y_i + 1, seed, p3);
		simd::float32_4 x_interp = Smoothstep(x - x_floor);
		simd::float32_4 y_interp = Smoothstep(y - y_floor);
		simd::float32_4 a = zm::Lerp(p0, p1, x_interp);
		simd::float32_4 b = zm::Lerp(p2, p3, x_interp);
		out = zm::Lerp(a, b, y_interp);
	}

	void ValueNoise::Eval(simd::float32_4& x, simd::float32_4& y, simd::float32_4& z, simd::float32_4& out) {
		x *= m_frequency;
		y *= m_frequency;
		z *= m_frequency;
		simd::float32_4 x_floor = simd::floor(x);
		simd::float32_4 y_floor = simd::floor(y);
		simd::float32_4 z_floor = simd::floor(z);
		simd::int32_4 x_i(x_floor);
		simd::int32_4 y_i(y_floor);
		simd::int32_4 z_i(z_floor);
		simd::int32_4 seed(m_seed);
		simd::float32_4 p0, p1, p2, p3, p4, p5, p6, p7;
		WhiteNoise::Eval(x_i + 0, y_i + 0, z_i + 0, seed, p0);
		WhiteNoise::Eval(x_i + 1, y_i + 0, z_i + 0, seed, p1);
		WhiteNoise::Eval(x_i + 0, y_i + 1, z_i + 0, seed, p2);
		WhiteNoise::Eval(x_i + 1, y_i + 1, z_i + 0, seed, p3);
		WhiteNoise::Eval(x_i + 0, y_i + 0, z_i + 1, seed, p4);
		WhiteNoise::Eval(x_i + 1, y_i + 0, z_i + 1, seed, p5);
		WhiteNoise::Eval(x_i + 0, y_i + 1, z_i + 1, seed, p6);
		WhiteNoise::Eval(x_i + 1, y_i + 1, z_i + 1, seed, p7);
		simd::float32_4 x_interp = Smoothstep(x - x_floor);
		simd::float32_4 y_interp = Smoothstep(y - y_floor);
		simd::float32_4 z_interp = Smoothstep(z - z_floor);
		simd::float32_4 a0 = zm::Lerp(p0, p1, x_interp);
		simd::float32_4 a1 = zm::Lerp(p2, p3, x_interp);
		simd::float32_4 b0 = zm::Lerp(p4, p5, x_interp);
		simd::float32_4 b1 = zm::Lerp(p6, p7, x_interp);
		simd::float32_4 a = zm::Lerp(a0, a1, y_interp);
		simd::float32_4 b = zm::Lerp(b0, b1, y_interp);
		out = zm::Lerp(a, b, z_interp);
	}
#endif
}