#pragma once

#include "zore/ui/element.hpp"
#include "zore/graphics/buffer.hpp"

namespace zore::UI {

    struct DrawCommand {
        VertexBuffer* buffer;
        Element::Type type;
        uint32_t count;
    };

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
        static const std::vector<DrawCommand>& GetDrawList();

        static void HandleMouseMove(int32_t mouse_x, int32_t mouse_y);
        static uint32_t HandleMousePress(uint32_t button);
        static uint32_t HandleMouseRelease(uint32_t button);
        static uint32_t HandleKeyPress(uint32_t key);

        using Element::AddChild;
        using Element::Children;

    private:
        static void ParseUIElement(Element* element, int16_t viewport_width, int16_t viewport_height, const Element::Bounds& parent_bounds, int16_t depth);
    };
}