#include "zore/audio/Source.hpp"
#include "zore/audio/Manager.hpp"
#include "zore/Debug.hpp"

#include <miniaudio.h>

namespace zore::audio {

	Source::Source(const std::string& filename, Mode mode, bool async) : m_status(Status::INVALID), m_mode(mode) {
        EngineConfig config = Manager::GetEngineConfig();
        
        switch (mode) {
            ma_decoder_config decoder_config = ma_decoder_config_init(config.format, config.channels, config.sample_rate);
            case Mode::DECODED: {
                ma_uint64 frame_count;
                ma_result result = ma_decode_file(filename.c_str(), &decoder_config, &frame_count, &m_data);
                if (result != MA_SUCCESS) return;

                ma_audio_buffer_config buffer_config = ma_audio_buffer_config_init(config.format, config.channels, frame_count, m_data, NULL);
                ma_audio_buffer* audio_buffer = new ma_audio_buffer();
                if (ma_audio_buffer_init(&buffer_config, audio_buffer) != MA_SUCCESS)
                    delete audio_buffer;
                m_data_source = audio_buffer;
                m_status = Status::READY;
            }
            case Mode::STREAM: {
                ma_decoder* decoder = new ma_decoder();

                ma_decoder_init_file(filename.c_str(), &decoder_config, decoder);
                m_status = Status::LOADING;
            }
        }
	}

	Source::~Source() {
		// Destructor implementation
	}
}