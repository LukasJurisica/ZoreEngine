#include "zore/utils/colour.hpp"

namespace zore {

	Colour Colour::nhsl(float h, float s, float l, float a) {
		if (s > 0.f) {
			float q = (l < 0.5f) ? (l * (1.f + s)) : ((l + s) - (l * s));
			float p = 2.f * l - q;
			return Colour(HueToRgb(p, q, h + (1.f / 3.f)), HueToRgb(p, q, h), HueToRgb(p, q, h - (1.f / 3.f)), n(a));
		}
		return Colour(n(l), n(l), n(l), n(a));
	}

	Colour Colour::nhsv(float h, float s, float v, float a) {
		h = fmodf(h * 6.f, 6.f);
		int i = static_cast<int>(h);
		h -= static_cast<float>(i);
		float p = v * (1.f - s);
		float q = v * (1.f - s * h);
		float t = v * (1.f - s * (1.f - h));

		switch (i) {
		case 0: return Colour(n(v), n(t), n(p), n(a));
		case 1: return Colour(n(q), n(v), n(p), n(a));
		case 2: return Colour(n(p), n(v), n(t), n(a));
		case 3: return Colour(n(p), n(q), n(v), n(a));
		case 4: return Colour(n(t), n(p), n(v), n(a));
		default: return Colour(n(v), n(p), n(q), n(a));
		}
	}

	Colour Colour::noklch(float l, float c, float h, float a) {
		float x = c * std::cos(h * 2.f * zm::PI);
		float y = c * std::sin(h * 2.f * zm::PI);
		float u = l + (0.3963377774f * x) + (0.2158037573f * y);
		float v = l - (0.1055613458f * x) - (0.0638541728f * y);
		float w = l - (0.0894841775f * x) - (1.2914855480f * y);
		u = u * u * u;
		v = v * v * v;
		w = w * w * w;
		float r = (+4.0767416621f * u) - (3.3077115913f * v) + (0.2309699292f * w);
		float g = (-1.2684380046f * u) + (2.6097574011f * v) - (0.3413193965f * w);
		float b = (-0.0041960863f * u) - (0.7034186147f * v) + (1.7076147010f * w);
		return Colour(SRGBEncode(r), SRGBEncode(g), SRGBEncode(b), n(a));
	}

	uint8_t Colour::HueToRgb(float p, float q, float t) {
		t = zm::WrapClamp(t, 0.f, 1.f);
		if (t < (1.f / 6.f)) return n(p + (q - p) * 6.f * t);
		if (t < (3.f / 6.f)) return n(q);
		if (t < (4.f / 6.f)) return n(p + (q - p) * ((2.f / 3.f) - t) * 6.f);
		return n(p);
	}

	uint8_t Colour::SRGBEncode(float c) {
		if (c <= 0.0031308f)
			return n(c *= 12.92f);
		return n(1.055f * std::pow(c, 1.f / 2.4f) - 0.055f);
	}
}