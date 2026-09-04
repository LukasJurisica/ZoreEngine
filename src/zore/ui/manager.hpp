#pragma once

#include "zore/ui/layer.hpp"
#include <string_view>

namespace zore::UI {

	class Manager {
	public:
		static Layer& CreateLayer(std::string_view name);
		static Layer* GetLayer(std::string_view name);
		static Layer* Bind(std::string_view name);
		static Layer* GetActiveLayer();
		static void Resize(uint32_t width, uint32_t height);
		static void Clear();

	protected:
		Manager() = default;
		~Manager() = default;
	};
}
