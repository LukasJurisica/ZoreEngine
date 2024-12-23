#pragma once

#include "zore/ui/Element.hpp"

namespace zore::UI {

    //========================================================================
    //	Layer Class
    //========================================================================

    class Layer : private Element {
    public:
        Layer();
        ~Layer() = default;

        void Bind();

        static void Resize(uint32_t width, uint32_t height);
        static void Flush();
        static void Render();

		static void HandleMouseMove(uint32_t mouse_x, uint32_t mouse_y);
		static uint32_t HandleMousePress(uint32_t button);
		static uint32_t HandleMouseRelease(uint32_t button);
		static uint32_t HandleKeyPress(uint32_t key);

		using Element::AddChild;
        using Element::GetChild;
		using Element::Children;

    private:
		static void ParseUIElement(Element& element, int16_t viewport_width, int16_t viewport_height, const Element::Bounds& parent_bounds, int16_t depth);
    };
}