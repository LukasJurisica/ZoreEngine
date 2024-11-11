#include "zore/ui/Layer.hpp"
#include "zore/devices/Window.hpp"
#include "zore/graphics/RenderEngine.hpp"
#include "zore/graphics/Buffer.hpp"
#include "zore/utils/UUID.hpp"
#include "zore/Debug.hpp"
#include <unordered_map>

namespace zore::UI {

	Layer* s_active_layer = nullptr;
	std::unordered_map<std::string, Layer*> s_layers;
	VertexBuffer s_quad_buffer(true);

	Layer::ColouredQuad::ColouredQuad(uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint32_t d, Colour c) :
		position((x << 16) | y), size((w << 16) | h), depth(d), colour(c.compressed()) {}

	Layer::Layer(const std::string& name) : Element(Type::PANEL, name) { 
		s_active_layer = this;
		s_layers[name] = this;
	}

	void Layer::Render() {
		if (!s_active_layer)
			return;
		s_quad_buffer.Bind();
		uint32_t count = static_cast<uint32_t>(s_active_layer->m_quads.size());
		RenderEngine::DrawLinearInstanced(4, count);
	}

	void Layer::Bind(const std::string& name) {
		auto iter = s_layers.find(name);
		if (iter == s_layers.end())
			return;
		s_active_layer = iter->second;
	}

	void Layer::Flush(uint32_t width, uint32_t height) {
		if (!s_active_layer)
			return;

		s_active_layer->m_quads.clear();
		Element::Bounds bounds = { width, height, 0, 0, width, height, 0, 0, width, height, 0, 0};
		s_active_layer->ParseUIElement(*s_active_layer, height, bounds, 0);
		s_quad_buffer.Set(s_active_layer->m_quads);
	}

	void Layer::HandleMouseMove(uint32_t mouse_x, uint32_t mouse_y) {
		if (!s_active_layer)
			return;
	}

	uint32_t Layer::HandleMouseClick(uint32_t button) {
		if (!s_active_layer)
			return luid::INVALID_ID;

		return luid::INVALID_ID;
	}

	void Layer::Bind() {
		s_active_layer = this;
	}

	void Layer::Flush() {
		Bind();
		glm::ivec2 resolution = Window::GetSize();
		Flush(resolution.x, resolution.y);	
	}

	void Layer::ParseUIElement(const Element& element, uint32_t global_height, const Element::Bounds& bounds, uint32_t depth) {
		if (bounds.middle[W] <= 0 or bounds.middle[H] <= 0)
			return;

		Colour c = element.GetStyle()->m_colour;
		// Only draw the element if it has a non-zero alpha value
		if (c.a() > 0) {
			switch (element.GetType()) {
			case Element::Type::PANEL:
				m_quads.push_back(ColouredQuad(bounds.middle[X], bounds.middle[Y], bounds.middle[W], bounds.middle[H], depth, c));
				break;
			case Element::Type::LABEL:

				break;
			case Element::Type::BUTTON:

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