#pragma once

#include <glm/vec4.hpp>

#include "graphics/RenderAPI.hpp"
#include "graphics/Shader.hpp"
#include "components/Camera.hpp"

class RenderEngine {
public:
	RenderEngine(std::unique_ptr<Camera>& camera);
	~RenderEngine();

	void begin();
	void end();
	void submit(const std::shared_ptr<VertexArray>& vertexArray, const std::shared_ptr<Shader>& shader);

	inline void setClearColour(const glm::vec4& colour) { m_renderAPI->setClearColour(colour); }
	inline void clear() { m_renderAPI->clear(); }

private:
	RenderAPI* m_renderAPI;
	std::unique_ptr<Camera>& m_camera;
};

/*
class RenderCommand {
public:
	inline static void setRenderAPI(RenderAPI* renderAPI) { s_renderAPI = renderAPI; }

	inline static void drawIndexed(const std::shared_ptr<VertexArray>& vertexArray) { s_renderAPI->drawIndexed(vertexArray); }
	inline static void setClearColour(const glm::vec4& colour) { s_renderAPI->setClearColour(colour); }
	inline static void clear() { s_renderAPI->clear(); }
private:
	static RenderAPI* s_renderAPI;
};
*/