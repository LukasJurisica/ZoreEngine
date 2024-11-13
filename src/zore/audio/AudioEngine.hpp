#pragma once

#include <string>

namespace zore {

	class AudioEngine {
	public:
		static void Init();
		static void Cleanup();

		static void Play(const std::string& filename);
	};
}