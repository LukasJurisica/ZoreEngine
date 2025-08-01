#include "zore/audio/Manager.hpp"
#include "zore/core/FileManager.hpp"
#include "zore/Debug.hpp"

#define MINIAUDIO_IMPLEMENTATION
#include <miniaudio.h>

namespace zore::audio {

	static ma_engine s_audio_engine;

	void Manager::Init() {
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
}