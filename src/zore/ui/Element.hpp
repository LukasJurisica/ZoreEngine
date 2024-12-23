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
		public:
			int16_t outer[4];
			int16_t middle[4];
			int16_t inner[4];
		};

		struct AutoParams {
		public:
			int16_t GetAutoSize(int16_t parent_size);

		public:
			std::vector<int16_t> auto_max_sizes;
			std::vector<int16_t> auto_min_sizes;
			int16_t required_size = 0;
			int16_t count = 0;
		};

		struct LayoutParams {
		public:
			LayoutParams(const Bounds& bounds, int16_t viewport_width, int16_t viewport_height, FlowDirection direction);
			int16_t GetAutoSize(int16_t axis) { return auto_params[axis].GetAutoSize(parent_bounds.inner[axis]); }

		public:
			const Bounds& parent_bounds;
			AutoParams auto_params[2];
			int16_t viewport_size[2];
			int16_t flow_axis;
		};

	public:
		Element(Type type, const std::string& style = "");
		Element(Type type, const Style* style);

		Element* AddChild(const Element& element);
		Element* AddChild(const Element& element, const std::string& style);
		Bounds ComputeBounds(LayoutParams& layout, int16_t auto_size);
		void ComputeRequiredSize(LayoutParams& layout, int16_t axis);

		inline void SetStyle(const Style* style) { m_style = style; }
		inline void SetStyle(const std::string& style) { m_style = Style::Get(style); }
		inline Type GetType() const { return m_type; }
		inline uint32_t GetUUID() const { return m_id; }
		inline const Style* GetStyle() const { return m_style; }
		inline Element* GetChild(uint32_t index) { return (index >= m_children.size()) ? nullptr : &m_children[index]; }
		inline std::vector<Element>& Children() { return m_children; }

	private:
		void ComputeSizeOfSecondaryAxis(LayoutParams& layout);
		void ComputeRequiredSize(LayoutParams& layout, int16_t axis, Unit value, int16_t min, int16_t max, int16_t& result);
		void UpdateIfAuto(Unit::Type type, int16_t& value, int16_t auto_size);

	protected:
		Type m_type;
		uint32_t m_id;
		const Style* m_style;
		int16_t m_size[2];
		int16_t m_min_size[2];
		int16_t m_max_size[2];
		int16_t m_margin[4];
		int16_t m_min_margin[4];
		int16_t m_max_margin[4];
		std::vector<Element> m_children;
	};
}