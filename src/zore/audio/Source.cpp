#include "zore/audio/Source.hpp"
#include "zore/audio/Manager.hpp"
#include "zore/Debug.hpp"

#include <miniaudio.h>

namespace zore::audio {

	Source::Source(const std::string& filename, Mode mode) : m_status(Status::INVALID), m_mode(mode), m_data(nullptr) {
        EngineConfig config = Manager::GetEngineConfig();
        ma_decoder_config decoder_config = ma_decoder_config_init(config.format, config.channels, config.sample_rate);
        
        if (mode == Mode::DECODED) {
            // Decode File
            ma_uint64 frame_count;
            if (ma_decode_file(filename.c_str(), &decoder_config, &frame_count, &m_data) != MA_SUCCESS)
                return;
            
			// Create Audio Buffer
            ma_audio_buffer_config buffer_config = ma_audio_buffer_config_init(config.format, config.channels, frame_count, m_data, nullptr);
            ma_audio_buffer* buffer = new ma_audio_buffer;
            if (ma_audio_buffer_init(&buffer_config, buffer) != MA_SUCCESS) {
                delete buffer;
				ma_free(m_data, nullptr);
                return;
            }
			m_data_source = buffer;
        }
        else if (mode == Mode::STREAM) {
			ma_decoder* decoder = new ma_decoder();
            if (ma_decoder_init_file(filename.c_str(), &decoder_config, decoder) != MA_SUCCESS)
                return;
            m_data_source = decoder;
        }
        m_status = Status::READY;
	}

	Source::~Source() {
		// Destructor implementation
	}
}