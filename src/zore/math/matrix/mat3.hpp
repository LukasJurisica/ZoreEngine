#pragma once

#include "zore/math/vector/vec3.hpp"

namespace zm {

	//============================================================
	//  3x3 Matrix
	//============================================================

	struct alignas(16) mat3 {
	public:
		// Constructors -------------------
		ALWAYS_INLINE mat3() : rows{ vec3(0.f), vec3(0.f), vec3(0.f) } {}
		template <zore::numeric U>
		ALWAYS_INLINE explicit mat3(U s) : rows{
			vec3(static_cast<float>(s), 0.f, 0.f),
			vec3(0.f, static_cast<float>(s), 0.f),
			vec3(0.f, 0.f, static_cast<float>(s))
		} {}
		ALWAYS_INLINE mat3(const vec3& r0, const vec3& r1, const vec3& r2) : rows{ r0, r1, r2 } {}
		// Comparison ---------------------
		ALWAYS_INLINE bool  operator==(const mat3& m) const { return rows[0] == m.rows[0] && rows[1] == m.rows[1] && rows[2] == m.rows[2]; }
		ALWAYS_INLINE bool  operator!=(const mat3& m) const { return rows[0] != m.rows[0] || rows[1] != m.rows[1] || rows[2] != m.rows[2]; }
		// Arithmetic ---------------------
		ALWAYS_INLINE mat3  operator+ (const mat3& m) const { return mat3(rows[0] + m.rows[0], rows[1] + m.rows[1], rows[2] + m.rows[2]); }
		ALWAYS_INLINE mat3& operator+=(const mat3& m) { rows[0] += m.rows[0]; rows[1] += m.rows[1]; rows[2] += m.rows[2]; return *this; }
		ALWAYS_INLINE mat3  operator- (const mat3& m) const { return mat3(rows[0] - m.rows[0], rows[1] - m.rows[1], rows[2] - m.rows[2]); }
		ALWAYS_INLINE mat3& operator-=(const mat3& m) { rows[0] -= m.rows[0]; rows[1] -= m.rows[1]; rows[2] -= m.rows[2]; return *this; }
		template <zore::numeric U>
		ALWAYS_INLINE mat3  operator* (U s) const { return mat3(rows[0] * s, rows[1] * s, rows[2] * s); }
		template <zore::numeric U>
		ALWAYS_INLINE mat3& operator*=(U s) { rows[0] *= s; rows[1] *= s; rows[2] *= s; return *this; }
		ALWAYS_INLINE vec3  operator* (const vec3& v) const { return vec3(rows[0].Dot(v), rows[1].Dot(v), rows[2].Dot(v)); }
		ALWAYS_INLINE mat3  operator* (const mat3& m) const {
			mat3 result;
			mat3 t = m.Transposed();
			for (int r = 0; r < 3; ++r)
				result[r] = vec3(rows[r].Dot(t[0]), rows[r].Dot(t[1]), rows[r].Dot(t[2]));
			return result;
		}
		ALWAYS_INLINE mat3& operator*=(const mat3& m) { *this = (*this) * m; return *this; }
		// Factories ----------------------
		static ALWAYS_INLINE mat3 Identity() { return mat3(1.f); }
		// Other --------------------------
		ALWAYS_INLINE vec3& operator[](int i) { return rows[i]; }
		ALWAYS_INLINE const vec3& operator[](int i) const { return rows[i]; }
		ALWAYS_INLINE void Transpose() {
			vec3 r0 = rows[0], r1 = rows[1], r2 = rows[2];
			rows[0] = vec3(r0.x, r1.x, r2.x);
			rows[1] = vec3(r0.y, r1.y, r2.y);
			rows[2] = vec3(r0.z, r1.z, r2.z);
		}
		ALWAYS_INLINE mat3 Transposed() const {
			mat3 result = *this;
			result.Transpose();
			return result;
		}

	public:
		// Data ---------------------------
		vec3 rows[3];
	};

	//========================================================================
	// Static Operators
	//========================================================================

	ALWAYS_INLINE mat3 Transpose(const mat3& m) { return m.Transposed(); }
}
