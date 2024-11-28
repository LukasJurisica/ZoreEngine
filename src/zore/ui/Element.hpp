#pragma once

#include "zore/ui/Style.hpp"
#include <vector>

namespace zore::UI {

	//========================================================================
	//	Element Class
	//========================================================================

	class Element {
	public:
		enum class Type { PANEL, BUTTON, LABEL, SLIDER };
		struct Bounds {
			int16_t outer[4];
			int16_t middle[4];
			int16_t inner[4];
		};

	public:
		Element(Type type, const std::string& style = "");
		Element(Type type, const Style* style);

		Element* AddChild(const Element& element);
		Element* AddChild(const Element& element, const std::string& style);
		Bounds ComputeBounds(const Bounds& parent_bounds, int16_t global_height, int16_t auto_size, int16_t axis) const;
		void ComputeRequiredSize(const Bounds& parent_bounds, int16_t global_height, int16_t& auto_count, int16_t& required_size, int16_t axis) const;

		inline void SetStyle(const Style* style) { m_style = style; }
		inline void SetStyle(const std::string& style) { m_style = Style::Get(style); }
		inline Type GetType() const { return m_type; }
		inline uint32_t GetUUID() const { return m_id; }
		inline const Style* GetStyle() const { return m_style; }
		inline Element* GetChild(uint32_t index) { return (index >= m_children.size()) ? nullptr : &m_children[index]; }
		inline const std::vector<Element>& GetChildren() const { return m_children; }

	private:
		void ComputeSizeOfSecondaryAxis(const Bounds& parent_bounds, int16_t global_height, int16_t axis) const;
		void ComputeRequiredSize(int16_t parent_size, int16_t global_height, int16_t max_size, Unit value, int16_t& auto_count, int16_t& required_size, int16_t& result) const;
		void UpdateIfAuto(Unit::Type type, int16_t& value, int16_t auto_size) const;

	protected:
		Type m_type;
		uint32_t m_id;
		const Style* m_style;
		mutable int16_t m_size[2];
		mutable int16_t m_max_size[2];
		mutable int16_t m_margin[4];
		std::vector<Element> m_children;
	};
}