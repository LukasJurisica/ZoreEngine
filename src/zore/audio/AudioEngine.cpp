#include "zore/audio/AudioEngine.hpp"
#include "soloud.h"

namespace zore {

	static SoLoud::Soloud* s_soloud_engine = new SoLoud::Soloud();

	void AudioEngine::Init() {
		s_soloud_engine->init();
	}

	void AudioEngine::Cleanup() {
		s_soloud_engine->deinit();
	}
}