#pragma once

#include <glm/vec4.hpp>

enum class RenderAPI {
	None, OpenGL
};

class RenderEngine {
public:
	RenderEngine();
	~RenderEngine();

	void clearScreen();
	void setClearColour(glm::vec4 colour);

	inline static RenderAPI getAPI() { return renderAPI; };

private:
	static RenderAPI renderAPI;
};