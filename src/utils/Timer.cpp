#include "utils/Timer.hpp"

#include <glfw/glfw3.h>

Timer::Timer() : startTime(0), lastTime(0){};

void Timer::start() {
	startTime = glfwGetTime();
	lastTime = startTime;
}

double Timer::timeSinceStart() {
	return (glfwGetTime() - startTime);
}

double Timer::deltaTime() {
	double currentTime = glfwGetTime();
	double deltaTime = currentTime - lastTime;
	lastTime = currentTime;
	return deltaTime;
}

Timer::~Timer() {};