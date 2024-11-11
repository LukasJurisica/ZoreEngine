#pragma once

#include "zore/ui/Element.hpp"

namespace zore::UI {

    //========================================================================
    //	Layer Class
    //========================================================================

    class Layer : public Element {
    public:
        Layer(const std::string& name);
        ~Layer() = default;

        static void Render();
        static void Bind(const std::string& name);
        static void Flush(uint32_t width, uint32_t height);
        static void HandleMouseMove(uint32_t mouse_x, uint32_t mouse_y);
        static uint32_t HandleMouseClick(uint32_t button);

        void Bind();
        void Flush();

    private:
		void ParseUIElement(const Element& element, uint32_t global_height, const Element::Bounds& parent_bounds, uint32_t depth);

    private:
		struct ColouredQuad {
			ColouredQuad(uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint32_t d, Colour c);
			uint32_t position;
			uint32_t size;
			uint32_t depth;
			uint32_t colour;
		};

    private:
        std::vector<ColouredQuad> m_quads;
    };
}