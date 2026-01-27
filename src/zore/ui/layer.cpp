#include "zore/ui/layer.hpp"
#include "zore/graphics/render_engine.hpp"
#include "zore/math/math.hpp"
#include "zore/utils/uuid.hpp"
#include "zore/utils/bits.hpp"
#include "zore/devices/Window.hpp"

#define HOVERED_BIT 0x1
#define PRESSED_BIT 0x2

namespace zore::UI {

	//========================================================================
	//	Utility Structs Class
	//========================================================================

	struct ColouredQuad {
		ColouredQuad(int32_t x, int32_t y, int32_t w, int32_t h, int32_t d, Colour c) :
			position((x << 16) | y), size((w << 16) | h), colour(c.u32()), data(d << 16) {
		}
		int32_t position;
		int32_t size;
		int32_t colour;
		int32_t data;
	};

	struct GlyphQuad {
		GlyphQuad(int32_t x, int32_t y, int32_t w, int32_t h, int32_t d, Colour c, int8_t glyph) :
			position((x << 16) | y), size((w << 16) | h), colour(c.u32()), data((d << 16) | (static_cast<uint32_t>(glyph - 32) << 8)) {
		}
		int32_t position;
		int32_t size;
		int32_t colour;
		int32_t data;
	};

	struct ButtonCollider {
		ButtonCollider(int16_t x, int16_t y, int16_t w, int16_t h, int16_t d, uint32_t id, uint32_t index) :
			x1(x), x2(x + w), y1(y), y2(y + h), depth(d), id(id), index(index) {
		}
		int16_t x1, x2, y1, y2, depth;
		uint32_t id, index;
	};

	static Layer* s_active_layer = nullptr;
	static ButtonCollider* s_hovered_button = nullptr;
	static ButtonCollider* s_pressed_button = nullptr;

	static std::vector<ButtonCollider> s_button_colliders;
	static std::vector<ColouredQuad> s_coloured_quads;
	static std::vector<GlyphQuad> s_glyph_quads;
	static VertexBuffer s_quad_buffer(true);
	static VertexBuffer s_glyph_buffer(true);

	static std::vector<DrawCommand> s_draw_list;

	//========================================================================
	//	Layer Class
	//========================================================================

	Layer::Layer() : Element(Type::PANEL) {
		if (!s_active_layer)
			Bind();
	}

	void Layer::Bind() {
		if (s_active_layer != this) {
			s_active_layer = this;
			zm::ivec2 resolution = Window::GetSize();
			Resize(resolution.x, resolution.y);
			s_hovered_button = nullptr;
			s_pressed_button = nullptr;
		}
	}

	void Layer::Resize(uint32_t width, uint32_t height) {
		s_coloured_quads.clear();
		s_glyph_quads.clear();
		s_button_colliders.clear();
		int16_t w = static_cast<int16_t>(width), h = static_cast<int16_t>(height);
		Element::Bounds bounds = { w, h, 0, 0, w, h, 0, 0, w, h, 0, 0 };
		ParseUIElement(s_active_layer, w, h, bounds, 0);
		Flush();
	}

	void Layer::Flush() {
		s_draw_list.clear();
		s_quad_buffer.Set(s_coloured_quads);
		s_draw_list.push_back({ &s_quad_buffer, Element::Type::PANEL, static_cast<uint32_t>(s_coloured_quads.size()) });
		s_glyph_buffer.Set(s_glyph_quads);
		s_draw_list.push_back({ &s_glyph_buffer, Element::Type::LABEL, static_cast<uint32_t>(s_glyph_quads.size()) });
	}

	const std::vector<DrawCommand>& Layer::GetDrawList() {
		return s_draw_list;
	}

	void Layer::HandleMouseMove(int32_t mouse_x, int32_t mouse_y) {
		if (!s_active_layer)
			return;

		ButtonCollider* top_button = nullptr;

		for (ButtonCollider& button : s_button_colliders)
			if (mouse_x >= button.x1 and mouse_x <= button.x2 and mouse_y >= button.y1 and mouse_y <= button.y2 && (!top_button || button.depth > top_button->depth))
				top_button = &button;

		if (s_hovered_button != top_button) {
			if (s_hovered_button) // Clear previously hovered button
				s_coloured_quads[s_hovered_button->index].data &= ~HOVERED_BIT;
			if (top_button) // Set new hovered button
				s_coloured_quads[top_button->index].data |= HOVERED_BIT;
			s_hovered_button = top_button;
			s_active_layer->Flush();
		}
	}

	uint32_t Layer::HandleMousePress(uint32_t button) {
		if (!s_active_layer || !s_hovered_button)
			return luid::INVALID_ID;

		s_coloured_quads[s_hovered_button->index].data |= PRESSED_BIT;
		s_pressed_button = s_hovered_button;
		Flush();
		return s_hovered_button->id;
	}

	uint32_t Layer::HandleMouseRelease(uint32_t button) {
		uint32_t button_id = luid::INVALID_ID;
		if (s_pressed_button) {
			s_coloured_quads[s_pressed_button->index].data &= ~PRESSED_BIT;
			Flush();
			if (s_hovered_button == s_pressed_button)
				button_id = s_hovered_button->id;
		}
		s_pressed_button = nullptr;
		return button_id;
	}

	uint32_t Layer::HandleKeyPress(uint32_t key) {
		return luid::INVALID_ID;
	}

	void Layer::ParseUIElement(Element* element, int16_t viewport_width, int16_t viewport_height, const Element::Bounds& bounds, int16_t depth) {
		if (bounds.middle[W] <= 0 || bounds.middle[H] <= 0)
			return;

		Colour c = element->GetStyle()->m_colour;
		const int16_t& x = bounds.middle[X], y = bounds.middle[Y], w = bounds.middle[W], h = bounds.middle[H];
		const std::string& text = element->GetText();
		int16_t glyph_width = text.length() > 0 ? (w / text.length()) : 0;
		int idx = 0;

		// Only draw the element if it has a non-zero alpha value
		if (c.a > 0) {
			switch (element->GetType()) {
			case Element::Type::PANEL:
				s_coloured_quads.push_back({ x, y, w, h, depth, c });
				break;
			case Element::Type::LABEL:
				for (int i = 0; i < text.length(); i++)
					s_glyph_quads.push_back({ x + (glyph_width * i), y, glyph_width, h, depth, c, text[i] });
				//s_coloured_quads.push_back({ x + (glyph_width * i), y, (glyph_width >> 1), h, depth, c });
				break;
			case Element::Type::BUTTON:
				s_button_colliders.push_back({ x, y, w, h, depth, element->GetUUID(), static_cast<uint32_t>(s_coloured_quads.size()) });
				s_coloured_quads.push_back({ x, y, w, h, depth, c });
				break;
			case Element::Type::SLIDER:

				break;
			}
		}

		if (bounds.inner[W] <= 0 || bounds.inner[H] <= 0 || element->Children().size() <= 0)
			return;

		int16_t x_offset = 0, y_offset = 0;
		LayoutParams params(bounds, viewport_width, viewport_height, element->GetStyle()->m_flow_direction);
		for (UNIQUE<Element>& child : element->Children())
			child->ComputeRequiredSize(params, params.flow_axis);

		int16_t x_gap, y_gap;
		uint16_t gap_count = static_cast<uint16_t>(element->Children().size()) - 1;
		int16_t min_gap = element->GetStyle()->m_min_gap[params.flow_axis].Get(params.viewport_size, params.parent_bounds.inner, 0, params.flow_axis);
		int16_t max_gap = element->GetStyle()->m_max_gap[params.flow_axis].Get(params.viewport_size, params.parent_bounds.inner, 0, params.flow_axis);

		if (element->GetStyle()->m_gap[params.flow_axis].GetType() == Unit::Type::AUTO) {
			for (int i = 0; i < gap_count; i++) {
				params.auto_params[params.flow_axis].count += 1;
				params.auto_params[params.flow_axis].auto_max_sizes.push_back(max_gap);
				params.auto_params[params.flow_axis].auto_min_sizes.push_back(min_gap);
			}
		}
		else {
			int16_t gap = zm::Clamp(element->GetStyle()->m_gap[params.flow_axis].Get(params.viewport_size, params.parent_bounds.inner, 0, params.flow_axis), min_gap, max_gap);
			params.auto_params[params.flow_axis].required_size += gap * gap_count;
		}

		int16_t auto_size = params.GetAutoSize(params.flow_axis);
		x_gap = element->GetStyle()->m_gap[W].Get(params.viewport_size, params.parent_bounds.inner, zm::Clamp(auto_size, min_gap, max_gap), W);
		y_gap = element->GetStyle()->m_gap[H].Get(params.viewport_size, params.parent_bounds.inner, zm::Clamp(auto_size, min_gap, max_gap), H);

		for (UNIQUE<Element>& child : element->Children()) {
			Bounds child_bounds = child->ComputeBounds(params, auto_size);

			if (element->GetStyle()->m_flow_direction == FlowDirection::HORIZONTAL) {
				child_bounds.outer[X] += x_offset;
				child_bounds.middle[X] += x_offset;
				child_bounds.inner[X] += x_offset;
				x_offset += child_bounds.outer[W] + x_gap;
			}
			else {
				child_bounds.outer[Y] += y_offset;
				child_bounds.middle[Y] += y_offset;
				child_bounds.inner[Y] += y_offset;
				y_offset += child_bounds.outer[H] + y_gap;
			}

			ParseUIElement(child.get(), viewport_width, viewport_height, child_bounds, depth + 1);
		}
	}
}