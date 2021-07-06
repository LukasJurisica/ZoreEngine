#pragma once
#include <chrono>

namespace zore {

	class Timer {
	public:
		Timer();
		~Timer() = default;

		void Reset();
		float TimeSinceStart();
		float DeltaTime(bool update = true);

	private:
		std::chrono::steady_clock::time_point startTime;
		std::chrono::steady_clock::time_point lastTime;
	};
}