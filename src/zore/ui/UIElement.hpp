#pragma once

#include <string>
#include <vector>

namespace zore {

	class UILayer;

	//========================================================================
	//	Unit Class
	//========================================================================

	class Unit {
	public:
		Unit() : m_unit_measurement(0) {};
		~Unit() = default;

		static Unit PCT(float value);
		static Unit REM(float value);
		uint16_t Get(uint16_t value = 0);

	private:
		Unit(uint16_t unit, uint16_t value);

	private:
		uint16_t m_unit_measurement;
	};

	//========================================================================
	//	Anchor Enum
	//========================================================================

	enum class Anchor {
		TOP_LEFT, TOP_CENTER, TOP_RIGHT, MID_LEFT, MID_CENTER, MID_RIGHT, BOT_LEFT, BOT_CENTER, BOT_RIGHT
	};

	//========================================================================
	//	Base UI Element Class
	//========================================================================

	class UIElement {
	public:
		UIElement();
		~UIElement() = default;

		void AddChild(const UIElement& element, const std::string& id = "");
		UIElement* GetElementByID(const std::string& id);

		UIElement& SetAnchor(Anchor anchor);
		UIElement& SetWidth(Unit width);
		UIElement& SetHeight(Unit height);
		UIElement& SetColour(float r, float g, float b, float a);

	protected:
		virtual void Initialize(UIElement* parent);
		void Update(uint32_t width, uint32_t height, uint32_t pos_x, uint32_t pos_y);

	protected:
		UILayer* m_layer;
		UIElement* m_parent;
		std::vector<UIElement> m_children;
		std::string m_id;
		uint32_t m_index;

		Unit m_width;
		Unit m_height;
		Anchor m_anchor;
	};
}