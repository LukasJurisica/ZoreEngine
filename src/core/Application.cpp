#include "core/Application.hpp"

#include "utils/Logger.hpp"

Application::Application() {}

void Application::mainLoop() {
	std::map<std::string, int> options;
	options["width"] = 1920;
	options["height"] = 1080;
	options["fullscreen"] = 0;
	options["vsync"] = 0;

	Window window(options);
	RenderEngine engine;
	engine.setClearColour(glm::vec4(0.8f, 0.5f, 0.5f, 1.0f));

	while (!window.shouldClose()) {
		engine.clearScreen();

		//std::cout << (window.keyPressed(KEY_H) ? "true" : "false") << std::endl;

		window.update();
	}
}

Application::~Application() {}