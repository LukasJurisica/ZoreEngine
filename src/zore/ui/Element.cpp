#include "zore/ui/Element.hpp"
#include "zore/utils/UUID.hpp"
#include "zore/math/MathUtils.hpp"
#include "zore/Debug.hpp"

namespace zore::UI {

	Element::Element(Type type, const std::string& style) : Element(type, Style::Get(style)) {}
	Element::Element(Type type, const Style* style) : m_type(type), m_style(style), m_id(luid::Generate()), m_size{ 0, 0 }, m_margin{ 0, 0, 0, 0 } {}

	Element* Element::AddChild(const Element& element) {
		m_children.push_back(element);
		return &m_children.back();
	}

	Element* Element::AddChild(const Element& element, const std::string& style) {
		m_children.push_back(element);
		m_children.back().SetStyle(style);
		return &m_children.back();
	}

	Element::Bounds Element::ComputeBounds(const Bounds& parent_bounds, int16_t global_height, int16_t auto_size, int16_t axis) const {
		// Compute size of element if any used auto specifier
		UpdateIfAuto(m_style->m_margin[axis + 0].GetType(), m_margin[axis + 0], auto_size);
		UpdateIfAuto(m_style->m_margin[axis + 2].GetType(), m_margin[axis + 2], auto_size);
		UpdateIfAuto(m_style->m_size[axis].GetType(), m_size[axis], auto_size);

		// Compute size of seconfary axis if it relies on the primary axis, and primary axis was auto
		if (m_style->m_dependent_axis == (1 - axis) and m_style->m_size[axis].GetType() == Unit::Type::AUT)
			ComputeSizeOfSecondaryAxis(parent_bounds, global_height, 1 - axis);

		Bounds bounds;

		// Compute the size and position of the middle content (self)
		bounds.middle[X] = parent_bounds.inner[X] + m_margin[L];
		bounds.middle[Y] = parent_bounds.inner[Y] + m_margin[T];
		bounds.middle[W] = m_size[W];
		bounds.middle[H] = m_size[H];

		// Compute the size and position of the outer content (self + margins)
		bounds.outer[X] = parent_bounds.inner[X];
		bounds.outer[Y] = parent_bounds.inner[Y];
		bounds.outer[W] = bounds.middle[W] + m_margin[L] + m_margin[R];
		bounds.outer[H] = bounds.middle[H] + m_margin[T] + m_margin[B];

		// Compute the size and position of the contained inner content (self - padding)
		int16_t padding_t = m_style->m_padding[T].Get(global_height, parent_bounds.inner[H], 0);
		int16_t padding_r = m_style->m_padding[R].Get(global_height, parent_bounds.inner[W], 0);
		int16_t padding_b = m_style->m_padding[B].Get(global_height, parent_bounds.inner[H], 0);
		int16_t padding_l = m_style->m_padding[L].Get(global_height, parent_bounds.inner[W], 0);
		bounds.inner[X] = bounds.middle[X] + padding_l;
		bounds.inner[Y] = bounds.middle[Y] + padding_t;
		bounds.inner[W] = bounds.middle[W] - padding_l - padding_r;
		bounds.inner[H] = bounds.middle[H] - padding_t - padding_b;

		return bounds;
	}

	void Element::ComputeRequiredSize(const Bounds& parent_bounds, int16_t global_height, int16_t& auto_count, int16_t& required_size, int16_t axis) const {
		m_max_size[W] = m_style->m_max_size[W].Get(global_height, parent_bounds.inner[W], 0);
		m_max_size[H] = m_style->m_max_size[H].Get(global_height, parent_bounds.inner[H], 0);

		// Compute size of element along primary axis if it is not dependent on the secondary axis
		if (m_style->m_dependent_axis != axis)
			ComputeRequiredSize(parent_bounds.inner[axis], global_height, m_max_size[axis], m_style->m_size[axis], auto_count, required_size, m_size[axis]);

		// Compute size of secondary axis if it does not rely on the primary axis, or the primary axis is calculable
		if (m_style->m_dependent_axis != (1 - axis) or m_style->m_size[axis].GetType() != Unit::Type::AUT) {
			ComputeSizeOfSecondaryAxis(parent_bounds, global_height, 1 - axis);
			if (m_style->m_dependent_axis == axis) {
				m_size[axis] = static_cast<int16_t>(zm::Round(m_size[1 - axis] * m_style->m_aspect_ratio));
				required_size += m_size[axis];
			}
		}

		// Compute size of margins on primary axis
		ComputeRequiredSize(parent_bounds.inner[axis], global_height, parent_bounds.inner[axis], m_style->m_margin[axis + 0], auto_count, required_size, m_margin[axis + 0]);
		ComputeRequiredSize(parent_bounds.inner[axis], global_height, parent_bounds.inner[axis], m_style->m_margin[axis + 2], auto_count, required_size, m_margin[axis + 2]);
	}

	void Element::ComputeSizeOfSecondaryAxis(const Bounds& parent_bounds, int16_t global_height, int16_t axis) const {
		int16_t auto_count = 0, required_size = 0;
		ComputeRequiredSize(parent_bounds.inner[axis], global_height, parent_bounds.inner[axis], m_style->m_margin[axis + 0], auto_count, required_size, m_margin[axis + 0]);
		ComputeRequiredSize(parent_bounds.inner[axis], global_height, parent_bounds.inner[axis], m_style->m_margin[axis + 2], auto_count, required_size, m_margin[axis + 2]);
		if (m_style->m_dependent_axis == axis)
			required_size += (m_size[axis] = zm::Min(m_max_size[axis], static_cast<int16_t>(zm::Round(m_size[1 - axis] * m_style->m_aspect_ratio))));
		else
			ComputeRequiredSize(parent_bounds.inner[axis], global_height, m_max_size[axis], m_style->m_size[axis], auto_count, required_size, m_size[axis]);
		int16_t auto_size = auto_count > 0 ? ((parent_bounds.inner[axis] - required_size) / auto_count) : 0;
		UpdateIfAuto(m_style->m_margin[axis + 0].GetType(), m_margin[axis + 0], auto_size);
		UpdateIfAuto(m_style->m_margin[axis + 2].GetType(), m_margin[axis + 2], auto_size);
		UpdateIfAuto(m_style->m_size[axis].GetType(), m_size[axis], zm::Min(m_max_size[axis], auto_size));
	}

	void Element::ComputeRequiredSize(int16_t parent_size, int16_t global_height, int16_t max_size, Unit value, int16_t& auto_count, int16_t& required_size, int16_t& result) const {
		if (value.GetType() == Unit::Type::AUT)
			auto_count++;
		else
			required_size += (result = zm::Min(max_size, value.Get(global_height, parent_size, 0)));
	}

	void Element::UpdateIfAuto(Unit::Type type, int16_t& value, int16_t auto_size) const {
		if (type == Unit::Type::AUT)
			value = auto_size;
	}
}