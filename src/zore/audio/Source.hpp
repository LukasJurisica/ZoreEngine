#pragma once

#include <string>

struct ma_data_source;

namespace zore::audio {

	class Source {
    public:
        enum class Status { INVALID = -1, LOADING, READY };
        enum class Mode { DECODED, STREAM };

	public:
        Source(const std::string& filename, Mode mode = Mode::DECODED, bool async = false);
        Source(const Source&) = delete;
        Source& operator=(const Source&) = delete;
        ~Source();

        Status GetStatus() const { return m_status; }

	private:
        ma_data_source* m_data_source;
        void* m_data;
        Status m_status;
        Mode m_mode;
	};
}