#include "core/Application.hpp"

int main() {
	Application app;

	app.setupScene();
	app.mainLoop();

	return 0;
}