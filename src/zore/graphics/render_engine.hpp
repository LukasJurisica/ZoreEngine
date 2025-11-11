#pragma once

#include "zore/utils/sized_integer.hpp"

namespace zore {

	//========================================================================
	//	Render Engine Interface
	//========================================================================

	class RenderEngine {
	public:
		static void Init();
        static void Cleanup();
		static void SetViewport(uint32_t width, uint32_t height, uint32_t x = 0u, uint32_t y = 0u);
	};
}