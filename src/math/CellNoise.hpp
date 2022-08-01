#pragma once
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>

namespace zm {

	struct CellData {
		float dist;
		glm::vec2 offset;
		glm::vec2 cell;
	};

	class CellNoise {
	public:
		CellNoise(float frequency, float centralBias, int seed);
		~CellNoise() = default;

		static void Eval(const glm::vec2& p, float centralBias, CellData& out, int seed = 0);
		void GetNoise(float x, float y, CellData& out);

		void SetSeed(int value) { seed = value; };
		void SetFrequency(float value) { frequency = value; };
		void SetCentralBias(float value);

	private:
		int seed;
		float frequency;
		float centralBias;
	};
}