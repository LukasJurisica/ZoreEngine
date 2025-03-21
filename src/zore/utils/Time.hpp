#pragma once
#include <chrono>

namespace zore {

	using TimePoint = std::chrono::steady_clock::time_point;

	//========================================================================
	//	Time Class
	//========================================================================

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
		
		static inline float Duration(TimePoint start, TimePoint end) {
			std::chrono::duration<float> elapsed_time = end - start;
			return elapsed_time.count();
		}
				
		static inline float Elapsed(TimePoint time) {
			std::chrono::duration<float> elapsed_time = CurrentTime() - time;
			return elapsed_time.count();
		}

	private:
		static inline void NewFrame() {
			TimePoint current_time = CurrentTime();
			std::chrono::duration<float> elapsed_time = current_time - s_last_time;
			s_delta_time = elapsed_time.count();
			s_last_time = current_time;
		}

	private:
		static inline TimePoint s_last_time;
		static inline float s_delta_time;
	};

	//========================================================================
	//	Timer Class
	//========================================================================

	class Timer {
	public:
		Timer() { Reset(); };
		~Timer() = default;

		void Reset() {
			last_time = Time::CurrentTime();
		}

		float Time() {
			std::chrono::duration<float> elapsed_time = Time::CurrentTime() - last_time;
			return elapsed_time.count();
		}

		bool TimeHasElapsed(float time, bool update = false) {
			TimePoint current_time = Time::CurrentTime();
			std::chrono::duration<float> elapsed_time = current_time - last_time;
			bool sufficient_time_elapsed = elapsed_time.count() >= time;
			if (sufficient_time_elapsed && update)
				last_time = current_time;
			return sufficient_time_elapsed;
		}

	private:
		TimePoint last_time;
	};
}