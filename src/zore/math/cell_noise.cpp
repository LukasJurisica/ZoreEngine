#include "zore/math/cell_noise.hpp"
#include "zore/math/white_noise.hpp"
#include "zore/debug.hpp"

namespace zm {

	CellNoise::CellNoise() : frequency(0.1f), seed(123456) {
		SetCentralBias(0.f);
	}

	CellNoise::CellNoise(float frequency, float centralBias, int seed) :
		frequency(frequency), seed(seed) {
		SetCentralBias(centralBias);
	}

	void CellNoise::Eval(const zm::vec2& p, float centralBias, CellData& out, int seed) {
		zm::vec2 i = zm::Round(p); // Integer Position
		zm::vec2 f = p - i - (centralBias * 0.5f); // Fractional Position

		out.dist = 8.0;
		for (int x = -1; x < 1; x++) {
			for (int y = -1; y < 1; y++) {
				zm::vec2 l = zm::vec2(x, y);
				zm::vec2 v = zm::WhiteNoise::Eval2(i + l);
				v = zm::vec2(zm::Clamp(v.x, 0.25f, 0.75f), zm::Clamp(v.y, 0.25f, 0.75f)) * (1.f - centralBias);
				zm::vec2 r = l + v - f;
				float d = zm::Dot(r, r);

				if (d < out.dist)
					out = { d, v, i + l };
			}
		}
	}

	void CellNoise::GetNoise(float x, float y, CellData& out) {
		zm::vec2 p = zm::vec2(x, y) * frequency;
		zm::vec2 c = zm::Round(p); // Cell Position
		zm::vec2 f = p - c - offset; // Fractional Position
		zm::ivec2 i = c; // Integer Position

		out.dist = 8.f;
		for (int x = -1; x < 1; x++) {
			for (int y = -1; y < 1; y++) {
				zm::vec2 l = zm::vec2(x, y);
				zm::vec2 v;
				zm::WhiteNoise::GetMultiNoise(i.x + x, i.y + y, seed, &v.x, 2);
				v = zm::vec2(zm::Clamp(v.x * mult, low, high), zm::Clamp(v.y * mult, low, high));
				zm::vec2 r = l + v - f;
				float d = zm::Dot(r, r);

				if (d < out.dist)
					out = { d, v, c + l };
			}
		}
	}

	void CellNoise::SetCentralBias(float value) {
		value = zm::Clamp(value, 0.f, 1.f);
		mult = 1.f - value;
		offset = value * 0.5f;
		low = 0.25f - offset;
		high = 0.75f - offset;
	}
}