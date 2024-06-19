#include "zore/ui/UILayer.hpp"
#include "zore/Debug.hpp"

namespace zore {

	UILayer::UILayer() {
		SetWidth(Unit::PCT(100.f));
		SetHeight(Unit::PCT(100.f));
	}

	void UILayer::Initialize(UIElement* parent) {
		throw ZORE_EXCEPTION("A UI Layer cannot be added as a child to another UI Element.");
	}

	void UILayer::Update(uint32_t width, uint32_t height) {
		UIElement::Update(width, height, 0, 0);
	}
}