#include "Timer.hpp"

namespace zore {

	Timer::Timer() {
		Reset();
	};

	void Timer::Reset() {
		startTime = std::chrono::steady_clock::now();
		lastTime = startTime;
	}

	float Timer::TimeSinceStart() {
		std::chrono::duration<float> elapsed = std::chrono::steady_clock::now() - startTime;
		return elapsed.count();
	}

	float Timer::DeltaTime(bool update) {
		std::chrono::steady_clock::time_point currentTime = std::chrono::steady_clock::now();
		std::chrono::duration<float> elapsed = currentTime - lastTime;
		if (update)
			lastTime = currentTime;
		return elapsed.count();
	}
}