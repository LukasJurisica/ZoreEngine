#include "math/CellNoise.hpp"
#include "math/WhiteNoise.hpp"
#include "math/MathUtils.hpp"
#include "debug/Debug.hpp"

namespace zm {

	CellNoise::CellNoise() : frequency(0.1f), seed(123456) {
		SetCentralBias(0.f);
	}

	CellNoise::CellNoise(float frequency, float centralBias, int seed) :
		frequency(frequency), seed(seed) {
		SetCentralBias(centralBias);
	}

	void CellNoise::Eval(const glm::vec2& p, float centralBias, CellData& out, int seed) {
		glm::vec2 i = glm::round(p); // Integer Position
		glm::vec2 f = p - i - (centralBias * 0.5f); // Fractional Position

		out.dist = 8.0;
		for (int x = -1; x < 1; x++) {
			for (int y = -1; y < 1; y++) {
				glm::vec2 l = glm::vec2(x, y);
				glm::vec2 v = zm::WhiteNoise::Eval2(i + l);
				v = glm::vec2(zm::Clamp(v.x, 0.25f, 0.75f), zm::Clamp(v.y, 0.25f, 0.75f)) * (1.f - centralBias);
				glm::vec2 r = l + v - f;
				float d = glm::dot(r, r);

				if (d < out.dist)
					out = { d, v, i + l };
			}
		}
	}

	void CellNoise::GetNoise(float x, float y, CellData& out) {
		glm::vec2 p = glm::vec2(x, y) * frequency;
		glm::vec2 c = glm::round(p); // Cell Position
		glm::vec2 f = p - c - offset; // Fractional Position
		glm::ivec2 i = c; // Integer Position

		out.dist = 8.f;
		for (int x = -1; x < 1; x++) {
			for (int y = -1; y < 1; y++) {
				glm::vec2 l = glm::vec2(x, y);
				glm::vec2 v;
				zm::WhiteNoise::GetMultiNoise(i.x + x, i.y + y, seed, &v.x, 2);
				v = glm::clamp(v * mult, low, high);
				glm::vec2 r = l + v - f;
				float d = glm::dot(r, r);

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