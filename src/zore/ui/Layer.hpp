#pragma once

#include "zore/ui/Element.hpp"

namespace zore::UI {

    //========================================================================
    //	Layer Class
    //========================================================================

    class Layer : public Element {
    public:
        Layer(const std::string& style = "");
        ~Layer() = default;
        
        void Resize(uint32_t width, uint32_t height);
        void Flush();
        void Render();

    private:
		void ParseUIElement(const Element& element, uint32_t global_height, const Element::Bounds& parent_bounds, int32_t depth);

    private:
		struct ColouredQuad {
			ColouredQuad(int32_t x, int32_t y, int32_t w, int32_t h, int32_t d, Colour c);
			int32_t position;
			int32_t size;
			int32_t data;
			int32_t colour;
		};

    private:
        std::vector<ColouredQuad> m_quads;
    };
}