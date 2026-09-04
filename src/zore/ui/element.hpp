#pragma once

#include "zore/ui/style.hpp"
#include "zore/math/vector/vec2.hpp"
#include "zore/math/vector/vec4.hpp"
#include <string>
#include <string_view>
#include <vector>

namespace zore::UI {

	class Element {
	public:
		enum class Type { PANEL, BUTTON, LABEL, SLIDER };
		using ID = uint32_t;

		struct Bounds {
			// Stored as width, height, x, y to match the W/H/X/Y UI indices.
			zm::ivec4 outer{};  // including margins
			zm::ivec4 middle{}; // the element itself
			zm::ivec4 inner{};  // content box (after padding)
		};

		struct Data {
			Data(Type type, const Style* style) : m_style(style), m_type(type) {}
			std::vector<ID> m_children;
			// Margins use the L/T/R/B indices; bounds use W/H/X/Y.
			zm::ivec4 m_min_margin{}, m_max_margin{}, m_margin{};
			zm::ivec2 m_min_size{}, m_max_size{}, m_size{};
			Bounds m_bounds{};
			std::string m_text;
			const Style* m_style;
			Type m_type;
		};

		struct Segment {
			int32_t min_size = 0, size = 0, max_size = 0;
			uint16_t auto_count = 0;
			void Add(Unit min_size, Unit size, Unit max_size, zm::sivec2 viewport_size, zm::sivec2 parent_size, int16_t axis, float scale = 1.f);
		};

		Element(Type type, const Style* style = nullptr);
		~Element() = default;

		Element Append(Type type, const Style* style);
		Element Append(Type type, std::string_view style = "");

		Element& SetStyle(const Style* style);
		Element& SetStyle(std::string_view style);
		Element& SetText(std::string_view text);

		ID GetID() const;
		Type GetType() const;
		const Style& GetStyle() const;
		const std::string& GetText() const;
		const Bounds& GetBounds() const;
		std::vector<Element> Children() const;

		void UpdateConstraints(zm::sivec2 viewport_size, zm::sivec2 parent_size);
		Segment GetSize(zm::sivec2 viewport_size, zm::sivec2 parent_size, uint8_t axis) const;

	protected:
		void Layout(zm::sivec2 viewport_size, const Bounds& bounds);

	private:
		struct AutoSlot {
			int32_t* value;
			int32_t min, max;
		};

	private:
		Element(ID id) : m_id(id) {}

		static void Distribute(int32_t available, std::vector<AutoSlot>& slots);
		static int32_t Resolve(Unit value, Unit minimum, Unit maximum, zm::sivec2 viewport, zm::sivec2 parent, uint8_t axis, bool& automatic);
		static void ResolveIndependentAxis(Data& data, zm::sivec2 viewport, zm::sivec2 parent, uint8_t axis);
		static int32_t ProportionalSize(const Data& data, uint8_t axis);

		void LayoutChildren(zm::sivec2 viewport_size);

	private:
		ID m_id = uint32_max;
	};
}
