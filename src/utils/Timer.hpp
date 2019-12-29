#pragma once

class Timer {
public:
	Timer();
	~Timer();

	void start();
	double timeSinceStart();
	double deltaTime();
private:
	double startTime;
	double lastTime;
};