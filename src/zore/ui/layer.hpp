#pragma once

#include "zore/ui/element.hpp"
#include "zore/graphics/buffer.hpp"

namespace zore::UI {

	struct DrawCommand {
		VertexBuffer* buffer;
		Element::Type type;
		uint32_t count;
	};

	class Layer : private Element {
	public:
		Layer();
		~Layer() = default;

		void Resize(uint32_t width, uint32_t height);
		void Flush();
		void HandleMouseMove(int32_t mouse_x, int32_t mouse_y);
		ID HandleMousePress(uint32_t button);
		ID HandleMouseRelease(uint32_t button);

		const std::vector<DrawCommand>& GetDrawList() const { return m_draw_list; }
		const Bounds& GetBounds() const { return Element::GetBounds(); }

		using Element::Append;
		using Element::Children;

	private:
		struct Quad {
			uint32_t position, size, colour, data;
		};
		struct ButtonCollider {
			int32_t x1, y1, x2, y2;
			uint16_t depth;
			ID id;
			uint32_t quad_index;
		};

	private:
		void BuildDrawData(const Element& element, uint16_t depth);

	private:
		std::vector<Quad> m_quads;
		std::vector<Quad> m_glyphs;
		std::vector<ButtonCollider> m_button_colliders;
		int32_t m_hovered_button = -1;
		int32_t m_pressed_button = -1;
		VertexBuffer m_quad_buffer{ true };
		VertexBuffer m_glyph_buffer{ true };
		std::vector<DrawCommand> m_draw_list;
	};
}
