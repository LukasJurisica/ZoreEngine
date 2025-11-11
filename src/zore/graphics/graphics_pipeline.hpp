#pragma once
#include "zore/graphics/shader.hpp"
#include "zore/graphics/vertex_layout.hpp"

namespace zore {

	class GraphicsPipeline {
	public:
		GraphicsPipeline() = default;
		GraphicsPipeline(const Shader& shader, const VertexLayout& layout);
		~GraphicsPipeline();
		void Bind() const;

	private:
#if defined(ZORE_RENDER_API_OPENGL)
		handle_t m_shader_id;
		handle_t m_vao_id;
#elif defined(ZORE_RENDER_API_VULKAN)
		handle_t m_pipeline_id;
		handle_t m_pipeline_layout;
		handle_t m_render_pass;
#endif
	};
}