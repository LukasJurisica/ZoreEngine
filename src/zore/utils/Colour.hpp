#pragma once

#include <zore/utils/DataTypes.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/ext/vector_float4.hpp>

namespace zore {

	class Colour {
	public:
		Colour() : m_r(0), m_g(0), m_b(0), m_a(0) {}
		Colour(uint8_t r, uint8_t g, uint8_t b, uint8_t a) : m_r(r), m_g(g), m_b(b), m_a(a) {}
		Colour(uint32_t c) : m_r((c >> 24) & 0xFF), m_g((c >> 16) & 0xFF), m_b((c >> 8) & 0xFF), m_a(c & 0xFF) {}

		static inline Colour rgb(uint32_t c) { return Colour(c); }
		static inline Colour rgb(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255) { return Colour(r, g, b, a); }
		static inline Colour nrgb(float r, float g, float b, float a = 1.f) { return Colour(n(r), n(g), n(b), n(a)); }
		static inline Colour rgb(const glm::vec3& c) { return Colour::nrgb(c.r, c.g, c.b); }
		static inline Colour rgb(const glm::vec4& c) { return Colour::nrgb(c.r, c.g, c.b, c.a); }
		static inline Colour hex(const char(&s)[5]) { return Colour(h(s[1], s[1]), h(s[2], s[2]), h(s[3], s[3]), 255); }
		static inline Colour hex(const char(&s)[6]) { return Colour(h(s[1], s[1]), h(s[2], s[2]), h(s[3], s[3]), h(s[4], s[4])); }
		static inline Colour hex(const char(&s)[8]) { return Colour(h(s[1], s[2]), h(s[3], s[4]), h(s[5], s[6]), 255); }
		static inline Colour hex(const char(&s)[10]) { return Colour(h(s[1], s[2]), h(s[3], s[4]), h(s[5], s[6]), h(s[7], s[8])); }
		static inline Colour invert(const Colour& colour) { return Colour(255 - colour.m_r, 255 - colour.m_g, 255 - colour.m_b, colour.m_a); }

		glm::vec3 rgb() const { return glm::vec3(m_r, m_g, m_b); }
		glm::vec4 rgba() const { return glm::vec4(m_r, m_g, m_b, m_a); }
		glm::vec3 nrgb() const { return glm::vec3(m_r, m_g, m_b) / 255.f; }
		glm::vec4 nrgba() const { return glm::vec4(m_r, m_g, m_b, m_a) / 255.f; }
		uint32_t u32() const { return (static_cast<uint32_t>(m_r) << 24) | (static_cast<uint32_t>(m_g) << 16) | (static_cast<uint32_t>(m_b) << 8) | m_a; }
		uint16_t u16() const { return (static_cast<uint16_t>(m_r >> 4) << 12) | (static_cast<uint16_t>(m_g >> 4) << 8) | (static_cast<uint16_t>(m_b >> 4) << 4) | (m_a >> 4); }
		uint8_t r() const { return m_r; }
		uint8_t g() const { return m_g; }
		uint8_t b() const { return m_b; }
		uint8_t a() const { return m_a; }
		uint8_t& r() { return m_r; }
		uint8_t& g() { return m_g; }
		uint8_t& b() { return m_b; }
		uint8_t& a() { return m_a; }
		uint8_t operator[](int index) const { return (&m_r)[index]; }
		uint8_t& operator[](int index) { return (&m_r)[index]; }

	private:
		static inline uint8_t h(char c) { return (c >= 97) ? (c - 87) : ((c >= 65) ? (c - 55) : (c - 48)); }
		static inline uint8_t h(char hi, char low) { return (h(hi) << 4) | h(low); }
		static inline uint8_t n(float x) { return static_cast<uint8_t>(x * 255.f); }

	private:
		uint8_t m_r, m_g, m_b, m_a;
	};
}