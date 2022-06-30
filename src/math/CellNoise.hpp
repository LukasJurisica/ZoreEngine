#pragma once
#include "math/WhiteNoise.hpp"
#include "math/MathUtils.hpp"
#include "debug/Debug.hpp"

#include <glm/vec3.hpp>
#include <math.h>
#include <vector>
#include <array>
#include <algorithm>

namespace zm {

	struct CellData {
		float value;
		float dist;
		glm::vec2 pos;
		int id;
		float contrib;

		bool operator < (const CellData& other) const {
			return dist < other.dist;
		}
	};

	class CellNoise {
	public:

		static void Eval(const glm::vec2& p, float centralBias, glm::vec3& out, int seed = 0) {
			DEBUG_ENSURE(centralBias < 0.25, "Central bias for cellular noise must be atleast 0.25 to ensure seed is found within quad cell search.");
			glm::vec2 i = glm::round(p); // Integer Position
			glm::vec2 f = p - i - (centralBias * 0.5f); // Fractional Position

			out.x = 8.0;
			for (int x = -1; x < 1; x++) {
				for (int y = -1; y < 1; y++) {
					glm::vec2 l = glm::vec2(x, y);
					glm::vec2 v = zm::WhiteNoise::Eval2(i + l) * (1.f - centralBias);
					glm::vec2 r = l + v - f;
					float d = glm::dot(r, r);

					if (d < out.x)
						out = glm::vec3(d, i + l);
				}
			}

			out.x = sqrt(out.x);
		}

		static void EvalOld(glm::vec2 p, float centralBias, std::array<CellData, 9>& data, int seed = 0) {
			// Setup Local Variables
			float offset = centralBias * 0.5f;
			float mult = 1.f - centralBias;
			glm::vec2 i = glm::floor(p); // Integer Position
			glm::vec2 f = p - i; // Fractional Position

			int index = 0;
			for (int x = -1; x <= 1; x++) {
				for (int y = -1; y <= 1; y++) {
					glm::vec2 l = { x, y };
					glm::vec3 v = WhiteNoise::Eval3(glm::vec3(l + i, seed));

					data[index] = { v.z, 0.f, { v.x, v.y }, 0, 0.f };
					data[index].pos = (data[index].pos * mult) + l + offset;
					data[index].dist = glm::length(data[index].pos - f);
					index++;
				}
			}
			std::sort(data.begin(), data.end());
		}

		static float ComputeDistanceToEdge(glm::vec2 p, CellData& closest, CellData& other) {
			glm::vec2 cp = (closest.pos + other.pos) / 2.f;

			glm::vec2 v = closest.pos - cp;
			glm::vec2 u = glm::fract(p) - cp;

			return glm::length(v * (glm::dot(u, v) / glm::dot(v, v)));
		}

		static float ComputeDistanceToEdge(glm::vec2 p, std::array<CellData, 9>& data) {
			float min = 8;
			// Only the nearest 4 cells (3 if you do not include the one you are in)
			// can possibly be close enough to have the smallest distance to an edge
			for (int i = 1; i < 4; i++) {
				float dist = zm::CellNoise::ComputeDistanceToEdge(p, data[0], data[i]);
				if (dist < min)
					min = dist;
			}
			return min;
		}

	};
}