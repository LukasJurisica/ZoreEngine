#pragma once

#ifdef __INTELLISENSE__
#include "zore/math/vector/vec4.hpp"
#endif

#include "zore/math/simd/simd_float32_4.hpp"
#include "zore/math/simd/simd_int32_4.hpp"
#include "zore/math/simd/simd_uint32_4.hpp"

namespace zm {

	//========================================================================
	//  int32 4 SIMD Vector
	//========================================================================

	template<>
	struct alignas(16) vec4_base<int32_t> {
	public:
		// Constructors -------------------
		ALWAYS_INLINE explicit vec4_base() : v() {}
		template <zore::numeric U>
		ALWAYS_INLINE vec4_base(U s) : v(static_cast<int32_t>(s)) {}
		template <zore::numeric U>
		ALWAYS_INLINE explicit vec4_base(U x, U y, U z, U w) : v(static_cast<int32_t>(x), static_cast<int32_t>(y), static_cast<int32_t>(z), static_cast<int32_t>(w)) {}
		template <zore::numeric U>
		ALWAYS_INLINE vec4_base(const vec4_base<U>& o) {
			if constexpr (std::is_same_v<U, uint32_t>)
				v = reinterpret_cast<const simd::uint32_4&>(o).v;
			else if constexpr (std::is_same_v<U, float>)
				v = simd::int32_4(reinterpret_cast<const simd::float32_4&>(o).v);
			else
				v = simd::int32_4(static_cast<int32_t>(o.x), static_cast<int32_t>(o.y), static_cast<int32_t>(o.z), static_cast<int32_t>(o.w));
		}
		ALWAYS_INLINE vec4_base(const simd::int32_4& o) : v(o) {}

	public:
		// Comparison ---------------------
		ALWAYS_INLINE bool operator== (const vec4_base<int32_t>& o) const { return simd::mask(v == o.v) == 0xFFFF; }
		ALWAYS_INLINE bool operator!= (const vec4_base<int32_t>& o) const { return simd::mask(v == o.v) != 0xFFFF; }
		// Arithmetic ---------------------
		ALWAYS_INLINE vec4_base<int32_t>& operator+  () { return *this; }
		ALWAYS_INLINE vec4_base<int32_t>  operator+  (const vec4_base<int32_t>& o) const { return vec4_base<int32_t>(v + o.v); }
		ALWAYS_INLINE vec4_base<int32_t>& operator+= (const vec4_base<int32_t>& o) { v += o.v; return *this; }
		ALWAYS_INLINE vec4_base<int32_t>  operator-  () const { return -v; }
		ALWAYS_INLINE vec4_base<int32_t>  operator-  (const vec4_base<int32_t>& o) const { return vec4_base<int32_t>(v - o.v); }
		ALWAYS_INLINE vec4_base<int32_t>& operator-= (const vec4_base<int32_t>& o) { v -= o.v; return *this; }
		ALWAYS_INLINE vec4_base<int32_t>  operator*  (const vec4_base<int32_t>& o) const { return vec4_base<int32_t>(v * o.v); }
		ALWAYS_INLINE vec4_base<int32_t>& operator*= (const vec4_base<int32_t>& o) { v *= o.v; return *this; }
		ALWAYS_INLINE vec4_base<int32_t>  operator/  (const vec4_base<int32_t>& o) const { return vec4_base<int32_t>(v / o.v); }
		ALWAYS_INLINE vec4_base<int32_t>& operator/= (const vec4_base<int32_t>& o) { v /= o.v; return *this; }
		// Other --------------------------
		ALWAYS_INLINE       int32_t& operator[](int index) { return data[index]; }
		ALWAYS_INLINE const int32_t& operator[](int index) const { return data[index]; }
		ALWAYS_INLINE int32_t Sum() const { return v.hsum(); }
		ALWAYS_INLINE int32_t Dot(const vec4_base<int32_t>& o) const { return v.dot(o.v); }
		ALWAYS_INLINE float Length() const { return std::sqrtf(static_cast<float>(v.dot(v))); }

	public:
		// Data ---------------------------
		union {
			struct { int32_t x, y, z, w; };
			struct { int32_t r, g, b, a; };
			int32_t data[4];
			simd::int32_4 v;
		};
	};

	//========================================================================
	//  uint32 4 SIMD Vector
	//========================================================================

	template<>
	struct alignas(16) vec4_base<uint32_t> {
	public:
		// Constructors -------------------
		ALWAYS_INLINE explicit vec4_base() : v() {}
		template <zore::numeric U>
		ALWAYS_INLINE vec4_base(U s) : v(static_cast<uint32_t>(s)) {}
		template <zore::numeric U>
		ALWAYS_INLINE explicit vec4_base(U x, U y, U z, U w) : v(static_cast<uint32_t>(x), static_cast<uint32_t>(y), static_cast<uint32_t>(z), static_cast<uint32_t>(w)) {}
		template <zore::numeric U>
		ALWAYS_INLINE vec4_base(const vec4_base<U>& o) {
			if constexpr (std::is_same_v<U, int32_t>)
				v = reinterpret_cast<const simd::int32_4&>(o).v;
			else if constexpr (std::is_same_v<U, float>)
				v = simd::uint32_4(reinterpret_cast<const simd::float32_4&>(o).v);
			else
				v = simd::uint32_4(static_cast<uint32_t>(o.x), static_cast<uint32_t>(o.y), static_cast<uint32_t>(o.z), static_cast<uint32_t>(o.w));
		}
		ALWAYS_INLINE vec4_base(const simd::uint32_4& o) : v(o) {}

	public:
		// Comparison ---------------------
		ALWAYS_INLINE bool operator== (const vec4_base<uint32_t>& o) const { return simd::mask(v == o.v) == 0xFFFF; }
		ALWAYS_INLINE bool operator!= (const vec4_base<uint32_t>& o) const { return simd::mask(v == o.v) != 0xFFFF; }
		// Arithmetic ---------------------
		ALWAYS_INLINE vec4_base<uint32_t>& operator+  () { return *this; }
		ALWAYS_INLINE vec4_base<uint32_t>  operator+  (const vec4_base<uint32_t>& o) const { return vec4_base<uint32_t>(v + o.v); }
		ALWAYS_INLINE vec4_base<uint32_t>& operator+= (const vec4_base<uint32_t>& o) { v += o.v; return *this; }
		ALWAYS_INLINE vec4_base<uint32_t>  operator-  () const { return -v; }
		ALWAYS_INLINE vec4_base<uint32_t>  operator-  (const vec4_base<uint32_t>& o) const { return vec4_base<uint32_t>(v - o.v); }
		ALWAYS_INLINE vec4_base<uint32_t>& operator-= (const vec4_base<uint32_t>& o) { v -= o.v; return *this; }
		ALWAYS_INLINE vec4_base<uint32_t>  operator*  (const vec4_base<uint32_t>& o) const { return vec4_base<uint32_t>(v * o.v); }
		ALWAYS_INLINE vec4_base<uint32_t>& operator*= (const vec4_base<uint32_t>& o) { v *= o.v; return *this; }
		ALWAYS_INLINE vec4_base<uint32_t>  operator/  (const vec4_base<uint32_t>& o) const { return vec4_base<uint32_t>(v / o.v); }
		ALWAYS_INLINE vec4_base<uint32_t>& operator/= (const vec4_base<uint32_t>& o) { v /= o.v; return *this; }
		// Other --------------------------
		ALWAYS_INLINE       uint32_t& operator[](int index) { return data[index]; }
		ALWAYS_INLINE const uint32_t& operator[](int index) const { return data[index]; }
		ALWAYS_INLINE uint32_t Sum() const { return v.hsum(); }
		ALWAYS_INLINE uint32_t Dot(const vec4_base<uint32_t>& o) const { return v.dot(o.v); }
		ALWAYS_INLINE float Length() const { return std::sqrtf(static_cast<float>(v.dot(v))); }

	public:
		// Data ---------------------------
		union {
			struct { uint32_t x, y, z, w; };
			struct { uint32_t r, g, b, a; };
			uint32_t data[4];
			simd::uint32_4 v;
		};
	};

	//========================================================================
	//  float32 4 SIMD Vector
	//========================================================================

	template<>
	struct alignas(16) vec4_base<float> {
	public:
		// Constructors -------------------
		ALWAYS_INLINE explicit vec4_base() : v() {}
		template <zore::numeric U>
		ALWAYS_INLINE vec4_base(U s) : v(static_cast<float>(s)) {}
		template <zore::numeric U>
		ALWAYS_INLINE explicit vec4_base(U x, U y, U z, U w) : v(static_cast<float>(x), static_cast<float>(y), static_cast<float>(z), static_cast<float>(w)) {}
		template <zore::numeric U>
		ALWAYS_INLINE vec4_base(const vec4_base<U>& o) {
			if constexpr (std::is_same_v<U, int32_t>)
				v = simd::float32_4(reinterpret_cast<const simd::int32_4&>(o).v);
			else if constexpr (std::is_same_v<U, uint32_t>)
				v = simd::float32_4(reinterpret_cast<const simd::uint32_4&>(o).v);
			else
				v = simd::float32_4(static_cast<float>(o.x), static_cast<float>(o.y), static_cast<float>(o.z), static_cast<float>(o.w));
		}
		ALWAYS_INLINE vec4_base(const simd::float32_4& o) : v(o) {}

	public:
		// Comparison ---------------------
		ALWAYS_INLINE bool operator== (const vec4_base<float>& o) const { return simd::mask(v == o.v) == 0xFFFF; }
		ALWAYS_INLINE bool operator!= (const vec4_base<float>& o) const { return simd::mask(v == o.v) != 0xFFFF; }
		// Arithmetic ---------------------
		ALWAYS_INLINE vec4_base<float>& operator+  () { return *this; }
		ALWAYS_INLINE vec4_base<float>  operator+  (const vec4_base<float>& o) const { return vec4_base<float>(v + o.v); }
		ALWAYS_INLINE vec4_base<float>& operator+= (const vec4_base<float>& o) { v += o.v; return *this; }
		ALWAYS_INLINE vec4_base<float>  operator-  () const { return -v; }
		ALWAYS_INLINE vec4_base<float>  operator-  (const vec4_base<float>& o) const { return vec4_base<float>(v - o.v); }
		ALWAYS_INLINE vec4_base<float>& operator-= (const vec4_base<float>& o) { v -= o.v; return *this; }
		ALWAYS_INLINE vec4_base<float>  operator*  (const vec4_base<float>& o) const { return vec4_base<float>(v * o.v); }
		ALWAYS_INLINE vec4_base<float>& operator*= (const vec4_base<float>& o) { v *= o.v; return *this; }
		ALWAYS_INLINE vec4_base<float>  operator/  (const vec4_base<float>& o) const { return vec4_base<float>(v / o.v); }
		ALWAYS_INLINE vec4_base<float>& operator/= (const vec4_base<float>& o) { v /= o.v; return *this; }
		// Other --------------------------
		ALWAYS_INLINE       float& operator[](int index) { return data[index]; }
		ALWAYS_INLINE const float& operator[](int index) const { return data[index]; }
		ALWAYS_INLINE float Sum() const { return v.hsum(); }
		ALWAYS_INLINE float Dot(const vec4_base<float>& o) const { return v.dot(o.v); }
		ALWAYS_INLINE float Length() const { return std::sqrtf(v.dot(v)); }
		ALWAYS_INLINE vec4_base<float>& Normalize() { v /= simd::float32_4(Length()); return *this; }

	public:
		// Data ---------------------------
		union {
			struct { float x, y, z, w; };
			struct { float r, g, b, a; };
			float data[4];
			simd::float32_4 v;
		};
	};

	template <>
	ALWAYS_INLINE vec4_base<float> Trunc(const vec4_base<float>& a) { return vec4_base<float>(simd::trunc(a.v)); }
	template <>
	ALWAYS_INLINE vec4_base<float> Floor(const vec4_base<float>& a) { return vec4_base<float>(simd::floor(a.v)); }
	template <>
	ALWAYS_INLINE vec4_base<float> Ceil(const vec4_base<float>& a) { return vec4_base<float>(simd::ceil(a.v)); }
	template <>
	ALWAYS_INLINE vec4_base<float> Round(const vec4_base<float>& a) { return vec4_base<float>(simd::round(a.v)); }
	template <>
	ALWAYS_INLINE vec4_base<float> Fract(const vec4_base<float>& a) { return vec4_base<float>(simd::fract(a.v)); }
}