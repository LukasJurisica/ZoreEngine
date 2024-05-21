#pragma once
#include <chrono>

namespace zore {

	using TimePoint = std::chrono::steady_clock::time_point;

	class Timer {
	public:
		Timer() { Reset(); };
		~Timer() = default;

		void Reset() {
			last_time = std::chrono::steady_clock::now();
		}

		float Time() {
			std::chrono::duration<float> elapsed_time = std::chrono::steady_clock::now() - last_time;
			return elapsed_time.count();
		}

		bool TimeHasElapsed(float time, bool update = false) {
			TimePoint current_time = std::chrono::steady_clock::now();
			std::chrono::duration<float> elapsed_time = current_time - last_time;
			bool sufficient_time_elapsed = elapsed_time.count() >= time;
			if (sufficient_time_elapsed && update)
				last_time = current_time;
			return sufficient_time_elapsed;
		}

	private:
		TimePoint last_time;
	};

	class Time {
	public:
		friend class Window;

	public:
		static inline float DeltaTime() {
			return s_delta_time;
		}

		static inline TimePoint CurrentTime() {
			return std::chrono::steady_clock::now();
		}

	private:
		static void NewFrame() {
			TimePoint current_time = CurrentTime();
			std::chrono::duration<float> elapsed_time = current_time - s_last_time;
			s_delta_time = elapsed_time.count();
			s_last_time = current_time;
		}

	private:
		// These are defined in Window.cpp
		static TimePoint s_last_time;
		static float s_delta_time;
	};
}