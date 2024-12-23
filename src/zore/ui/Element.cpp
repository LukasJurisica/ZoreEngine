#include "zore/ui/Element.hpp"
#include "zore/utils/UUID.hpp"
#include "zore/math/MathUtils.hpp"
#include "zore/Debug.hpp"

#include <algorithm>

namespace zore::UI {

#define INT16_MAX_VALUE 32767

	//========================================================================
	//	Element Class
	//========================================================================

	int16_t Element::AutoParams::GetAutoSize(int16_t parent_size) {
		if (count <= 0)
			return 0;

		int16_t l_count = count, l_required_size = required_size;
		int16_t size = (parent_size - required_size) / count;

		std::sort(auto_max_sizes.begin(), auto_max_sizes.end());
		for (int16_t x : auto_max_sizes) {
			if (count == 1)
				return parent_size - required_size;
			else if (x > size)
				break;
			size = (parent_size - (required_size += x)) / (--count);
		}

		std::sort(auto_min_sizes.rbegin(), auto_min_sizes.rend());
		for (int16_t x : auto_min_sizes) {
			if (count == 1)
				return parent_size - required_size;
			else if (x < size)
				break;
			size = (parent_size - (required_size += x)) / (--count);
		}

		return size;
	}

	Element::LayoutParams::LayoutParams(const Bounds& bounds, int16_t viewport_width, int16_t viewport_height, FlowDirection direction)
		: parent_bounds(bounds), viewport_size{ viewport_width, viewport_height }, flow_axis(static_cast<int16_t>(direction)) {
	}

	Element::Element(Type type, const std::string& style) : Element(type, Style::Get(style)) {}
	Element::Element(Type type, const Style* style) :
		m_type(type), m_style(style), m_id(luid::Generate()),
		m_size{}, m_max_size{}, m_min_size{}, m_margin{}, m_min_margin{}, m_max_margin{} {
	}

	Element* Element::AddChild(const Element& element) {
		m_children.push_back(element);
		return &m_children.back();
	}

	Element* Element::AddChild(const Element& element, const std::string& style) {
		m_children.push_back(element);
		m_children.back().SetStyle(style);
		return &m_children.back();
	}

	Element::Bounds Element::ComputeBounds(LayoutParams& layout, int16_t auto_size) {
		// Compute size of element if any used auto specifier
		int16_t axis = layout.flow_axis;
		UpdateIfAuto(m_style->m_margin[axis + 0].GetType(), m_margin[axis + 0], auto_size);
		UpdateIfAuto(m_style->m_margin[axis + 2].GetType(), m_margin[axis + 2], auto_size);
		UpdateIfAuto(m_style->m_size[axis].GetType(), m_size[axis], zm::Clamp(auto_size, m_min_size[axis], m_max_size[axis]));

		// Compute size of secondary axis if it relies on the primary axis, and primary axis was auto
		if (m_style->m_dependent_axis == (1 - layout.flow_axis) and m_style->m_size[layout.flow_axis].GetType() == Unit::Type::AUT)
			ComputeSizeOfSecondaryAxis(layout);

		Bounds bounds;

		// Compute the size and position of the middle content (self)
		bounds.middle[X] = layout.parent_bounds.inner[X] + m_margin[L];
		bounds.middle[Y] = layout.parent_bounds.inner[Y] + m_margin[T];
		bounds.middle[W] = m_size[W];
		bounds.middle[H] = m_size[H];

		// Compute the size and position of the outer content (self + margins)
		bounds.outer[X] = layout.parent_bounds.inner[X];
		bounds.outer[Y] = layout.parent_bounds.inner[Y];
		bounds.outer[W] = bounds.middle[W] + m_margin[L] + m_margin[R];
		bounds.outer[H] = bounds.middle[H] + m_margin[T] + m_margin[B];

		// Compute the size and position of the contained inner content (self - padding)
		int16_t min_padding[4] = {
			m_style->m_min_padding[T].Get(layout.viewport_size, layout.parent_bounds.inner[H], 0),
			m_style->m_min_padding[R].Get(layout.viewport_size, layout.parent_bounds.inner[W], 0),
			m_style->m_min_padding[B].Get(layout.viewport_size, layout.parent_bounds.inner[H], 0),
			m_style->m_min_padding[L].Get(layout.viewport_size, layout.parent_bounds.inner[W], 0)
		};
		int16_t max_padding[4] = {
			m_style->m_max_padding[T].Get(layout.viewport_size, layout.parent_bounds.inner[H], 0),
			m_style->m_max_padding[R].Get(layout.viewport_size, layout.parent_bounds.inner[W], 0),
			m_style->m_max_padding[B].Get(layout.viewport_size, layout.parent_bounds.inner[H], 0),
			m_style->m_max_padding[L].Get(layout.viewport_size, layout.parent_bounds.inner[W], 0)
		};
		int16_t padding_t = zm::Clamp(m_style->m_padding[T].Get(layout.viewport_size, layout.parent_bounds.inner[H], 0), min_padding[T], max_padding[T]);
		int16_t padding_r = zm::Clamp(m_style->m_padding[R].Get(layout.viewport_size, layout.parent_bounds.inner[W], 0), min_padding[R], max_padding[R]);
		int16_t padding_b = zm::Clamp(m_style->m_padding[B].Get(layout.viewport_size, layout.parent_bounds.inner[H], 0), min_padding[B], max_padding[B]);
		int16_t padding_l = zm::Clamp(m_style->m_padding[L].Get(layout.viewport_size, layout.parent_bounds.inner[W], 0), min_padding[L], max_padding[L]);
		bounds.inner[X] = bounds.middle[X] + padding_l;
		bounds.inner[Y] = bounds.middle[Y] + padding_t;
		bounds.inner[W] = bounds.middle[W] - padding_l - padding_r;
		bounds.inner[H] = bounds.middle[H] - padding_t - padding_b;

		return bounds;
	}

	void Element::ComputeRequiredSize(LayoutParams& layout, int16_t axis) {
		m_min_size[W] = m_style->m_min_size[W].Get(layout.viewport_size, layout.parent_bounds.inner[W], 0);
		m_min_size[H] = m_style->m_min_size[H].Get(layout.viewport_size, layout.parent_bounds.inner[H], 0);
		m_max_size[W] = m_style->m_max_size[W].Get(layout.viewport_size, layout.parent_bounds.inner[W], 0);
		m_max_size[H] = m_style->m_max_size[H].Get(layout.viewport_size, layout.parent_bounds.inner[H], 0);

		m_min_margin[T] = m_style->m_min_margin[T].Get(layout.viewport_size, layout.parent_bounds.inner[H], 0);
		m_min_margin[R] = m_style->m_min_margin[R].Get(layout.viewport_size, layout.parent_bounds.inner[W], 0);
		m_min_margin[B] = m_style->m_min_margin[B].Get(layout.viewport_size, layout.parent_bounds.inner[H], 0);
		m_min_margin[L] = m_style->m_min_margin[L].Get(layout.viewport_size, layout.parent_bounds.inner[W], 0);

		m_max_margin[T] = m_style->m_max_margin[T].Get(layout.viewport_size, layout.parent_bounds.inner[H], 0);
		m_max_margin[R] = m_style->m_max_margin[R].Get(layout.viewport_size, layout.parent_bounds.inner[W], 0);
		m_max_margin[B] = m_style->m_max_margin[B].Get(layout.viewport_size, layout.parent_bounds.inner[H], 0);
		m_max_margin[L] = m_style->m_max_margin[L].Get(layout.viewport_size, layout.parent_bounds.inner[W], 0);

		// Compute size of element along primary axis if it is not dependent on the secondary axis
		if (m_style->m_dependent_axis != axis)
			ComputeRequiredSize(layout, axis, m_style->m_size[axis], m_min_size[axis], m_max_size[axis], m_size[axis]);
		// Compute size of secondary axis if it does not rely on the primary axis, or the primary axis is calculable
		if (m_style->m_dependent_axis != (1 - axis) or m_style->m_size[axis].GetType() != Unit::Type::AUT) {
			ComputeSizeOfSecondaryAxis(layout);
			if (m_style->m_dependent_axis == axis) {
				m_size[axis] = static_cast<int16_t>(zm::Round(m_size[1 - axis] * m_style->m_aspect_ratio));
				layout.auto_params[axis].required_size += m_size[axis];
			}
		}

		// Compute size of margins on primary axis
		ComputeRequiredSize(layout, axis, m_style->m_margin[axis + 0], m_min_margin[axis + 0], m_max_margin[axis + 0], m_margin[axis + 0]);
		ComputeRequiredSize(layout, axis, m_style->m_margin[axis + 2], m_min_margin[axis + 2], m_max_margin[axis + 2], m_margin[axis + 2]);
	}

	void Element::ComputeSizeOfSecondaryAxis(LayoutParams& layout) {
		int16_t axis = 1 - layout.flow_axis;
		layout.auto_params[axis].required_size = 0;
		layout.auto_params[axis].count = 0;
		ComputeRequiredSize(layout, axis, m_style->m_margin[axis + 0], m_min_margin[axis + 0], m_max_margin[axis + 0], m_margin[axis + 0]);
		ComputeRequiredSize(layout, axis, m_style->m_margin[axis + 2], m_min_margin[axis + 2], m_max_margin[axis + 2], m_margin[axis + 2]);
		if (m_style->m_dependent_axis == axis) {
			m_size[axis] = zm::Min(m_max_size[axis], static_cast<int16_t>(zm::Round(m_size[layout.flow_axis] * m_style->m_aspect_ratio)));
			layout.auto_params[axis].required_size += m_size[axis];
		}
		else
			ComputeRequiredSize(layout, axis, m_style->m_size[axis], m_min_size[axis], m_max_size[axis], m_size[axis]);
		int16_t auto_size = layout.GetAutoSize(axis);
		UpdateIfAuto(m_style->m_margin[axis + 0].GetType(), m_margin[axis + 0], zm::Clamp(auto_size, m_min_margin[axis + 0], m_max_margin[axis + 0]));
		UpdateIfAuto(m_style->m_margin[axis + 2].GetType(), m_margin[axis + 2], zm::Clamp(auto_size, m_min_margin[axis + 2], m_max_margin[axis + 2]));
		UpdateIfAuto(m_style->m_size[axis].GetType(), m_size[axis], zm::Clamp(auto_size, m_min_size[axis], m_max_size[axis]));
	}

	void Element::ComputeRequiredSize(LayoutParams& layout, int16_t axis, Unit value, int16_t min, int16_t max, int16_t& result) {
		if (value.GetType() == Unit::Type::AUT) {
			layout.auto_params[axis].count++;
			layout.auto_params[axis].auto_max_sizes.push_back(max);
			layout.auto_params[axis].auto_min_sizes.push_back(min);
		}
		else {
			result = zm::Min(max, value.Get(layout.viewport_size, layout.parent_bounds.inner[axis], 0));
			layout.auto_params[axis].required_size += result;
		}
	}

	void Element::UpdateIfAuto(Unit::Type type, int16_t& value, int16_t auto_size) {
		if (type == Unit::Type::AUT)
			value = auto_size;
	}
}