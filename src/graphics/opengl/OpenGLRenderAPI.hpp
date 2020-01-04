#pragma once

#include "graphics/RenderAPI.hpp"

class OpenGLRenderAPI : public RenderAPI {
public:
	void init() override;
	void setViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;
	void setClearColour(const glm::vec4& colour) override;
	void clear() override;

	void drawIndexed(const std::shared_ptr<VertexArray>& vertexArray) const override;
};
