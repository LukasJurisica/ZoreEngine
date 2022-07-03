#include "math/CellNoise.hpp"
#include "math/WhiteNoise.hpp"
#include "math/MathUtils.hpp"
#include "debug/Debug.hpp"
//
//#include <math.h>
//#include <algorithm>
//
namespace zm {

	CellNoise::CellNoise(float frequency, int seed) : frequency(frequency), centralBias(0.25f), seed(seed) {}

	void CellNoise::Eval(const glm::vec2& p, float centralBias, CellData& out, int seed) {
		glm::vec2 i = glm::round(p); // Integer Position
		glm::vec2 f = p - i - (centralBias * 0.5f); // Fractional Position

		out.dist = 8.0;
		for (int x = -1; x < 1; x++) {
			for (int y = -1; y < 1; y++) {
				glm::vec2 l = glm::vec2(x, y);
				glm::vec2 v = zm::WhiteNoise::Eval2(i + l) * (1.f - centralBias);
				glm::vec2 r = l + v - f;
				float d = glm::dot(r, r);

				if (d < out.dist)
					out = { d, v, i + l };
			}
		}

		out.dist = sqrt(out.dist);
	}

	void CellNoise::GetNoise(int x, int y, CellData& out) {
		glm::vec2 p = glm::vec2(x, y) * frequency;
		glm::vec2 i = glm::round(p); // Integer Position
		glm::vec2 f = p - i - (centralBias * 0.5f); // Fractional Position

		out.dist = 8.0;
		for (int x = -1; x < 1; x++) {
			for (int y = -1; y < 1; y++) {
				glm::vec2 l = glm::vec2(x, y);
				glm::vec2 v = zm::WhiteNoise::Eval2(i + l) * (1.f - centralBias);
				glm::vec2 r = l + v - f;
				float d = glm::dot(r, r);

				if (d < out.dist)
					out = { d, v, i + l };
			}
		}

		out.dist = sqrt(out.dist);
	}

	void CellNoise::SetCentralBias(float value) {
		centralBias = zm::Clamp(value, 0.25f, 1.f);
	}
}