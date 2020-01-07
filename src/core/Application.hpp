#pragma once

#include "core/core.hpp"
#include "core/Window.hpp"
#include "graphics/RenderEngine.hpp"
#include "graphics/Shader.hpp"
#include "graphics/Buffer.hpp"
#include "components/camera.hpp"
#include "utils/Timer.hpp"

class Application {
public:
	Application();
	~Application();

	void handleResize(int width, int height);
	void handleKeyPress(int key, int action);
	void handleMousePress(int button, int action);
	void handleMouseMove(float x, float y);

	void setupScene();
	void updateScene(float deltaTime);
	void mainLoop();

private:
	std::shared_ptr<Camera> m_camera;
	std::unique_ptr<Window> m_window;
	std::unique_ptr<RenderEngine> m_engine;
	
	std::shared_ptr<Shader> m_shader;
	std::shared_ptr<VertexArray> m_squareVA;

	glm::vec3 m_camPos = glm::vec3(0.0);
	float m_camRot = 0;
};