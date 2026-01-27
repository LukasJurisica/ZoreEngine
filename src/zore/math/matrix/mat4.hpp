#pragma once

#include "zore/math/vector/vec3.hpp"
#include "zore/math/vector/vec4.hpp"

namespace zm {

	//============================================================
	//  4x4 Matrix
	//============================================================

	struct alignas(16) mat4 {
	public:
		// Constructors -------------------
		ALWAYS_INLINE mat4() : rows{ vec4(0.f), vec4(0.f), vec4(0.f), vec4(0.f) } {}
		template <zore::numeric U>
		ALWAYS_INLINE explicit mat4(U s) : rows{
			vec4(static_cast<float>(s), 0.f, 0.f, 0.f),
			vec4(0.f, static_cast<float>(s), 0.f, 0.f),
			vec4(0.f, 0.f, static_cast<float>(s), 0.f),
			vec4(0.f, 0.f, 0.f, static_cast<float>(s))
		} {}
		ALWAYS_INLINE mat4(const vec4& r0, const vec4& r1, const vec4& r2, const vec4& r3) : rows{ r0, r1, r2, r3 } {}
		// Comparison ---------------------
		ALWAYS_INLINE bool  operator==(const mat4& m) const { return rows[0] == m.rows[0] && rows[1] == m.rows[1] && rows[2] == m.rows[2] && rows[3] == m.rows[3]; }
		ALWAYS_INLINE bool  operator!=(const mat4& m) const { return rows[0] != m.rows[0] || rows[1] != m.rows[1] || rows[2] != m.rows[2] || rows[3] != m.rows[3]; }
		// Arithmetic ---------------------
		ALWAYS_INLINE mat4  operator+ (const mat4& m) const { return mat4(rows[0] + m.rows[0], rows[1] + m.rows[1], rows[2] + m.rows[2], rows[3] + m.rows[3]); }
		ALWAYS_INLINE mat4& operator+=(const mat4& m) { rows[0] += m.rows[0]; rows[1] += m.rows[1]; rows[2] += m.rows[2]; rows[3] += m.rows[3]; return *this; }
		ALWAYS_INLINE mat4  operator- (const mat4& m) const { return mat4(rows[0] - m.rows[0], rows[1] - m.rows[1], rows[2] - m.rows[2], rows[3] - m.rows[3]); }
		ALWAYS_INLINE mat4& operator-=(const mat4& m) { rows[0] -= m.rows[0]; rows[1] -= m.rows[1]; rows[2] -= m.rows[2]; rows[3] -= m.rows[3]; return *this; }
		template <zore::numeric U>
		ALWAYS_INLINE mat4  operator* (U s) const { return mat4(rows[0] * s, rows[1] * s, rows[2] * s, rows[3] * s); }
		template <zore::numeric U>
		ALWAYS_INLINE mat4& operator*=(U s) { rows[0] *= s; rows[1] *= s; rows[2] *= s; rows[3] *= s; return *this; }
		ALWAYS_INLINE vec4  operator* (const vec4& v) const { return vec4(rows[0].Dot(v), rows[1].Dot(v), rows[2].Dot(v), rows[3].Dot(v)); }
		ALWAYS_INLINE mat4  operator* (const mat4& m) const {
			mat4 result;
			mat4 t = m.Transposed();
			for (int r = 0; r < 4; ++r)
				result[r] = vec4(rows[r].Dot(t[0]), rows[r].Dot(t[1]), rows[r].Dot(t[2]), rows[r].Dot(t[3]));
			return result;
		}
		ALWAYS_INLINE mat4& operator*=(const mat4& m) { *this = (*this) * m; return *this; }
		// Factories ----------------------
		static ALWAYS_INLINE mat4 Identity() { return mat4(1.f); }
		
		// Other --------------------------
		ALWAYS_INLINE vec4& operator[](int i) { return rows[i]; }
		ALWAYS_INLINE const vec4& operator[](int i) const { return rows[i]; }
		bool Invert();
		ALWAYS_INLINE void Transpose() {
			if constexpr (simd_enabled<float, 4>)
				transpose(rows[0].v, rows[1].v, rows[2].v, rows[3].v);
			else {
				vec4 r0 = rows[0], r1 = rows[1], r2 = rows[2], r3 = rows[3];
				rows[0] = vec4(r0.x, r1.x, r2.x, r3.x);
				rows[1] = vec4(r0.y, r1.y, r2.y, r3.y);
				rows[2] = vec4(r0.z, r1.z, r2.z, r3.z);
				rows[3] = vec4(r0.w, r1.w, r2.w, r3.w);
			}
		}
		ALWAYS_INLINE mat4 Transposed() const {
			mat4 result = *this;
			result.Transpose();
			return result;
		}

	public:
		// Data ---------------------------
		vec4 rows[4];
	};

	//========================================================================
	// Static Operators
	//========================================================================

	ALWAYS_INLINE mat4 Transpose(const mat4& m) { return m.Transposed(); }
	ALWAYS_INLINE mat4 Inverse(const mat4& m) { mat4 result = m; result.Invert(); return result; }
	mat4 LookAt(const vec3& eye, const vec3& target, const vec3& up);
	mat4 Perspective(float fov, float aspect, float nearZ, float farZ);
	mat4 Orthographic(float left, float right, float bottom, float top, float near, float far);
}