#include "zore/ui/element.hpp"
#include "zore/structures/object_pool.hpp"
#include <algorithm>
#include <cmath>

namespace zore::UI {

	static zore::object_pool<Element::Data, Element::ID> s_element_pool;

	static int32_t clamp_value(int32_t value, int32_t minimum, int32_t maximum) {
		return std::clamp(value, minimum, std::max(minimum, maximum));
	}

	void Element::Segment::Add(Unit minimum, Unit value, Unit maximum, zm::sivec2 viewport, zm::sivec2 parent, int16_t axis, float scale) {
		const int32_t lo = minimum.Get(viewport, parent, 0, axis);
		const int32_t hi = std::max<int32_t>(lo, maximum.Get(viewport, parent, 0, axis));
		min_size += static_cast<int32_t>(std::lround(lo * scale));
		max_size += static_cast<int32_t>(std::lround(hi * scale));
		if (value.GetType() == Unit::Type::AUTO)
			++auto_count;
		else
			size += static_cast<int32_t>(std::lround(clamp_value(value.Get(viewport, parent, 0, axis), lo, hi) * scale));
	}

	Element::Element(Type type, const Style* style) {
		m_id = s_element_pool.acquire(type, style ? style : &Style::Get(""));
	}

	Element Element::Append(Type type, const Style* style) {
		Element child(type, style);
		s_element_pool[m_id].m_children.push_back(child.m_id);
		return child;
	}

	Element Element::Append(Type type, std::string_view style) {
		return Append(type, &Style::Get(style));
	}

	Element& Element::SetStyle(const Style* style) {
		s_element_pool[m_id].m_style = style ? style : &Style::Get("");
		return *this;
	}

	Element& Element::SetStyle(std::string_view style) {
		return SetStyle(&Style::Get(style));
	}

	Element& Element::SetText(std::string_view text) {
		s_element_pool[m_id].m_text.assign(text);
		return *this;
	}

	Element::ID Element::GetID() const {
		return m_id;
	}

	Element::Type Element::GetType() const {
		return s_element_pool[m_id].m_type;
	}

	const Style& Element::GetStyle() const {
		return *s_element_pool[m_id].m_style;
	}

	const std::string& Element::GetText() const {
		return s_element_pool[m_id].m_text;
	}

	const Element::Bounds& Element::GetBounds() const {
		return s_element_pool[m_id].m_bounds;
	}

	std::vector<Element> Element::Children() const {
		std::vector<Element> children;
		children.reserve(s_element_pool[m_id].m_children.size());
		for (ID id : s_element_pool[m_id].m_children)
			children.push_back(Element(id));
		return children;
	}

	void Element::UpdateConstraints(zm::sivec2 viewport, zm::sivec2 parent) {
		Data& data = s_element_pool[m_id];
		const Style& style = *data.m_style;
		for (uint8_t axis = 0; axis < 2; ++axis) {
			data.m_min_size[axis] = style.m_min_size[axis].Get(viewport, parent, 0, axis);
			data.m_max_size[axis] = std::max<int32_t>(data.m_min_size[axis], style.m_max_size[axis].Get(viewport, parent, 0, axis));
			for (uint8_t side : { axis, static_cast<uint8_t>(axis + 2) }) {
				data.m_min_margin[side] = style.m_min_margin[side].Get(viewport, parent, 0, axis);
				data.m_max_margin[side] = std::max<int32_t>(data.m_min_margin[side], style.m_max_margin[side].Get(viewport, parent, 0, axis));
			}
		}
	}

	Element::Segment Element::GetSize(zm::sivec2 viewport, zm::sivec2 parent, uint8_t axis) const {
		const Style& style = *(s_element_pool[m_id].m_style);
		Segment result;
		result.Add(style.m_min_size[axis], style.m_size[axis], style.m_max_size[axis], viewport, parent, axis);
		result.Add(style.m_min_margin[axis], style.m_margin[axis], style.m_max_margin[axis], viewport, parent, axis);
		result.Add(style.m_min_margin[axis + 2], style.m_margin[axis + 2], style.m_max_margin[axis + 2], viewport, parent, axis);
		return result;
	}

	int32_t Element::Resolve(Unit value, Unit minimum, Unit maximum, zm::sivec2 viewport,
		zm::sivec2 parent, uint8_t axis, bool& automatic) {
		const int32_t lo = minimum.Get(viewport, parent, 0, axis);
		const int32_t hi = std::max<int32_t>(lo, maximum.Get(viewport, parent, 0, axis));
		automatic = value.GetType() == Unit::Type::AUTO;
		return automatic ? 0 : clamp_value(value.Get(viewport, parent, 0, axis), lo, hi);
	}

	void Element::Distribute(int32_t available, std::vector<AutoSlot>& slots) {
		if (slots.empty()) return;
		available = std::max(0, available);
		std::vector<size_t> active(slots.size());
		for (size_t i = 0; i < slots.size(); ++i) active[i] = i;
		while (!active.empty()) {
			const int32_t share = available / static_cast<int32_t>(active.size());
			bool constrained = false;
			for (auto it = active.begin(); it != active.end();) {
				AutoSlot& slot = slots[*it];
				if (share < slot.min || share > slot.max) {
					*slot.value = share < slot.min ? slot.min : slot.max;
					available -= *slot.value;
					it = active.erase(it);
					constrained = true;
				} else ++it;
			}
			if (!constrained) {
				for (size_t i = 0; i < active.size(); ++i) {
					AutoSlot& slot = slots[active[i]];
					*slot.value = share + (i < static_cast<size_t>(available % active.size()) ? 1 : 0);
				}
				break;
			}
			available = std::max(0, available);
		}
	}

	void Element::ResolveIndependentAxis(Data& data, zm::sivec2 viewport, zm::sivec2 parent, uint8_t axis) {
		const Style& s = *data.m_style;
		std::vector<AutoSlot> slots;
		int32_t fixed = 0;
		bool automatic = false;
		for (uint8_t side : { axis, static_cast<uint8_t>(axis + 2) }) {
			data.m_margin[side] = Resolve(s.m_margin[side], s.m_min_margin[side], s.m_max_margin[side], viewport, parent, axis, automatic);
			if (automatic) slots.push_back({ &data.m_margin[side], data.m_min_margin[side], data.m_max_margin[side] });
			else fixed += data.m_margin[side];
		}
		data.m_size[axis] = Resolve(s.m_size[axis], s.m_min_size[axis], s.m_max_size[axis], viewport, parent, axis, automatic);
		if (automatic) slots.push_back({ &data.m_size[axis], data.m_min_size[axis], data.m_max_size[axis] });
		else fixed += data.m_size[axis];
		Distribute(parent[axis] - fixed, slots);
	}

	int32_t Element::ProportionalSize(const Data& data, uint8_t axis) {
		float ratio = data.m_style->m_aspect_ratio;
		if (data.m_type == Type::LABEL && !data.m_text.empty() && axis == W)
			ratio = static_cast<float>(data.m_text.size()) * data.m_style->m_text_aspect_ratio;
		return static_cast<int32_t>(std::lround(data.m_size[1 - axis] * ratio));
	}

	void Element::Layout(zm::sivec2 viewport, const Bounds& bounds) {
		Data& data = s_element_pool[m_id];
		data.m_bounds = bounds;
		data.m_size = { bounds.middle[W], bounds.middle[H] };
		LayoutChildren(viewport);
	}

	void Element::LayoutChildren(zm::sivec2 viewport) {
		Data& parent_data = s_element_pool[m_id];
		if (parent_data.m_children.empty()) return;
		const Style& parent_style = *parent_data.m_style;
		const zm::sivec2 parent_size(parent_data.m_bounds.inner[W], parent_data.m_bounds.inner[H]);
		const uint8_t flow = parent_style.m_flow_direction == Axis::HORIZONTAL ? W : H;
		const uint8_t cross = 1 - flow;
		std::vector<AutoSlot> slots;
		int32_t fixed = 0;

		for (ID id : parent_data.m_children) {
			Data& child = s_element_pool[id];
			Element handle(id);
			handle.UpdateConstraints(viewport, parent_size);
			const Style& s = *child.m_style;
			// A cross-axis size which depends on the flow size must wait until flow-axis auto distribution has completed.
			if (s.m_dependent_axis != cross)
				ResolveIndependentAxis(child, viewport, parent_size, cross);
			bool automatic = false;
			for (uint8_t side : { flow, static_cast<uint8_t>(flow + 2) }) {
				child.m_margin[side] = Resolve(s.m_margin[side], s.m_min_margin[side], s.m_max_margin[side], viewport, parent_size, flow, automatic);
				if (automatic) slots.push_back({ &child.m_margin[side], child.m_min_margin[side], child.m_max_margin[side] });
				else fixed += child.m_margin[side];
			}
			if (s.m_dependent_axis == flow) {
				child.m_size[flow] = clamp_value(ProportionalSize(child, flow), child.m_min_size[flow], child.m_max_size[flow]);
				fixed += child.m_size[flow];
			} else {
				child.m_size[flow] = Resolve(s.m_size[flow], s.m_min_size[flow], s.m_max_size[flow], viewport, parent_size, flow, automatic);
				if (automatic) slots.push_back({ &child.m_size[flow], child.m_min_size[flow], child.m_max_size[flow] });
				else fixed += child.m_size[flow];
			}
		}

		const size_t gap_count = parent_data.m_children.size() - 1;
		std::vector<int32_t> gaps(gap_count, 0);
		for (size_t i = 0; i < gap_count; ++i) {
			bool automatic = false;
			gaps[i] = Resolve(parent_style.m_gap[flow], parent_style.m_min_gap[flow], parent_style.m_max_gap[flow], viewport, parent_size, flow, automatic);
			const int32_t lo = parent_style.m_min_gap[flow].Get(viewport, parent_size, 0, flow);
			const int32_t hi = std::max<int32_t>(lo, parent_style.m_max_gap[flow].Get(viewport, parent_size, 0, flow));
			if (automatic) slots.push_back({ &gaps[i], lo, hi }); else fixed += gaps[i];
		}
		Distribute(parent_size[flow] - fixed, slots);

		int32_t cursor = parent_data.m_bounds.inner[flow == W ? X : Y];
		for (size_t index = 0; index < parent_data.m_children.size(); ++index) {
			ID id = parent_data.m_children[index];
			Data& child = s_element_pool[id];
			const Style& s = *child.m_style;
			if (s.m_dependent_axis == cross) {
				child.m_size[cross] = clamp_value(ProportionalSize(child, cross), child.m_min_size[cross], child.m_max_size[cross]);
				std::vector<AutoSlot> cross_slots;
				int32_t cross_fixed = child.m_size[cross];
				bool automatic = false;
				for (uint8_t side : { cross, static_cast<uint8_t>(cross + 2) }) {
					child.m_margin[side] = Resolve(s.m_margin[side], s.m_min_margin[side], s.m_max_margin[side], viewport, parent_size, cross, automatic);
					if (automatic) cross_slots.push_back({ &child.m_margin[side], child.m_min_margin[side], child.m_max_margin[side] });
					else cross_fixed += child.m_margin[side];
				}
				Distribute(parent_size[cross] - cross_fixed, cross_slots);
			}
			const int32_t outer_x = flow == W ? cursor : parent_data.m_bounds.inner[X];
			const int32_t outer_y = flow == H ? cursor : parent_data.m_bounds.inner[Y];
			Bounds bounds;
			bounds.outer = {
				child.m_size[W] + child.m_margin[L] + child.m_margin[R],
				child.m_size[H] + child.m_margin[T] + child.m_margin[B],
				outer_x, outer_y };
			bounds.middle = {
				child.m_size[W], child.m_size[H],
				outer_x + child.m_margin[L], outer_y + child.m_margin[T] };
			int32_t padding[4];
			for (uint8_t side = 0; side < 4; ++side) {
				const uint8_t axis = side & 1;
				const int32_t lo = s.m_min_padding[side].Get(viewport, child.m_size, 0, axis);
				const int32_t hi = std::max<int32_t>(lo, s.m_max_padding[side].Get(viewport, child.m_size, 0, axis));
				padding[side] = clamp_value(s.m_padding[side].Get(viewport, child.m_size, 0, axis), lo, hi);
			}
			bounds.inner = {
				std::max(0, bounds.middle[W] - padding[L] - padding[R]),
				std::max(0, bounds.middle[H] - padding[T] - padding[B]),
				bounds.middle[X] + padding[L], bounds.middle[Y] + padding[T] };
			Element handle(id);
			handle.Layout(viewport, bounds);
			cursor += bounds.outer[flow] + (index < gaps.size() ? gaps[index] : 0);
		}
	}
}
