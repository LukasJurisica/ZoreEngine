#include "zore/ui/Layer.hpp"
#include "zore/graphics/RenderEngine.hpp"
#include "zore/graphics/Buffer.hpp"
#include "zore/utils/UUID.hpp"
#include "zore/utils/BitUtils.hpp"
#include "zore/Debug.hpp"
namespace zore::UI {

	struct ButtonCollider {
		ButtonCollider(int16_t x, int16_t y, int16_t w, int16_t h, uint32_t id, uint64_t index) : 
			x1(x), x2(x + w), y1(y), y2(y + h), id(id), index(index) {}
		int16_t x1, x2, y1, y2;
		uint16_t id, index;
	};

	ButtonCollider* s_hovered_button = nullptr;
	ButtonCollider* s_pressed_button = nullptr;

	std::vector<ButtonCollider> s_button_colliders;
	VertexBuffer s_quad_buffer(true);

	//========================================================================
	//	Layer Class
	//========================================================================

	Layer::ColouredQuad::ColouredQuad(int32_t x, int32_t y, int32_t w, int32_t h, int32_t d, Colour c) :
		position((x << 16) | y), size((w << 16) | h), data(d << 16), colour(c.compressed()) {
	}

	Layer::Layer(const std::string& style) : Element(Type::PANEL, style) {}

	void Layer::Resize(uint32_t width, uint32_t height) {
		m_quads.clear();
		s_button_colliders.clear();
		Element::Bounds bounds = { width, height, 0, 0, width, height, 0, 0, width, height, 0, 0 };
		ParseUIElement(*this, height, bounds, 0);
		Flush();
	}

	void Layer::Flush() {
		s_quad_buffer.Set(m_quads);
	}

	void Layer::Render() {
		s_quad_buffer.Bind();
		uint32_t count = static_cast<uint32_t>(m_quads.size());
		RenderEngine::DrawLinearInstanced(4, count);
	}

	//void Layer::HandleMouseMove(uint32_t mouse_x, uint32_t mouse_y) {
	//	if (!s_active_layer)
	//		return;

	//	ButtonCollider* best_button = nullptr;

	//	for (ButtonCollider& button : s_button_colliders) {
	//		if (mouse_x >= button.x1 and mouse_x <= button.x2 and mouse_y >= button.y1 and mouse_y <= button.y2)
	//			best_button = &button;
	//	}

	//	if (s_hovered_button != best_button) {
	//		if (s_hovered_button) // Clear previously hovered button
	//			s_active_layer->m_quads[s_hovered_button->index].data &= ~0x3;
	//		if (best_button) // Set new hovered button
	//			s_active_layer->m_quads[best_button->index].data |= 0x1;
	//		s_hovered_button = best_button;
	//		s_active_layer->Flush();
	//	}
	//}

	//uint32_t Layer::HandleMouseClick(uint32_t button) {
	//	if (!s_active_layer)
	//		return luid::INVALID_ID;

	//	if (s_hovered_button) {
	//		s_active_layer->m_quads[s_hovered_button->index].data |= (0x1 << 1);
	//		s_active_layer->Flush();
	//		return s_hovered_button->id;
	//	}
	//	return luid::INVALID_ID;
	//}

	//uint32_t Layer::HandleKeyPress(uint32_t key) {
	//	if (!s_active_layer)
	//		return luid::INVALID_ID;

	//	return luid::INVALID_ID;
	//}

	void Layer::ParseUIElement(const Element& element, uint32_t global_height, const Element::Bounds& bounds, int32_t depth) {
		if (bounds.middle[W] <= 0 or bounds.middle[H] <= 0)
			return;

		Colour c = element.GetStyle()->m_colour;
		// Only draw the element if it has a non-zero alpha value
		if (c.a() > 0) {
			switch (element.GetType()) {
			case Element::Type::PANEL:
				m_quads.push_back({ bounds.middle[X], bounds.middle[Y], bounds.middle[W], bounds.middle[H], depth, c });
				break;
			case Element::Type::LABEL:

				break;
			case Element::Type::BUTTON:
				s_button_colliders.push_back({ bounds.middle[X], bounds.middle[Y], bounds.middle[W], bounds.middle[H], element.GetUUID(), m_quads.size() });
				m_quads.push_back({ bounds.middle[X], bounds.middle[Y], bounds.middle[W], bounds.middle[H], depth, c });
				break;
			case Element::Type::SLIDER:

				break;
			}
		}

		if (bounds.inner[W] <= 0 or bounds.inner[H] <= 0)
			return;

		depth += 1;
		uint32_t x_offset = 0, y_offset = 0;
		uint32_t auto_count = 0, required_size = 0;
		uint32_t axis = static_cast<uint32_t>(element.GetStyle()->m_flow_direction);

		for (const Element& child : element.GetChildren())
			child.ComputeRequiredSize(bounds, global_height, auto_count, required_size, axis);
		uint32_t auto_size = (auto_count > 0) ? (bounds.inner[axis] - required_size) / auto_count : 0;

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