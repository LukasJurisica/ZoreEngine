#include "utils/Timer.hpp"

#include <glfw/glfw3.h>

Timer::Timer() : m_startTime(0), m_lastTime(0){};

void Timer::start() {
	m_startTime = getCurrentTime();
	m_lastTime = m_startTime;
}

float Timer::timeSinceStart() {
	return (getCurrentTime() - m_startTime);
}

float Timer::deltaTime() {
	float currentTime = getCurrentTime();
	float deltaTime = currentTime - m_lastTime;
	m_lastTime = currentTime;
	return deltaTime;
}

float Timer::getCurrentTime() {
	return (float)glfwGetTime();
}

Timer::~Timer() {};