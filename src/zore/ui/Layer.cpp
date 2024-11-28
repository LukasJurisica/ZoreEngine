#include "zore/ui/Layer.hpp"
#include "zore/graphics/RenderEngine.hpp"
#include "zore/graphics/Buffer.hpp"
#include "zore/utils/UUID.hpp"
#include "zore/utils/BitUtils.hpp"
#include "zore/Debug.hpp"
#include "zore/devices/Window.hpp"

#define HOVERED_BIT 0x1
#define PRESSED_BIT 0x2

namespace zore::UI {

	//========================================================================
	//	Utility Structs Class
	//========================================================================

	struct ColouredQuad {
		ColouredQuad(int32_t x, int32_t y, int32_t w, int32_t h, int32_t d, Colour c) :
			position((x << 16) | y), size((w << 16) | h), data(d << 16), colour(c.compressed()) {
		}
		int32_t position;
		int32_t size;
		int32_t data;
		int32_t colour;
	};

	struct ButtonCollider {
		ButtonCollider(int16_t x, int16_t y, int16_t w, int16_t h, int32_t d, uint32_t id, uint64_t index) :
			x1(x), x2(x + w), y1(y), y2(y + h), depth(d), id(id), index(index) {
		}
		int16_t x1, x2, y1, y2;
		uint16_t depth, id, index;
	};

	static Layer* s_active_layer = nullptr;
	static ButtonCollider* s_hovered_button = nullptr;
	static ButtonCollider* s_pressed_button = nullptr;

	static std::vector<ButtonCollider> s_button_colliders;
	static std::vector<ColouredQuad> s_quads;
	static VertexBuffer s_quad_buffer(true);

	//========================================================================
	//	Layer Class
	//========================================================================

	Layer::Layer() : Element(Type::PANEL) {
		if (!s_active_layer)
			s_active_layer = this;
	}

	void Layer::Bind() {
		if (s_active_layer != this) {
			s_active_layer = this;
			glm::ivec2 resolution = Window::GetSize();
			Resize(resolution.x, resolution.y);
			s_hovered_button = nullptr;
			s_pressed_button = nullptr;
		}
	}

	void Layer::Resize(uint32_t width, uint32_t height) {
		s_quads.clear();
		s_button_colliders.clear();
		Element::Bounds bounds = { width, height, 0, 0, width, height, 0, 0, width, height, 0, 0 };
		ParseUIElement(*s_active_layer, height, bounds, 0);
		Flush();
	}

	void Layer::Flush() {
		s_quad_buffer.Set(s_quads);
	}

	void Layer::Render() {
		s_quad_buffer.Bind();
		RenderEngine::DrawLinearInstanced(4, static_cast<uint32_t>(s_quads.size()));
	}

	void Layer::HandleMouseMove(uint32_t mouse_x, uint32_t mouse_y) {
		if (!s_active_layer)
			return;

		ButtonCollider* best_button = nullptr;

		for (ButtonCollider& button : s_button_colliders)
			if (mouse_x >= button.x1 and mouse_x <= button.x2 and mouse_y >= button.y1 and mouse_y <= button.y2 && (!best_button || button.depth > best_button->depth))
				best_button = &button;

		if (s_hovered_button != best_button) {
			if (s_hovered_button) // Clear previously hovered button
				s_quads[s_hovered_button->index].data &= ~HOVERED_BIT;
			if (best_button) // Set new hovered button
				s_quads[best_button->index].data |= HOVERED_BIT;
			s_hovered_button = best_button;
			s_active_layer->Flush();
		}
	}

	uint32_t Layer::HandleMousePress(uint32_t button) {
		if (!s_active_layer || !s_hovered_button)
			return luid::INVALID_ID;

		s_quads[s_hovered_button->index].data |= PRESSED_BIT;
		s_pressed_button = s_hovered_button;
		Flush();
		return s_hovered_button->id;
	}

	uint32_t Layer::HandleMouseRelease(uint32_t button) {
		uint32_t button_id = luid::INVALID_ID;
		if (s_pressed_button) {
			s_quads[s_pressed_button->index].data &= ~PRESSED_BIT;
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

	void Layer::ParseUIElement(const Element& element, int16_t global_height, const Element::Bounds& bounds, int16_t depth) {
		if (bounds.middle[W] <= 0 or bounds.middle[H] <= 0)
			return;

		Logger::Log(depth);

		Colour c = element.GetStyle()->m_colour;
		// Only draw the element if it has a non-zero alpha value
		if (c.a() > 0) {
			switch (element.GetType()) {
			case Element::Type::PANEL:
				s_quads.push_back({ bounds.middle[X], bounds.middle[Y], bounds.middle[W], bounds.middle[H], depth, c });
				break;
			case Element::Type::LABEL:

				break;
			case Element::Type::BUTTON:
				s_button_colliders.push_back({ bounds.middle[X], bounds.middle[Y], bounds.middle[W], bounds.middle[H], depth, element.GetUUID(), s_quads.size() });
				s_quads.push_back({ bounds.middle[X], bounds.middle[Y], bounds.middle[W], bounds.middle[H], depth, c });
				break;
			case Element::Type::SLIDER:

				break;
			}
		}

		if (bounds.inner[W] <= 0 or bounds.inner[H] <= 0)
			return;

		int16_t x_offset = 0, y_offset = 0;
		int16_t auto_count = 0, required_size = 0;
		int16_t axis = static_cast<uint32_t>(element.GetStyle()->m_flow_direction);

		for (const Element& child : element.GetChildren())
			child.ComputeRequiredSize(bounds, global_height, auto_count, required_size, axis);
		int16_t auto_size = (auto_count > 0) ? (bounds.inner[axis] - required_size) / auto_count : 0;

		depth += 1;
		for (const Element& child : element.GetChildren()) {
			Bounds child_bounds = child.ComputeBounds(bounds, global_height, auto_size, axis);

			if (element.GetStyle()->m_flow_direction == FlowDirection::HORIZONTAL) {
				child_bounds.outer[X] += x_offset;
				child_bounds.middle[X] += x_offset;
				child_bounds.inner[X] += x_offset;
				x_offset += child_bounds.outer[W];
			}
			else {
				child_bounds.outer[Y] += y_offset;
				child_bounds.middle[Y] += y_offset;
				child_bounds.inner[Y] += y_offset;
				y_offset += child_bounds.outer[H];
			}

			ParseUIElement(child, global_height, child_bounds, depth);
		}
	}
}