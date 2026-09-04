#include "zore/ui/layer.hpp"
#include <algorithm>
#include <cmath>

namespace zore::UI {
	static constexpr uint32_t HOVERED_BIT = 0x1;
	static constexpr uint32_t PRESSED_BIT = 0x2;

	static uint32_t pack_pair(int32_t first, int32_t second) {
		return (static_cast<uint32_t>(static_cast<uint16_t>(first)) << 16) |
			static_cast<uint16_t>(second);
	}

	Layer::Layer() : Element(Type::PANEL, nullptr) {}

	void Layer::Resize(uint32_t width, uint32_t height) {
		const int32_t w = std::min<uint32_t>(width, static_cast<uint32_t>(int16_max));
		const int32_t h = std::min<uint32_t>(height, static_cast<uint32_t>(int16_max));
		Bounds bounds;
		bounds.outer = bounds.middle = bounds.inner = { w, h, 0, 0 };
		Layout({ static_cast<int16_t>(w), static_cast<int16_t>(h) }, bounds);
		m_quads.clear();
		m_glyphs.clear();
		m_button_colliders.clear();
		m_hovered_button = -1;
		m_pressed_button = -1;
		BuildDrawData(*this, 0);
	}

	void Layer::BuildDrawData(const Element& element, uint16_t depth) {
		const Bounds& b = element.GetBounds();
		const Colour colour = element.GetStyle().m_colour;
		if (b.middle[W] > 0 && b.middle[H] > 0 && colour.a > 0) {
			const Quad quad{ pack_pair(b.middle[X], b.middle[Y]), pack_pair(b.middle[W], b.middle[H]),
				colour.u32(), static_cast<uint32_t>(depth) << 16 };
			if (element.GetType() == Type::LABEL) {
				const std::string& text = element.GetText();
				const Style& style = element.GetStyle();
				int32_t glyph_width = text.empty() ? 0 : b.middle[W] / static_cast<int32_t>(text.size());
				int32_t glyph_height = b.middle[H];
				if (!style.m_stretch_text && !text.empty()) {
					glyph_height = std::min(glyph_height, static_cast<int32_t>(std::lround(glyph_width / style.m_text_aspect_ratio)));
					glyph_width = static_cast<int32_t>(std::lround(glyph_height * style.m_text_aspect_ratio));
				}
				const int32_t text_width = glyph_width * static_cast<int32_t>(text.size());
				const int32_t text_x = b.middle[X] + (b.middle[W] - text_width) / 2;
				const int32_t text_y = b.middle[Y] + (b.middle[H] - glyph_height) / 2;
				for (size_t i = 0; i < text.size(); ++i) {
					Quad glyph = quad;
					glyph.position = pack_pair(text_x + glyph_width * static_cast<int32_t>(i), text_y);
					glyph.size = pack_pair(glyph_width, glyph_height);
					glyph.data |= static_cast<uint32_t>(static_cast<uint8_t>(text[i]) - 32u) << 8;
					m_glyphs.push_back(glyph);
				}
			} else if (element.GetType() != Type::SLIDER) {
				if (element.GetType() == Type::BUTTON) {
					m_button_colliders.push_back({ b.middle[X], b.middle[Y],
						b.middle[X] + b.middle[W], b.middle[Y] + b.middle[H], depth,
						element.GetID(), static_cast<uint32_t>(m_quads.size()) });
				}
				m_quads.push_back(quad);
			}
		}
		for (Element child : element.Children())
			BuildDrawData(child, depth + 1);
	}

	void Layer::HandleMouseMove(int32_t mouse_x, int32_t mouse_y) {
		int32_t hovered = -1;
		for (size_t i = 0; i < m_button_colliders.size(); ++i) {
			const ButtonCollider& button = m_button_colliders[i];
			if (mouse_x >= button.x1 && mouse_x <= button.x2 && mouse_y >= button.y1 && mouse_y <= button.y2 &&
				(hovered < 0 || button.depth > m_button_colliders[hovered].depth))
				hovered = static_cast<int32_t>(i);
		}
		if (hovered == m_hovered_button) return;
		if (m_hovered_button >= 0)
			m_quads[m_button_colliders[m_hovered_button].quad_index].data &= ~HOVERED_BIT;
		if (hovered >= 0)
			m_quads[m_button_colliders[hovered].quad_index].data |= HOVERED_BIT;
		m_hovered_button = hovered;
		Flush();
	}

	Element::ID Layer::HandleMousePress(uint32_t button) {
		(void)button;
		if (m_hovered_button < 0) return uint32_max;
		m_pressed_button = m_hovered_button;
		const ButtonCollider& pressed = m_button_colliders[m_pressed_button];
		m_quads[pressed.quad_index].data |= PRESSED_BIT;
		Flush();
		return pressed.id;
	}

	Element::ID Layer::HandleMouseRelease(uint32_t button) {
		(void)button;
		if (m_pressed_button < 0) return uint32_max;
		const ButtonCollider& pressed = m_button_colliders[m_pressed_button];
		m_quads[pressed.quad_index].data &= ~PRESSED_BIT;
		const ID id = m_hovered_button == m_pressed_button ? pressed.id : uint32_max;
		m_pressed_button = -1;
		Flush();
		return id;
	}

	void Layer::Flush() {
		m_draw_list.clear();
		m_quad_buffer.Set(m_quads.data(), m_quads.size() * sizeof(Quad), sizeof(Quad));
		m_draw_list.push_back({ &m_quad_buffer, Type::PANEL, static_cast<uint32_t>(m_quads.size()) });
		m_glyph_buffer.Set(m_glyphs.data(), m_glyphs.size() * sizeof(Quad), sizeof(Quad));
		m_draw_list.push_back({ &m_glyph_buffer, Type::LABEL, static_cast<uint32_t>(m_glyphs.size()) });
	}
}
