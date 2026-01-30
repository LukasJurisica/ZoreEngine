#pragma once

#include "zore/math/sdf.hpp"
#include "zore/math/math.hpp"
#include "zore/math/vector.hpp"

namespace zm {

	//========================================================================
	//	2D Signed Distance Functions
	//========================================================================

	float SDF::Circle(float x, float y, float r) {
		return zm::vec2(x, y).Length() - r;
	}

	float SDF::Square(float x, float y, float r) {
		x = zm::Abs(x) - r;
		y = zm::Abs(y) - r;
		return zm::vec2(zm::Max(x, 0.f), zm::Max(y, 0.f)).Length() + zm::Min(zm::Max(x, y), 0.f);
	}

	float SDF::Triangle(float x, float y, float r) {
		x = zm::Abs(x) - r;
		y += (r / SQRT3) + (r * SQRT3) / 6.f;
		if (x + (SQRT3 * y) > 0) {
			float tx = (x - (SQRT3 * y)) * 0.5f;
			y = ((-SQRT3 * x) - y) * 0.5f;
			x = tx;
		}
		x -= zm::Clamp(x, -2.f * r, 0.f);
		return -zm::vec2(x, y).Length() * zm::Sign(y);
	}

	float SDF::Pentagon(float x, float y, float r) {
		zm::vec2 p = { x, -y };
		const zm::vec3 k = zm::vec3(0.809016994, 0.587785252, 0.726542528);
		p.x = Abs(p.x);
		p -= zm::vec2(-k.x, k.y) * zm::Min(zm::Dot(zm::vec2(-k.x, k.y), p), 0.f) * 2.f;
		p -= zm::vec2( k.x, k.y) * zm::Min(zm::Dot(zm::vec2( k.x, k.y), p), 0.f) * 2.f;
		p -= zm::vec2(zm::Clamp(p.x, -r * k.z, r * k.z), r);
		return p.Length() * zm::Sign(p.y);
	}

	float SDF::Star5(float x, float y, float r) {
		float rf = 0.5f;
		zm::vec2 p = zm::vec2(x, y + 0.1f);
		const zm::vec2 k1 = zm::vec2(0.809016994375, -0.587785252292);
		const zm::vec2 k2 = zm::vec2(-k1.x, k1.y);
		p.x = zm::Abs(p.x);
		p -= k1 * zm::Max(zm::Dot(k1, p), 0.f) * 2.f;
		p -= k2 * zm::Max(zm::Dot(k2, p), 0.f) * 2.f;
		p.x = zm::Abs(p.x);
		p.y -= r;
		zm::vec2 ba = (zm::vec2(-k1.y, k1.x) * rf) - zm::vec2(0, 1);
		float h = zm::Clamp(zm::Dot(p, ba) / zm::Dot(ba, ba), 0.f, r);
		return zm::Length(p - ba * h) * zm::Sign(p.y * ba.x - p.x * ba.y);
	}

	//========================================================================
	//	3D Signed Distance Functions
	//========================================================================

	float SDF::Sphere(float x, float y, float z, float r) {
		return zm::Length(zm::vec3(x, y, z)) - r;
	}

	float SDF::Cube(float x, float y, float z, float r) {
		x = zm::Abs(x) - r;
		y = zm::Abs(y) - r;
		z = zm::Abs(z) - r;
		return zm::Length(zm::vec3(zm::Max(x, 0.f), zm::Max(y, 0.f), zm::Max(z, 0.f))) + zm::Min(zm::Max(x, zm::Max(y, z)), 0.f) - r;
	}
}