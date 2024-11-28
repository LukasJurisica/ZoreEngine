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
		enum class Type : uint16_t { PCT, REM, PXL, AUT };

	public:
		Unit() : m_type(Type::PCT), m_value(0) {};
		~Unit() = default;

		static Unit PCT(float value);
		static Unit REM(float value);
		static Unit PXL(int16_t value);
		static Unit AUT();

		Type GetType() const { return m_type; }
		int16_t Get(int16_t global_size, int16_t parent_size, int16_t auto_size) const;

	private:
		Unit(Type type, int16_t value) : m_type(type), m_value(value) {};
		static int16_t PercentageAsInt(float value);

	private:
		int16_t m_value;
		Type m_type;
	};

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
		static Style& GetDefaultStyle();

		Style& SetWidth(Unit width, float aspect_ratio);
		Style& SetHeight(Unit height, float aspect_ratio);
		Style& SetSize(Unit width, Unit height);
		Style& SetMaxWidth(Unit width);
		Style& SetMaxHeight(Unit height);
		Style& SetMaxSize(Unit width, Unit height);
		Style& SetMargin(Unit margin);
		Style& SetMargin(Unit horizontal, Unit vertical);
		Style& SetMargin(Unit left, Unit top, Unit right, Unit bottom);
		Style& SetPadding(Unit padding);
		Style& SetPadding(Unit horizontal, Unit vertical);
		Style& SetPadding(Unit left, Unit top, Unit right, Unit bottom);
		Style& SetFlowDirection(FlowDirection flow_direction);
		Style& SetColour(Colour colour);

	public:
		float m_aspect_ratio;
		uint8_t m_dependent_axis;
		Unit m_size[2];
		Unit m_max_size[2];
		Unit m_margin[4];
		Unit m_padding[4];
		FlowDirection m_flow_direction;
		Colour m_colour;
	};
}