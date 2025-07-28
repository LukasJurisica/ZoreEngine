#pragma once

#include <string>

namespace zore::audio {

	class Manager {
	public:
		static void Init();
		static void Cleanup();

		static void update();
		static void SetVolume(float volume);
		static void Play(const std::string& filename);
	};
}