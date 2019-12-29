#pragma once

#include <glm/vec4.hpp>

class RenderEngine {
public:
	RenderEngine();
	~RenderEngine();

	void clearScreen();
	void setClearColour(glm::vec4 colour);

};