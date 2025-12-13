#pragma once

#include <cmath>
#include "zore/platform.hpp"
#include "zore/utils/sized_integer.hpp"
#include "zore/utils/concepts.hpp"
#include "zore/math/vector/vec3.hpp"
#include "zore/math/vector/vec4.hpp"

namespace zm {

    //============================================================
    //  4x4 Matrix (float only)
    //============================================================
    struct alignas(16) mat4 {
    public:
        vec4 rows[4];

    public:
        // Constructors ------------------------------------------
        ALWAYS_INLINE mat4() : rows{ vec4(0), vec4(0), vec4(0), vec4(0) } {}

        ALWAYS_INLINE explicit mat4(float diag) : rows{
            vec4(diag, 0.f, 0.f, 0.f),
            vec4(0.f, diag, 0.f, 0.f),
            vec4(0.f, 0.f, diag, 0.f),
            vec4(0.f, 0.f, 0.f, diag)
        } {
        }

        ALWAYS_INLINE mat4(
            const vec4& r0,
            const vec4& r1,
            const vec4& r2,
            const vec4& r3) : rows{ r0, r1, r2, r3 } {
        }

        // Indexing ----------------------------------------------
        ALWAYS_INLINE vec4& operator[](int i) { return rows[i]; }
        ALWAYS_INLINE const vec4& operator[](int i) const { return rows[i]; }

        // Comparison ---------------------------------------------
        ALWAYS_INLINE bool operator==(const mat4& m) const {
            return rows[0] == m.rows[0] && rows[1] == m.rows[1] &&
                rows[2] == m.rows[2] && rows[3] == m.rows[3];
        }
        ALWAYS_INLINE bool operator!=(const mat4& m) const { return !(*this == m); }

        // Arithmetic ---------------------------------------------
        ALWAYS_INLINE mat4 operator+(const mat4& m) const {
            return mat4(rows[0] + m.rows[0], rows[1] + m.rows[1], rows[2] + m.rows[2], rows[3] + m.rows[3]);
        }
        ALWAYS_INLINE mat4& operator+=(const mat4& m) {
            rows[0] += m.rows[0]; rows[1] += m.rows[1]; rows[2] += m.rows[2]; rows[3] += m.rows[3];
            return *this;
        }

        ALWAYS_INLINE mat4 operator-(const mat4& m) const {
            return mat4(rows[0] - m.rows[0], rows[1] - m.rows[1], rows[2] - m.rows[2], rows[3] - m.rows[3]);
        }
        ALWAYS_INLINE mat4& operator-=(const mat4& m) {
            rows[0] -= m.rows[0]; rows[1] -= m.rows[1]; rows[2] -= m.rows[2]; rows[3] -= m.rows[3];
            return *this;
        }

        ALWAYS_INLINE mat4 operator*(float s) const {
            return mat4(rows[0] * vec4(s), rows[1] * vec4(s), rows[2] * vec4(s), rows[3] * vec4(s));
        }

        ALWAYS_INLINE mat4& operator*=(float s) {
            rows[0] *= vec4(s); rows[1] *= vec4(s); rows[2] *= vec4(s); rows[3] *= vec4(s);
            return *this;
        }

        ALWAYS_INLINE vec4 operator*(const vec4& v) const {
            return vec4(
                rows[0].Dot(v),
                rows[1].Dot(v),
                rows[2].Dot(v),
                rows[3].Dot(v)
            );
        }

        ALWAYS_INLINE mat4 operator*(const mat4& m) const {
            mat4 result;
            mat4 t = m.Transposed();

            for (int r = 0; r < 4; ++r) {
                result[r] = vec4(
                    rows[r].Dot(t[0]),
                    rows[r].Dot(t[1]),
                    rows[r].Dot(t[2]),
                    rows[r].Dot(t[3])
                );
            }
            return result;
        }

        ALWAYS_INLINE mat4& operator*=(const mat4& m) {
            *this = (*this) * m;
            return *this;
        }

        // Utility --------------------------------------------------
        ALWAYS_INLINE mat4 Transposed() const {
            return mat4(
                vec4(rows[0].x, rows[1].x, rows[2].x, rows[3].x),
                vec4(rows[0].y, rows[1].y, rows[2].y, rows[3].y),
                vec4(rows[0].z, rows[1].z, rows[2].z, rows[3].z),
                vec4(rows[0].w, rows[1].w, rows[2].w, rows[3].w)
            );
        }

        static ALWAYS_INLINE mat4 Identity() { return mat4(1.0f); }
        // ---------------------------------------------------------
        // Factory: LookAt
        // ---------------------------------------------------------
        static ALWAYS_INLINE mat4 LookAt(const vec3& eye, const vec3& target, const vec3& up) {
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

        // ---------------------------------------------------------
        // Factory: Perspective Projection
        // ---------------------------------------------------------
        static ALWAYS_INLINE mat4 Perspective(float fov, float aspect, float nearZ, float farZ) {
            float f = 1.0f / std::tanf(fov * 0.5f);
            float nf = 1.0f / (nearZ - farZ);

            return mat4(
                vec4(f / aspect, 0.f, 0.f, 0.f),
                vec4(0.f, f, 0.f, 0.f),
                vec4(0.f, 0.f, (farZ + nearZ) * nf, -1.0f),
                vec4(0.f, 0.f, (2.0f * farZ * nearZ) * nf, 0.f)
            );
        }

        // ---------------------------------------------------------
        // Factory: Orthographic Projection
        // ---------------------------------------------------------
        static ALWAYS_INLINE mat4 Orthographic(float left, float right, float bottom, float top, float nearZ, float farZ) {
            float rl = right - left;
            float tb = top - bottom;
            float fn = farZ - nearZ;

            return mat4(
                vec4(2.0f / rl, 0.f, 0.f, 0.f),
                vec4(0.f, 2.0f / tb, 0.f, 0.f),
                vec4(0.f, 0.f, -2.0f / fn, 0.f),
                vec4(-(right + left) / rl, -(top + bottom) / tb, -(farZ + nearZ) / fn, 1.f)
            );
        }
    };

    // Static Helper Functions --------------------------------------
    ALWAYS_INLINE mat4 Transpose(const mat4& m) { return m.Transposed(); }

} // namespace zm
