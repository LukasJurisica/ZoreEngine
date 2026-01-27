#include "zore/math/matrix/mat4.hpp"

namespace zm {

	bool mat4::Invert() {
		const float
			m00 = rows[0].x, m01 = rows[0].y, m02 = rows[0].z, m03 = rows[0].w,
			m10 = rows[1].x, m11 = rows[1].y, m12 = rows[1].z, m13 = rows[1].w,
			m20 = rows[2].x, m21 = rows[2].y, m22 = rows[2].z, m23 = rows[2].w,
			m30 = rows[3].x, m31 = rows[3].y, m32 = rows[3].z, m33 = rows[3].w;

		mat4 result;
		result[0].x = ( m11 * m22 * m33) - (m11 * m23 * m32) - (m21 * m12 * m33) + (m21 * m13 * m32) + (m31 * m12 * m23) - (m31 * m13 * m22);
		result[0].y = (-m01 * m22 * m33) + (m01 * m23 * m32) + (m21 * m02 * m33) - (m21 * m03 * m32) - (m31 * m02 * m23) + (m31 * m03 * m22);
		result[0].z = ( m01 * m12 * m33) - (m01 * m13 * m32) - (m11 * m02 * m33) + (m11 * m03 * m32) + (m31 * m02 * m13) - (m31 * m03 * m12);
		result[0].w = (-m01 * m12 * m23) + (m01 * m13 * m22) + (m11 * m02 * m23) - (m11 * m03 * m22) - (m21 * m02 * m13) + (m21 * m03 * m12);
		result[1].x = (-m10 * m22 * m33) + (m10 * m23 * m32) + (m20 * m12 * m33) - (m20 * m13 * m32) - (m30 * m12 * m23) + (m30 * m13 * m22);
		result[1].y = ( m00 * m22 * m33) - (m00 * m23 * m32) - (m20 * m02 * m33) + (m20 * m03 * m32) + (m30 * m02 * m23) - (m30 * m03 * m22);
		result[1].z = (-m00 * m12 * m33) + (m00 * m13 * m32) + (m10 * m02 * m33) - (m10 * m03 * m32) - (m30 * m02 * m13) + (m30 * m03 * m12);
		result[1].w = ( m00 * m12 * m23) - (m00 * m13 * m22) - (m10 * m02 * m23) + (m10 * m03 * m22) + (m20 * m02 * m13) - (m20 * m03 * m12);
		result[2].x = ( m10 * m21 * m33) - (m10 * m23 * m31) - (m20 * m11 * m33) + (m20 * m13 * m31) + (m30 * m11 * m23) - (m30 * m13 * m21);
		result[2].y = (-m00 * m21 * m33) + (m00 * m23 * m31) + (m20 * m01 * m33) - (m20 * m03 * m31) - (m30 * m01 * m23) + (m30 * m03 * m21);
		result[2].z = ( m00 * m11 * m33) - (m00 * m13 * m31) - (m10 * m01 * m33) + (m10 * m03 * m31) + (m30 * m01 * m13) - (m30 * m03 * m11);
		result[2].w = (-m00 * m11 * m23) + (m00 * m13 * m21) + (m10 * m01 * m23) - (m10 * m03 * m21) - (m20 * m01 * m13) + (m20 * m03 * m11);
		result[3].x = (-m10 * m21 * m32) + (m10 * m22 * m31) + (m20 * m11 * m32) - (m20 * m12 * m31) - (m30 * m11 * m22) + (m30 * m12 * m21);
		result[3].y = ( m00 * m21 * m32) - (m00 * m22 * m31) - (m20 * m01 * m32) + (m20 * m02 * m31) + (m30 * m01 * m22) - (m30 * m02 * m21);
		result[3].z = (-m00 * m11 * m32) + (m00 * m12 * m31) + (m10 * m01 * m32) - (m10 * m02 * m31) - (m30 * m01 * m12) + (m30 * m02 * m11);
		result[3].w = ( m00 * m11 * m22) - (m00 * m12 * m21) - (m10 * m01 * m22) + (m10 * m02 * m21) + (m20 * m01 * m12) - (m20 * m02 * m11);

		float det = (m00 * result[0].x) + (m01 * result[1].x) + (m02 * result[2].x) + (m03 * result[3].x);
		if (!std::isfinite(det) || std::abs(det) < 1e-6f)
			return false;
		*this = result * (1.f / det);
		return true;
	}

	mat4 LookAt(const vec3& eye, const vec3& target, const vec3& up) {
		vec3 f = Normalize(target - eye);
		vec3 s = Normalize(Cross(f, up));
		vec3 u = Cross(s, f);

		return mat4(
			vec4(s.x, u.x, -f.x, 0.f),
			vec4(s.y, u.y, -f.y, 0.f),
			vec4(s.z, u.z, -f.z, 0.f),
			vec4(-Dot(s, eye), -Dot(u, eye), Dot(f, eye), 1.f)
		);
	}

	mat4 Perspective(float fov, float aspect, float nearZ, float farZ) {
		float f = 1.0f / std::tanf(fov * 0.5f);
		float nf = 1.0f / (nearZ - farZ);

		return mat4(
			vec4(f / aspect, 0.f, 0.f, 0.f),
			vec4(0.f, f, 0.f, 0.f),
			vec4(0.f, 0.f, (farZ + nearZ) * nf, -1.0f),
			vec4(0.f, 0.f, (2.0f * farZ * nearZ) * nf, 0.f)
		);
	}

	mat4 Orthographic(float left, float right, float bottom, float top, float near, float far) {
		float rl = right - left;
		float tb = top - bottom;
		float fn = far - near;

		return mat4(
			vec4(2.f / rl, 0.f, 0.f, 0.f),
			vec4(0.f, 2.f / tb, 0.f, 0.f),
			vec4(0.f, 0.f, -2.f / fn, 0.f),
			vec4(-(right + left) / rl, -(top + bottom) / tb, -(far + near) / fn, 1.f)
		);
	}
}