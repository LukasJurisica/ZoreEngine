#pragma once
#include "math/MathUtils.hpp"
#include <glm/glm.hpp>

namespace zm {
	
	class WhiteNoise {
	public:

		//========================================================================
		//  Single Float Output
		//========================================================================

		static float Eval1(float p) {
			p = zm::Fract(p * .1031f);
			p *= p + 33.33f;
			p *= p + p;
			return zm::Fract(p);
		}

		static float Eval1(glm::vec2 p) {
			glm::vec3 p3 = glm::fract(glm::vec3(p.x, p.y, p.x) * .1031f);
			p3 += glm::dot(p3, glm::vec3(p3.y, p3.z, p3.x) + 33.33f);
			return glm::fract((p3.x + p3.y) * p3.z);
		}

		static float Eval1(glm::vec3 p3) {
			p3 = glm::fract(p3 * .1031f);
			p3 += glm::dot(p3, glm::vec3(p3.y, p3.z, p3.x) + 31.32f);
			return glm::fract((p3.x + p3.y) * p3.z);
		}
		
		//========================================================================
		//  Vector2 Output
		//========================================================================
		
		static glm::vec2 Eval2(float p) {
			glm::vec3 p3 = glm::fract(glm::vec3(p) * glm::vec3(.1031f, .1030f, .0973f));
			p3 += glm::dot(p3, glm::vec3(p3.y, p3.z, p3.x) + 33.33f);
			return glm::fract((glm::vec2(p3.x) + glm::vec2(p3.y, p3.z)) * glm::vec2(p3.z, p3.y));
		}

		static glm::vec2 Eval2(glm::vec2 p) {
			glm::vec3 p3 = glm::fract(glm::vec3(p.x, p.y, p.x) * glm::vec3(.1031f, .1030f, .0973f));
			p3 += glm::dot(p3, glm::vec3(p3.y, p3.z, p3.x) + 33.33f);
			return glm::fract((glm::vec2(p3.x) + glm::vec2(p3.y, p3.z)) * glm::vec2(p3.z, p3.y));
		}

		static glm::vec2 Eval2(glm::vec3 p3) {
			p3 = glm::fract(p3 * glm::vec3(.1031f, .1030f, .0973f));
			p3 += glm::dot(p3, glm::vec3(p3.y, p3.z, p3.x) + 33.33f);
			return glm::fract((glm::vec2(p3.x) + glm::vec2(p3.y, p3.z)) * glm::vec2(p3.z, p3.y));
		}

		//========================================================================
		//  Vector3 Output
		//========================================================================

		static glm::vec3 Eval3(float p) {
			glm::vec3 p3 = glm::fract(glm::vec3(p) * glm::vec3(.1031f, .1030f, .0973f));
			p3 += glm::dot(p3, glm::vec3(p3.y, p3.z, p3.x) + 33.33f);
			return glm::fract((glm::vec3(p3.x, p3.x, p3.y) + glm::vec3(p3.y, p3.z, p3.z)) * glm::vec3(p3.z, p3.y, p3.x));
		}

		static glm::vec3 Eval3(glm::vec2 p) {
			glm::vec3 p3 = glm::fract(glm::vec3(p.x, p.y, p.x) * glm::vec3(.1031f, .1030f, .0973f));
			p3 += glm::dot(p3, glm::vec3(p3.y, p3.z, p3.x) + 33.33f);
			return glm::fract((glm::vec3(p3.x, p3.x, p3.y) + glm::vec3(p3.y, p3.z, p3.z)) * glm::vec3(p3.z, p3.y, p3.x));
		}

		static glm::vec3 Eval3(glm::vec3 p3) {
			p3 = glm::fract(p3 * glm::vec3(.1031f, .1030f, .0973f));
			p3 += glm::dot(p3, glm::vec3(p3.y, p3.z, p3.x) + 33.33f);
			return glm::fract((glm::vec3(p3.x, p3.x, p3.y) + glm::vec3(p3.y, p3.z, p3.z)) * glm::vec3(p3.z, p3.y, p3.x));
		}

		//========================================================================
		//  Vector4 Output
		//========================================================================

		static glm::vec4 Eval4(float p) {
			glm::vec4 p4 = glm::fract(glm::vec4(p) * glm::vec4(.1031f, .1030f, .0973f, .1099f));
			p4 += glm::dot(p4, glm::vec4(p4.w, p4.z, p4.x, p4.y) + 33.33f);
			return glm::fract((glm::vec4(p4.x, p4.x, p4.y, p4.z) + glm::vec4(p4.y, p4.z, p4.z, p4.w)) * glm::vec4(p4.z, p4.y, p4.w, p4.x));
		}

		static glm::vec4 Eval4(glm::vec2 p) {
			glm::vec4 p4 = glm::fract(glm::vec4(p.x, p.y, p.x, p.y) * glm::vec4(.1031f, .1030f, .0973f, .1099f));
			p4 += glm::dot(p4, glm::vec4(p4.w, p4.z, p4.x, p4.y) + 33.33f);
			return glm::fract((glm::vec4(p4.x, p4.x, p4.y, p4.z) + glm::vec4(p4.y, p4.z, p4.z, p4.w)) * glm::vec4(p4.z, p4.y, p4.w, p4.x));
		}

		static glm::vec4 Eval4(glm::vec3 p) {
			glm::vec4 p4 = glm::fract(glm::vec4(p.x, p.y, p.z, p.x) * glm::vec4(.1031f, .1030f, .0973f, .1099f));
			p4 += glm::dot(p4, glm::vec4(p4.w, p4.z, p4.x, p4.y) + 33.33f);
			return glm::fract((glm::vec4(p4.x, p4.x, p4.y, p4.z) + glm::vec4(p4.y, p4.z, p4.z, p4.w)) * glm::vec4(p4.z, p4.y, p4.w, p4.x));
		}

		static glm::vec4 Eval4(glm::vec4 p4) {
			p4 = glm::fract(p4 * glm::vec4(.1031f, .1030f, .0973f, .1099f));
			p4 += glm::dot(p4, glm::vec4(p4.w, p4.z, p4.x, p4.y) + 33.33f);
			return glm::fract((glm::vec4(p4.x, p4.x, p4.y, p4.z) + glm::vec4(p4.y, p4.z, p4.z, p4.w)) * glm::vec4(p4.z, p4.y, p4.w, p4.x));
		}
	};
}