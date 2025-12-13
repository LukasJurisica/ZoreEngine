#pragma once
#include "zore/graphics/shader.hpp"
#include "zore/graphics/internal/vertex_layout.hpp"
#include "zore/graphics/internal/render_pass.hpp"

namespace zore {

	class GraphicsPipeline {
	public:
		GraphicsPipeline() = default;
		GraphicsPipeline(const Shader& shader, const VertexLayout& layout);
		~GraphicsPipeline();
		void Bind() const;

        static handle_t CreatePipelineLayout();

	private:
		handle_t m_pipeline_id;
		handle_t m_pipeline_layout;
		std::shared_ptr<RenderPass> m_render_pass;
	};
}