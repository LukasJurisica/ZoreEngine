#pragma once

#include "zore/utils/DataTypes.hpp"
#include "zore/utils/Colour.hpp"
#include <string>

namespace zore::UI {

	//========================================================================
	//	Constants
	//========================================================================

	static inline constexpr uint8_t L = 0; // Left
	static inline constexpr uint8_t T = 1; // Top
	static inline constexpr uint8_t R = 2; // Right
	static inline constexpr uint8_t B = 3; // Bottom

	static inline constexpr uint8_t W = 0; // Width
	static inline constexpr uint8_t H = 1; // Height
	static inline constexpr uint8_t X = 2; // X
	static inline constexpr uint8_t Y = 3; // Y

	//========================================================================
	//	Unit Class
	//========================================================================

	class Unit {
	public:
		enum class Type : uint16_t { AUTO, VW, VH, PW, PH, PC, PX };

	public:
		Unit() : m_type(Type::PC), m_value(0) {};
		~Unit() = default;

		static inline constexpr Unit AUTO() { return Unit(Type::AUTO, 0); }
		static inline constexpr Unit VW(float value) { return Unit(Type::VW, PercentageAsInt(value)); }
		static inline constexpr Unit VH(float value) { return Unit(Type::VH, PercentageAsInt(value)); }
		static inline constexpr Unit PW(float value) { return Unit(Type::PW, PercentageAsInt(value)); }
		static inline constexpr Unit PH(float value) { return Unit(Type::PH, PercentageAsInt(value)); }
		static inline constexpr Unit PC(float value) { return Unit(Type::PC, PercentageAsInt(value)); }
		static inline constexpr Unit PX(int16_t value) { return Unit(Type::PX, value); }

		Type GetType() const { return m_type; }
		int16_t Get(const int16_t* viewport_size, const int16_t* parent_size, int16_t auto_size, int16_t axis) const;

	private:
		constexpr Unit(Type type, int16_t value) : m_type(type), m_value(value) {};
		static int16_t PercentageAsInt(float value);

	private:
		int16_t m_value;
		Type m_type;
	};

	namespace literals {
		constexpr Unit operator"" _vw(long double value) noexcept { return Unit::VW(static_cast<float>(value)); }
		constexpr Unit operator"" _vw(unsigned long long int value) noexcept { return Unit::VW(static_cast<float>(value)); }
		constexpr Unit operator"" _vh(long double value) noexcept { return Unit::VH(static_cast<float>(value)); }
		constexpr Unit operator"" _vh(unsigned long long int value) noexcept { return Unit::VH(static_cast<float>(value)); }
		constexpr Unit operator"" _pw(long double value) noexcept { return Unit::PW(static_cast<float>(value)); }
		constexpr Unit operator"" _pw(unsigned long long int value) noexcept { return Unit::PW(static_cast<float>(value)); }
		constexpr Unit operator"" _ph(long double value) noexcept { return Unit::PH(static_cast<float>(value)); }
		constexpr Unit operator"" _ph(unsigned long long int value) noexcept { return Unit::PH(static_cast<float>(value)); }
		constexpr Unit operator"" _pc(long double value) noexcept { return Unit::PC(static_cast<float>(value)); }
		constexpr Unit operator"" _pc(unsigned long long int value) noexcept { return Unit::PC(static_cast<float>(value)); }
		constexpr Unit operator"" _px(unsigned long long int value) noexcept { return Unit::PX(static_cast<uint16_t>(value)); }
		constexpr Unit AUTO() { return Unit::AUTO(); }
	}

	//========================================================================
	//	Positioning Enums
	//========================================================================

	enum class FlowDirection : uint8_t { HORIZONTAL, VERTICAL };

	//========================================================================
	//	Style Class
	//========================================================================

	class Style {
	public:
		Style();
		static Style& Create(const std::string& name);
		static Style& Clone(const Style& style, const std::string& new_name);
		static Style* Get(const std::string& name);

		Style& SetWidth(Unit width);
		Style& SetHeight(Unit height);
		Style& SetWidth(Unit width, float aspect_ratio);
		Style& SetHeight(Unit height, float aspect_ratio);
		Style& SetSize(Unit width, Unit height);
		Style& SetMinWidth(Unit width);
		Style& SetMinHeight(Unit height);
		Style& SetMinSize(Unit width, Unit height);
		Style& SetMaxWidth(Unit width);
		Style& SetMaxHeight(Unit height);
		Style& SetMaxSize(Unit width, Unit height);

		Style& SetMargin(Unit margin);
		Style& SetMargin(Unit horizontal, Unit vertical);
		Style& SetMargin(Unit left, Unit top, Unit right, Unit bottom);
		Style& SetMinMargin(Unit margin);
		Style& SetMinMargin(Unit horizontal, Unit vertical);
		Style& SetMinMargin(Unit left, Unit top, Unit right, Unit bottom);
		Style& SetMaxMargin(Unit margin);
		Style& SetMaxMargin(Unit horizontal, Unit vertical);
		Style& SetMaxMargin(Unit left, Unit top, Unit right, Unit bottom);

		Style& SetPadding(Unit padding);
		Style& SetPadding(Unit horizontal, Unit vertical);
		Style& SetPadding(Unit left, Unit top, Unit right, Unit bottom);
		Style& SetMinPadding(Unit padding);
		Style& SetMinPadding(Unit horizontal, Unit vertical);
		Style& SetMinPadding(Unit left, Unit top, Unit right, Unit bottom);
		Style& SetMaxPadding(Unit padding);
		Style& SetMaxPadding(Unit horizontal, Unit vertical);
		Style& SetMaxPadding(Unit left, Unit top, Unit right, Unit bottom);

		Style& SetFlowDirection(FlowDirection flow_direction);
		Style& SetGap(Unit gap);
		Style& SetGap(Unit horizontal, Unit vertical);
		Style& SetMinGap(Unit gap);
		Style& SetMinGap(Unit horizontal, Unit vertical);
		Style& SetMaxGap(Unit gap);
		Style& SetMaxGap(Unit horizontal, Unit vertical);
		Style& SetColour(Colour colour);

	public:
		Unit m_size[2];
		Unit m_min_size[2];
		Unit m_max_size[2];
		Unit m_margin[4];
		Unit m_min_margin[4];
		Unit m_max_margin[4];
		Unit m_padding[4];
		Unit m_min_padding[4];
		Unit m_max_padding[4];
		Unit m_gap[2];
		Unit m_min_gap[2];
		Unit m_max_gap[2];
		Colour m_colour;
		float m_aspect_ratio;
		bool m_text_scaled;
		uint8_t m_dependent_axis;
		FlowDirection m_flow_direction;
	};
}