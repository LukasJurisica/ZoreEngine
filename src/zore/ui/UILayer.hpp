#pragma once

#include "zore/ui/UIElement.hpp"
#include "zore/utils/DataTypes.hpp"

namespace zore {

	class UILayer : public UIElement {
	public:
		UILayer();
		~UILayer() = default;

		void Update(uint32_t width, uint32_t height);

	protected:
		void Initialize(UIElement* parent) override;
	};
}