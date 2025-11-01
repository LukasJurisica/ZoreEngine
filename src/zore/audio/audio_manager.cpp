#include "zore/audio/audio_manager.hpp"
#include "zore/core/file_manager.hpp"
#include "zore/debug.hpp"
#define MINIAUDIO_IMPLEMENTATION
#include <miniaudio.h>

namespace zore::audio {

	static ma_engine s_audio_engine;

	void Manager::Init() {
        ma_engine_config engine_config = ma_engine_config_init();

		ENSURE(ma_engine_init(NULL, &s_audio_engine) == MA_SUCCESS, "Failed to initialize audio engine");
		Logger::Info("Audio Engine Initialization Complete.");
	}

	void Manager::Cleanup() {
		ma_engine_uninit(&s_audio_engine);
		Logger::Info("Audio Engine Cleanup Complete.");
	}

	void Manager::update() {
		//ma_engine_update(&s_audio_engine);
	}

	void Manager::SetVolume(float volume) {
		ma_result result = ma_engine_set_volume(&s_audio_engine, volume);
		ENSURE(result == MA_SUCCESS, std::format("Failed to set audio volume. Error Code: {}", static_cast<int>(result)));
	}

	void Manager::Play(const std::string& filename) {
		ma_result result = ma_engine_play_sound(&s_audio_engine, filename.c_str(), NULL);
		if (result != MA_SUCCESS)
			Logger::Error("Failed to play sound:", filename, "Error Code:", result);
	}

    ma_engine* Manager::GetEngine() {
        return &s_audio_engine;
    }

    EngineConfig Manager::GetEngineConfig() {
        return {
            .format      = MA_DEFAULT_FORMAT,
            .channels    = ma_engine_get_channels(&s_audio_engine),
            .sample_rate = ma_engine_get_sample_rate(&s_audio_engine)
        };
    }
}