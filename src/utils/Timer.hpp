#pragma once

class Timer {
public:
	Timer();
	~Timer();

	void start();
	float timeSinceStart();
	float deltaTime();
private:
	float getCurrentTime();

	float m_startTime;
	float m_lastTime;
};