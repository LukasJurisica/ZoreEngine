#include "zore/ui/style.hpp"
#include "zore/math/math.hpp"
#include "zore/structures/string_unordered_map.hpp"
#include "zore/debug.hpp"

namespace zore::UI {

	//========================================================================
	//	Unit Class
	//========================================================================

	int16_t Unit::Get(const zm::sivec2& viewport_size, const zm::sivec2& parent_size, int16_t auto_size, int16_t axis) const {
		switch (m_type) {
		case Type::AUTO:
			return auto_size;
		case Type::VW:
			return UnpackPercentage(viewport_size[W], m_value);
		case Type::VH:
			return UnpackPercentage(viewport_size[H], m_value);
		case Type::PW:
			return UnpackPercentage(parent_size[W], m_value);
		case Type::PH:
			return UnpackPercentage(parent_size[H], m_value);
		case Type::PC:
			return UnpackPercentage(parent_size[axis], m_value);
		case Type::PX:
			return m_value;
		default:
			return 0;
		}
	}

	int16_t Unit::PackPercentage(float value) {
		static constexpr float upper_bound = int16_max / 100.f;
		return static_cast<int16_t>(upper_bound * zm::Clamp(value, 0.f, 100.f) + 0.5f);
	}

	int16_t Unit::UnpackPercentage(int16_t driver, int16_t value) {
		static constexpr float recip = 1.f / int16_max;
		return static_cast<int16_t>(driver * recip * value + 0.5f);
	}

	//========================================================================
	//	Style Class
	//========================================================================

	static zore::string_unordered_map<Style> s_styles;

	Style::Style() {
		SetSize(Unit::PC(100), Unit::PC(100));
		SetMinSize(Unit::PX(0), Unit::PX(0));
		SetMaxSize(Unit::PX(int16_max), Unit::PX(int16_max));
		SetMargin(Unit::PX(0));
		SetMinMargin(Unit::PX(0));
		SetMaxMargin(Unit::PX(int16_max));
		SetPadding(Unit::PX(0));
		SetMinPadding(Unit::PX(0));
		SetMaxPadding(Unit::PX(int16_max));
		SetGap(Unit::PX(0));
		SetMinGap(Unit::PX(0));
		SetMaxGap(Unit::PX(int16_max));
		SetFlowDirection(Axis::VERTICAL);
		SetScrollAxis(Axis::NONE);
		SetColour(Colour(0x00000000));
	}

	Style& Style::Create(std::string_view name) {
		return s_styles.insert({ std::string(name), Style() }).first->second;
	}

	Style& Style::Clone(const Style& style, std::string_view new_name) {
		return s_styles.insert({ std::string(new_name), style }).first->second;
	}

	Style& Style::Get(std::string_view name) {
		auto iter = s_styles.find(name);
		if (iter != s_styles.end())
			return iter->second;
		else if (name != "")
			Logger::Warn("Requested UI Style doesn't exist:", name);
		static Style default_style;
		return default_style;
	}

	Style& Style::SetWidth(Unit width) {
		m_size[W] = width;
		m_dependent_axis = 2;
		m_text_warped = false;
		return *this;
	}

	Style& Style::SetHeight(Unit height) {
		m_size[H] = height;
		m_dependent_axis = 2;
		m_text_warped = false;
		return *this;
	}

	Style& Style::SetWidth(Unit width, float aspect_ratio) {
		m_size[W] = width;
		m_aspect_ratio = aspect_ratio;
		m_dependent_axis = 1;
		m_text_warped = true;
		return *this;
	}

	Style& Style::SetHeight(Unit height, float aspect_ratio) {
		m_size[H] = height;
		m_aspect_ratio = aspect_ratio;
		m_dependent_axis = 0;
		m_text_warped = true;
		return *this;
	}

	Style& Style::SetSize(Unit width, Unit height) {
		m_size[W] = width;
		m_size[H] = height;
		m_dependent_axis = 2;
		m_text_warped = true;
		return *this;
	}

	Style& Style::SetMinWidth(Unit width) {
		ENSURE(width.GetType() != Unit::Type::AUTO, "Minimum size cannot be auto");
		m_min_size[W] = width;
		return *this;
	}

	Style& Style::SetMinHeight(Unit height) {
		ENSURE(height.GetType() != Unit::Type::AUTO, "Minimum size cannot be auto");
		m_min_size[H] = height;
		return *this;
	}

	Style& Style::SetMinSize(Unit width, Unit height) {
		ENSURE(width.GetType() != Unit::Type::AUTO, "Minimum size cannot be auto");
		ENSURE(height.GetType() != Unit::Type::AUTO, "Minimum size cannot be auto");
		m_min_size[W] = width;
		m_min_size[H] = height;
		return *this;
	}

	Style& Style::SetMaxWidth(Unit width) {
		ENSURE(width.GetType() != Unit::Type::AUTO, "Maximum size cannot be auto");
		m_max_size[W] = width;
		return *this;
	}

	Style& Style::SetMaxHeight(Unit height) {
		ENSURE(height.GetType() != Unit::Type::AUTO, "Maximum size cannot be auto");
		m_max_size[H] = height;
		return *this;
	}

	Style& Style::SetMaxSize(Unit width, Unit height) {
		ENSURE(width.GetType() != Unit::Type::AUTO, "Maximum size cannot be auto");
		ENSURE(height.GetType() != Unit::Type::AUTO, "Maximum size cannot be auto");
		m_max_size[W] = width;
		m_max_size[H] = height;
		return *this;
	}

	Style& Style::SetMargin(Unit margin) {
		m_margin[T] = margin;
		m_margin[R] = margin;
		m_margin[B] = margin;
		m_margin[L] = margin;
		return *this;
	}

	Style& Style::SetMargin(Unit horizontal, Unit vertical) {
		m_margin[T] = vertical;
		m_margin[R] = horizontal;
		m_margin[B] = vertical;
		m_margin[L] = horizontal;
		return *this;
	}

	Style& Style::SetMargin(Unit left, Unit top, Unit right, Unit bottom) {
		m_margin[T] = top;
		m_margin[R] = right;
		m_margin[B] = bottom;
		m_margin[L] = left;
		return *this;
	}

	Style& Style::SetMinMargin(Unit margin) {
		ENSURE(margin.GetType() != Unit::Type::AUTO, "Minimum margin cannot be auto");
		m_min_margin[T] = margin;
		m_min_margin[R] = margin;
		m_min_margin[B] = margin;
		m_min_margin[L] = margin;
		return *this;
	}

	Style& Style::SetMinMargin(Unit horizontal, Unit vertical) {
		ENSURE(vertical.GetType() != Unit::Type::AUTO, "Minimum margin cannot be auto");
		ENSURE(horizontal.GetType() != Unit::Type::AUTO, "Minimum margin cannot be auto");
		m_min_margin[T] = vertical;
		m_min_margin[R] = horizontal;
		m_min_margin[B] = vertical;
		m_min_margin[L] = horizontal;
		return *this;
	}

	Style& Style::SetMinMargin(Unit left, Unit top, Unit right, Unit bottom) {
		ENSURE(left.GetType() != Unit::Type::AUTO, "Minimum margin cannot be auto");
		ENSURE(top.GetType() != Unit::Type::AUTO, "Minimum margin cannot be auto");
		ENSURE(right.GetType() != Unit::Type::AUTO, "Minimum margin cannot be auto");
		ENSURE(bottom.GetType() != Unit::Type::AUTO, "Minimum margin cannot be auto");
		m_min_margin[T] = top;
		m_min_margin[R] = right;
		m_min_margin[B] = bottom;
		m_min_margin[L] = left;
		return *this;
	}

	Style& Style::SetMaxMargin(Unit margin) {
		m_max_margin[T] = margin;
		m_max_margin[R] = margin;
		m_max_margin[B] = margin;
		m_max_margin[L] = margin;
		return *this;
	}

	Style& Style::SetMaxMargin(Unit horizontal, Unit vertical) {
		m_max_margin[T] = vertical;
		m_max_margin[R] = horizontal;
		m_max_margin[B] = vertical;
		m_max_margin[L] = horizontal;
		return *this;
	}

	Style& Style::SetMaxMargin(Unit left, Unit top, Unit right, Unit bottom) {
		ENSURE(left.GetType() != Unit::Type::AUTO, "Maximum margin cannot be auto");
		ENSURE(top.GetType() != Unit::Type::AUTO, "Maximum margin cannot be auto");
		ENSURE(right.GetType() != Unit::Type::AUTO, "Maximum margin cannot be auto");
		ENSURE(bottom.GetType() != Unit::Type::AUTO, "Maximum margin cannot be auto");
		m_max_margin[T] = top;
		m_max_margin[R] = right;
		m_max_margin[B] = bottom;
		m_max_margin[L] = left;
		return *this;
	}

	Style& Style::SetPadding(Unit padding) {
		m_padding[T] = padding;
		m_padding[R] = padding;
		m_padding[B] = padding;
		m_padding[L] = padding;
		return *this;
	}

	Style& Style::SetPadding(Unit horizontal, Unit vertical) {
		m_padding[T] = vertical;
		m_padding[R] = horizontal;
		m_padding[B] = vertical;
		m_padding[L] = horizontal;
		return *this;
	}

	Style& Style::SetPadding(Unit left, Unit top, Unit right, Unit bottom) {
		m_padding[T] = top;
		m_padding[R] = right;
		m_padding[B] = bottom;
		m_padding[L] = left;
		return *this;
	}

	Style& Style::SetMinPadding(Unit padding) {
		m_min_padding[T] = padding;
		m_min_padding[R] = padding;
		m_min_padding[B] = padding;
		m_min_padding[L] = padding;
		return *this;
	}

	Style& Style::SetMinPadding(Unit horizontal, Unit vertical) {
		m_min_padding[T] = vertical;
		m_min_padding[R] = horizontal;
		m_min_padding[B] = vertical;
		m_min_padding[L] = horizontal;
		return *this;
	}

	Style& Style::SetMinPadding(Unit left, Unit top, Unit right, Unit bottom) {
		m_min_padding[T] = top;
		m_min_padding[R] = right;
		m_min_padding[B] = bottom;
		m_min_padding[L] = left;
		return *this;
	}

	Style& Style::SetMaxPadding(Unit padding) {
		m_max_padding[T] = padding;
		m_max_padding[R] = padding;
		m_max_padding[B] = padding;
		m_max_padding[L] = padding;
		return *this;
	}

	Style& Style::SetMaxPadding(Unit horizontal, Unit vertical) {
		m_max_padding[T] = vertical;
		m_max_padding[R] = horizontal;
		m_max_padding[B] = vertical;
		m_max_padding[L] = horizontal;
		return *this;
	}

	Style& Style::SetMaxPadding(Unit left, Unit top, Unit right, Unit bottom) {
		m_max_padding[T] = top;
		m_max_padding[R] = right;
		m_max_padding[B] = bottom;
		m_max_padding[L] = left;
		return *this;
	}

	Style& Style::SetGap(Unit gap) {
		m_gap[W] = gap;
		m_gap[H] = gap;
		return *this;
	}

	Style& Style::SetGap(Unit horizontal, Unit vertical) {
		m_gap[W] = horizontal;
		m_gap[H] = vertical;
		return *this;
	}

	Style& Style::SetMinGap(Unit gap) {
		m_min_gap[W] = gap;
		m_min_gap[H] = gap;
		return *this;
	}

	Style& Style::SetMinGap(Unit horizontal, Unit vertical) {
		m_min_gap[W] = horizontal;
		m_min_gap[H] = vertical;
		return *this;
	}

	Style& Style::SetMaxGap(Unit gap) {
		m_max_gap[W] = gap;
		m_max_gap[H] = gap;
		return *this;
	}

	Style& Style::SetMaxGap(Unit horizontal, Unit vertical) {
		m_max_gap[W] = horizontal;
		m_max_gap[H] = vertical;
		return *this;
	}

	Style& Style::SetFlowDirection(Axis flow_direction) {
		m_flow_direction = flow_direction;
		return *this;
	}

	Style& Style::SetScrollAxis(Axis scroll_axis) {
		m_scroll_axis = scroll_axis;
		return *this;
	}

	Style& Style::SetColour(Colour colour) {
		m_colour = colour;
		return *this;
	}
}