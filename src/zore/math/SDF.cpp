#pragma once
#include "zore/math/SDF.hpp"
#include "zore/math/MathUtils.hpp"

#include <glm/glm.hpp>

namespace zm {

	//========================================================================
	//	2D Signed Distance Functions
	//========================================================================

	float SDF::Circle(float x, float y, float r) {
		return Length(x, y) - r;
	}

	float SDF::Square(float x, float y, float r) {
		x = abs(x) - r;
		y = abs(y) - r;
		return Length(Max(x, 0.f), Max(y, 0.f)) + Min(Max(x, y), 0.f);
	}

	float SDF::Triangle(float x, float y, float r) {
		x = abs(x) - r;
		y += (r / SQRT3) + (r * SQRT3) / 6.f;
		if (x + (SQRT3 * y) > 0) {
			float tx = (x - (SQRT3 * y)) * 0.5f;
			y = ((-SQRT3 * x) - y) * 0.5f;
			x = tx;
		}
		x -= Clamp(x, -2.f * r, 0.f);
		return -Length(x, y) * Sign(y);
	}

	float SDF::Pentagon(float x, float y, float r) {
		glm::vec2 p = { x, -y };
		const glm::vec3 k = glm::vec3(0.809016994, 0.587785252, 0.726542528);
		p.x = abs(p.x);
		p -= glm::vec2(-k.x, k.y) * glm::min(glm::dot(glm::vec2(-k.x, k.y), p), 0.f) * 2.f;
		p -= glm::vec2( k.x, k.y) * glm::min(glm::dot(glm::vec2( k.x, k.y), p), 0.f) * 2.f;
		p -= glm::vec2(glm::clamp(p.x, -r * k.z, r * k.z), r);
		return glm::length(p) * glm::sign(p.y);
	}

	float SDF::Star5(float x, float y, float r) {
		float rf = 0.5f;
		glm::vec2 p = glm::vec2(x, y + 0.1f);
		const glm::vec2 k1 = glm::vec2(0.809016994375, -0.587785252292);
		const glm::vec2 k2 = glm::vec2(-k1.x, k1.y);
		p.x = abs(p.x);
		p -= k1 * glm::max(dot(k1, p), 0.f) * 2.f;
		p -= k2 * glm::max(dot(k2, p), 0.f) * 2.f;
		p.x = abs(p.x);
		p.y -= r;
		glm::vec2 ba = rf * glm::vec2(-k1.y, k1.x) - glm::vec2(0, 1);
		float h = glm::clamp(dot(p, ba) / dot(ba, ba), 0.f, r);
		return glm::length(p - ba * h) * glm::sign(p.y * ba.x - p.x * ba.y);
	}

	//========================================================================
	//	3D Signed Distance Functions
	//========================================================================

	float SDF::Sphere(float x, float y, float z, float r) {
		return Length(x, y, z) - r;
	}

	float SDF::Cube(float x, float y, float z, float r) {
		x = abs(x) - r;
		y = abs(y) - r;
		z = abs(z) - r;
		return Length(Max(x, 0.f), Max(y, 0.f), Max(z, 0.f)) + Min(Max(x, Max(y, z)), 0.f) - r;
	}
}