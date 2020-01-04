#pragma once

#include <memory>

#include "window/Window.hpp"
#include "graphics/RenderEngine.hpp"
#include "graphics/Shader.hpp"
#include "graphics/Buffer.hpp"
#include "components/camera.hpp"
#include "utils/Timer.hpp"

class Application {
public:
	Application();
	~Application();

	void handleEvent(Event* event);

	void setupScene();
	void updateScene(float deltaTime);
	void mainLoop();

private:
	std::vector<Event*> m_events;
	std::unique_ptr<Window> m_window;
	std::unique_ptr<RenderEngine> m_engine;

	std::shared_ptr<Shader> m_shader;
	std::shared_ptr<VertexArray> m_squareVA;
	std::unique_ptr<Camera> m_camera;

	glm::vec3 m_camPos = glm::vec3(0.0);
	float m_camRot = 0;

};