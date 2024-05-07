#pragma once
#include <chrono>

namespace zore {

	class Timer {
	public:
		Timer() { Reset(); };
		~Timer() = default;

		void Reset() {
			start_time = std::chrono::steady_clock::now();
			last_time = start_time;
		}

		float Time() {
			std::chrono::duration<float> elapsed_time = std::chrono::steady_clock::now() - start_time;
			return elapsed_time.count();
		}

		float DeltaTime() {
			std::chrono::steady_clock::time_point current_time = std::chrono::steady_clock::now();
			std::chrono::duration<float> elapsed_time = current_time - last_time;
			last_time = current_time;
			return elapsed_time.count();
		}

		bool TimeHasElapsed(float time, bool update = false) {
			std::chrono::steady_clock::time_point current_time = std::chrono::steady_clock::now();
			std::chrono::duration<float> elapsed_time = current_time - start_time;
			bool sufficient_time_elapsed = elapsed_time.count() >= time;
			if (sufficient_time_elapsed && update)
				start_time = current_time;
			return sufficient_time_elapsed;
		}

	private:
		std::chrono::steady_clock::time_point start_time;
		std::chrono::steady_clock::time_point last_time;
	};
}