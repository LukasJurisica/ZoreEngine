#pragma once

#include "zore/ui/style.hpp"
#include "zore/utils/memory.hpp"
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
		Element(Type type, const Style* style);
		Element(Type type, const std::string& style = "");
		Element(const Element&) = delete;
		Element(Element&&) = delete;
		Element& operator=(const Element&) = delete;
		Element& operator=(Element&&) = delete;
		~Element() = default;

		Element& AddChild(Type type, const Style* style);
		Element& AddChild(Type type, const std::string& style = "");
		Element& SetStyle(const Style* style);
		Element& SetStyle(const std::string& style);
		Element& SetText(const std::string& text);

		inline Type GetType() const { return m_type; }
		inline uint32_t GetUUID() const { return m_id; }
		inline const std::string& GetText() const { return m_text; }
		inline const Style* GetStyle() const { return m_style; }
		inline std::vector<UNIQUE<Element>>& Children() { return m_children; }

		Bounds ComputeBounds(LayoutParams& layout, int16_t auto_size);
		void ComputeRequiredSize(LayoutParams& layout, int16_t axis);

	private:
		void ComputeSizeOfSecondaryAxis(LayoutParams& layout);
		void ComputeRequiredSize(LayoutParams& layout, int16_t axis, Unit value, int16_t min, int16_t max, int16_t& result);
		void UpdateIfAuto(Unit::Type type, int16_t& value, int16_t auto_size);
		void ComputeProportionalSize(LayoutParams& layout, int16_t axis);

	protected:
		std::string m_text;
		std::vector<UNIQUE<Element>> m_children;
		Type m_type;
		const Style* m_style;
		uint32_t m_id;
		int16_t m_min_margin[4];
		int16_t m_max_margin[4];
		int16_t m_margin[4];
		int16_t m_min_size[2];
		int16_t m_max_size[2];
		int16_t m_size[2];
	};
}