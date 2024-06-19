#include "zore/ui/UILayer.hpp"
#include "zore/ui/UIElement.hpp"
#include "zore/math/MathUtils.hpp"
#include "zore/Debug.hpp"

namespace zore {

	//========================================================================
	//	Unit Class
	//========================================================================

	Unit Unit::PCT(float value) {
		value = zm::Clamp(value, 0.f, 100.f);
		static constexpr float upper_bound = 32767.f / 100.f;
		return Unit(0, static_cast<uint16_t>(upper_bound * value));
	}

	Unit Unit::REM(float value) {
		value = zm::Clamp(value, 0.f, 32767.f);
		return Unit(1, static_cast<uint16_t>(value));
	}

	uint16_t Unit::Get(uint16_t value) {
		switch (m_unit_measurement & 1) {
		case 0: return static_cast<uint16_t>(((m_unit_measurement >> 1) / 32767.f) * value);
		case 1: return m_unit_measurement >> 1;
		default: return 0;
		}
	}

	Unit::Unit(uint16_t unit, uint16_t value) : m_unit_measurement((value << 1) | unit) {}

	//========================================================================
	//	Base UI Element Class
	//========================================================================

	static constexpr uint32_t INVALID_UI_ELEMENT_INDEX = ~0;

	UIElement::UIElement() : m_layer(nullptr), m_parent(nullptr), m_index(INVALID_UI_ELEMENT_INDEX) {
		SetAnchor(Anchor::TOP_LEFT);
	}

	void UIElement::AddChild(const UIElement& element, const std::string& id) {
		m_children.push_back(element);
		m_children.back().m_id = id;
		m_children.back().Initialize(this);
	}

	UIElement* UIElement::GetElementByID(const std::string& id) {
		return this;
	}

	UIElement& UIElement::SetAnchor(Anchor anchor) {
		m_anchor = anchor;
		return *this;
	}

	UIElement& UIElement::SetWidth(Unit width) {
		m_width = width;
		return *this;
	}

	UIElement& UIElement::SetHeight(Unit height) {
		m_height = height;
		return *this;
	}

	void UIElement::Initialize(UIElement* parent) {
		if (!parent->m_layer)
			return;
		m_parent = parent;
		m_layer = parent->m_layer;
		for (UIElement& child : m_children)
			child.Initialize(this);
	}

	void UIElement::Update(uint32_t width, uint32_t height, uint32_t pos_x, uint32_t pos_y) {
		Logger::Log(TOSTR(width) + " " + TOSTR(height));
	}
}