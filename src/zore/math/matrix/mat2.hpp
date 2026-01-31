#pragma once

#include "zore/math/vector/vec2.hpp"

namespace zm {

	//============================================================
	//  2x2 Matrix
	//============================================================

	struct alignas(16) mat2 {
	public:
		// Constructors -------------------
		ALWAYS_INLINE mat2() : rows{ vec2(0.f), vec2(0.f) } {}
		template <zore::numeric U>
		ALWAYS_INLINE explicit mat2(U s) : rows{
			vec2(static_cast<float>(s), 0.f),
			vec2(0.f, static_cast<float>(s))
		} {}
		ALWAYS_INLINE mat2(const vec2& r0, const vec2& r1) : rows{ r0, r1 } {}
		template <zore::numeric X1, zore::numeric Y1, zore::numeric X2, zore::numeric Y2>
		ALWAYS_INLINE mat2(X1 x1, Y1 y1, X2 x2, Y2 y2) : rows{{ x1, y1 }, { x2, y2 }} {}
		// Comparison ---------------------
		ALWAYS_INLINE bool  operator==(const mat2& m) const { return rows[0] == m.rows[0] && rows[1] == m.rows[1]; }
		ALWAYS_INLINE bool  operator!=(const mat2& m) const { return rows[0] != m.rows[0] || rows[1] != m.rows[1]; }
		// Arithmetic ---------------------
		ALWAYS_INLINE mat2  operator+ (const mat2& m) const { return mat2(rows[0] + m.rows[0], rows[1] + m.rows[1]); }
		ALWAYS_INLINE mat2& operator+=(const mat2& m) { rows[0] += m.rows[0]; rows[1] += m.rows[1]; return *this; }
		ALWAYS_INLINE mat2  operator- (const mat2& m) const { return mat2(rows[0] - m.rows[0], rows[1] - m.rows[1]); }
		ALWAYS_INLINE mat2& operator-=(const mat2& m) { rows[0] -= m.rows[0]; rows[1] -= m.rows[1]; return *this; }
		template <zore::numeric U>
		ALWAYS_INLINE mat2  operator* (U s) const { return mat2(rows[0] * s, rows[1] * s); }
		template <zore::numeric U>
		ALWAYS_INLINE mat2& operator*=(U s) { rows[0] *= s; rows[1] *= s; return *this; }
		ALWAYS_INLINE vec2  operator* (const vec2& v) const { return vec2(rows[0].Dot(v), rows[1].Dot(v)); }
		ALWAYS_INLINE mat2  operator* (const mat2& m) const {
			mat2 result;
			mat2 t = m.Transposed();
			for (int r = 0; r < 2; ++r)
				result[r] = vec2(rows[r].Dot(t[0]), rows[r].Dot(t[1]));
			return result;
		}
		ALWAYS_INLINE mat2& operator*=(const mat2& m) { *this = (*this) * m; return *this; }
		// Factories ----------------------
		static ALWAYS_INLINE mat2 Identity() { return mat2(1.f); }
		// Other --------------------------
		ALWAYS_INLINE vec2& operator[](int i) { return rows[i]; }
		ALWAYS_INLINE const vec2& operator[](int i) const { return rows[i]; }
		ALWAYS_INLINE void Transpose() {
				vec2 r0 = rows[0], r1 = rows[1];
				rows[0] = vec2(r0.x, r1.x);
				rows[1] = vec2(r0.y, r1.y);
		}
		ALWAYS_INLINE mat2 Transposed() const {
			mat2 result = *this;
			result.Transpose();
			return result;
		}

	public:
		// Data ---------------------------
		vec2 rows[2];
	};

	//========================================================================
	// Static Operators
	//========================================================================

	ALWAYS_INLINE mat2 Transpose(const mat2& m) { return m.Transposed(); }
}
