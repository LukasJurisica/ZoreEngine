#pragma once

#include "zore/utils/sized_integer.hpp"
#include "zore/math/math.hpp"
#include "zore/math/vector/vec3.hpp"
#include "zore/math/vector/vec4.hpp"

namespace zore {

	class Colour {
	public:
		Colour() : r(0), g(0), b(0), a(0) {}
		Colour(uint8_t r, uint8_t g, uint8_t b, uint8_t a) : r(r), g(g), b(b), a(a) {}
		Colour(uint32_t c) : r((c >> 24) & 0xFF), g((c >> 16) & 0xFF), b((c >> 8) & 0xFF), a(c & 0xFF) {}

		static inline Colour rgb(uint32_t c) { return Colour(c); }
		static inline Colour rgb(const zm::vec3& c) { return nrgb(c.r, c.g, c.b); }
		static inline Colour rgb(const zm::vec4& c) { return nrgb(c.r, c.g, c.b, c.a); }
		static inline Colour rgb(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255) { return Colour(r, g, b, a); }
		static inline Colour nrgb(float r, float g, float b, float a = 1.f) { return Colour(n(r), n(g), n(b), n(a)); }
		static inline Colour hex(const char(&s)[5]) { return Colour(h(s[1], s[1]), h(s[2], s[2]), h(s[3], s[3]), 255); }
		static inline Colour hex(const char(&s)[6]) { return Colour(h(s[1], s[1]), h(s[2], s[2]), h(s[3], s[3]), h(s[4], s[4])); }
		static inline Colour hex(const char(&s)[8]) { return Colour(h(s[1], s[2]), h(s[3], s[4]), h(s[5], s[6]), 255); }
		static inline Colour hex(const char(&s)[10]) { return Colour(h(s[1], s[2]), h(s[3], s[4]), h(s[5], s[6]), h(s[7], s[8])); }
		static inline Colour hsl(float h, float s, float l, float a = 1.f) { return nhsl(h / 360.f, s, l, a); }
		static Colour nhsl(float h, float s, float l, float a = 1.f);
		static inline Colour hsv(float h, float s, float v, float a = 1.f) { return nhsv(h / 360.f, s, v, a); }
		static Colour nhsv(float h, float s, float v, float a = 1.f);
		static inline Colour oklch(float l, float c, float h, float a = 1.f) { return nhsl(l, c, h / 360.f, a); }
		static Colour noklch(float l, float c, float h, float a = 1.f);
		static inline Colour invert(const Colour& colour) { return Colour(255 - colour.r, 255 - colour.g, 255 - colour.b, colour.a); }

		zm::ivec3 rgb() const { return zm::ivec3(r, g, b); }
		zm::ivec4 rgba() const { return zm::ivec4(r, g, b, a); }
		zm::vec3 nrgb() const { return zm::vec3(r, g, b) / 255.f; }
		zm::vec4 nrgba() const { return zm::vec4(r, g, b, a) / 255.f; }
		uint32_t u32() const { return (static_cast<uint32_t>(r) << 24) | (static_cast<uint32_t>(g) << 16) | (static_cast<uint32_t>(b) << 8) | static_cast<uint32_t>(a); }
		uint16_t u16() const { return (static_cast<uint16_t>(r >> 4) << 12) | (static_cast<uint16_t>(g >> 4) << 8) | (static_cast<uint16_t>(b >> 4) << 4) | static_cast<uint16_t>(a >> 4); }
		uint8_t u8() const { return ((r >> 6) << 6) | ((g >> 6) << 4) | ((b >> 6) << 2) | (a >> 6); }
		uint8_t operator[](int index) const { return (&r)[index]; }
		uint8_t& operator[](int index) { return (&r)[index]; }

	private:
		static inline uint8_t h(char c) { return (c >= 97) ? (c - 87) : ((c >= 65) ? (c - 55) : (c - 48)); }
		static inline uint8_t h(char hi, char low) { return (h(hi) << 4) | h(low); }
		static inline uint8_t n(float x) { return static_cast<uint8_t>(zm::Clamp(x, 0.f, 1.f) * 255.f); }
		static inline uint8_t HueToRgb(float p, float q, float t);
		static inline uint8_t SRGBEncode(float c);

	public:
		uint8_t r, g, b, a;
	};
}