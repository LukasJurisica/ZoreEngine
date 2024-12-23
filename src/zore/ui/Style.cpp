#include "zore/ui/Style.hpp"
#include "zore/math/MathUtils.hpp"
#include "zore/Debug.hpp"

#include <unordered_map>

namespace zore::UI {

#define INT16_MAX_VALUE 32767

	//========================================================================
	//	Unit Class
	//========================================================================

	Unit Unit::AUT() {
		return Unit(Type::AUT, 0);
	}

	Unit Unit::VPW(float value) {
		return Unit(Type::VPW, PercentageAsInt(value));
	}

	Unit Unit::VPH(float value) {
		return Unit(Type::VPH, PercentageAsInt(value));
	}

	Unit Unit::PCT(float value) {
		return Unit(Type::PCT, PercentageAsInt(value));
	}

	Unit Unit::PXL(int16_t value) {
		return Unit(Type::PXL, value);
	}

	int16_t Unit::Get(int16_t viewport_size[2], int16_t parent_size, int16_t auto_size) const {
		constexpr float recip = 1.f / INT16_MAX_VALUE;
		switch (m_type) {
		case Type::AUT:
			return auto_size;
		case Type::VPW:
			return static_cast<int16_t>(viewport_size[W] * recip * m_value + 0.5f);
		case Type::VPH:
			return static_cast<int16_t>(viewport_size[H] * recip * m_value + 0.5f);
		case Type::PCT:
			return static_cast<int16_t>(parent_size * recip * m_value + 0.5f);
		case Type::PXL:
			return m_value;
		default:
			return 0;
		}
	}

	int16_t Unit::PercentageAsInt(float value) {
		value = zm::Clamp(value, 0.f, 100.f);
		static constexpr float upper_bound = INT16_MAX_VALUE / 100.f;
		return static_cast<int16_t>(zm::Round(upper_bound * value));
	}

	//========================================================================
	//	Style Class
	//========================================================================

	static std::unordered_map<std::string, Style> s_styles;

	Style::Style() {
		SetSize(Unit::PCT(100), Unit::PCT(100));
		SetMinSize(Unit::PXL(0), Unit::PXL(0));
		SetMaxSize(Unit::PXL(INT16_MAX_VALUE), Unit::PXL(INT16_MAX_VALUE));
		SetMargin(Unit::PXL(0));
		SetMinMargin(Unit::PXL(0));
		SetMaxMargin(Unit::PXL(INT16_MAX_VALUE));
		SetPadding(Unit::PXL(0));
		SetMinPadding(Unit::PXL(0));
		SetMaxPadding(Unit::PXL(INT16_MAX_VALUE));
		SetFlowDirection(FlowDirection::VERTICAL);
		SetColour(Colour::rgb(0, 0, 0, 0));
	}

	Style& Style::Create(const std::string & name) {
		return s_styles.insert({ name, Style() }).first->second;
	}

	Style& Style::Clone(const Style& style, const std::string& new_name) {
		return s_styles.insert({ new_name, style }).first->second;
	}

	Style* Style::Get(const std::string& name) {
		auto iter = s_styles.find(name);
		if (iter != s_styles.end())
			return &iter->second;
		else if (name != "")
			Logger::Warn("Attempted to use UI Style that doesn't exist:", name);
		return &GetDefaultStyle();
	}

	Style& Style::GetDefaultStyle() {
		static Style style;
		return style;
	}

	Style& Style::SetWidth(Unit width, float aspect_ratio) {
		m_size[W] = width;
		m_size[H] = Unit::PXL(0);
		m_aspect_ratio = aspect_ratio;
		m_dependent_axis = 1;
		return *this;
	}

	Style& Style::SetHeight(Unit height, float aspect_ratio) {
		m_size[W] = Unit::PXL(0);
		m_size[H] = height;
		m_aspect_ratio = aspect_ratio;
		m_dependent_axis = 0;
		return *this;
	}

	Style& Style::SetSize(Unit width, Unit height) {
		m_size[W] = width;
		m_size[H] = height;
		m_aspect_ratio = 0.f;
		m_dependent_axis = 2;
		return *this;
	}

	Style& Style::SetMinWidth(Unit width) {
		m_min_size[W] = width;
		return *this;
	}

	Style& Style::SetMinHeight(Unit height) {
		m_min_size[H] = height;
		return *this;
	}

	Style& Style::SetMinSize(Unit width, Unit height) {
		m_min_size[W] = width;
		m_min_size[H] = height;
		return *this;
	}

	Style& Style::SetMaxWidth(Unit width) {
		m_max_size[W] = width;
		return *this;
	}

	Style& Style::SetMaxHeight(Unit height) {
		m_max_size[H] = height;
		return *this;
	}

	Style& Style::SetMaxSize(Unit width, Unit height) {
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
		m_min_margin[T] = margin;
		m_min_margin[R] = margin;
		m_min_margin[B] = margin;
		m_min_margin[L] = margin;
		return *this;
	}

	Style& Style::SetMinMargin(Unit horizontal, Unit vertical) {
		m_min_margin[T] = vertical;
		m_min_margin[R] = horizontal;
		m_min_margin[B] = vertical;
		m_min_margin[L] = horizontal;
		return *this;
	}

	Style& Style::SetMinMargin(Unit left, Unit top, Unit right, Unit bottom) {
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

	Style& Style::SetFlowDirection(FlowDirection flow_direction) {
		m_flow_direction = flow_direction;
		return *this;
	}

	Style& Style::SetColour(Colour colour) {
		m_colour = colour;
		return *this;
	}
}