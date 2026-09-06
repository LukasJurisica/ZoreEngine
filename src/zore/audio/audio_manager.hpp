#pragma once

#include <miniaudio.h>
#include <string>

namespace zore::audio {

    struct EngineConfig {
        ma_format format;
        ma_uint32 channels;
        ma_uint32 sample_rate;
    };

	class Manager {
	public:
		static void Init();
		static void Cleanup();

		static void update();
		static void SetVolume(float volume);
		static void Play(const std::string& filename);

        static ma_engine* GetEngine();
        static EngineConfig GetEngineConfig();
	};
}