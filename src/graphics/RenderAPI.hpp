#pragma once

#include <glm/vec4.hpp>

#include "graphics/Buffer.hpp"

class RenderAPI {
public:
	enum class API {
		None, OpenGL
	};
	virtual void init() = 0;
	virtual void setViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;
	virtual void setClearColour(const glm::vec4& colour) = 0;
	virtual void clear() = 0;

	virtual void drawIndexed(const std::shared_ptr<VertexArray>& vertexArray) const = 0;

	inline static API getAPI() { return s_API; }
	inline static void setAPI(API api) { s_API = api; }

private:
	static API s_API;
};