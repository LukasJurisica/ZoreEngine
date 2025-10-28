#pragma once

#include <string>

namespace zore::audio {

	class Source {
    public:
        enum class Status { INVALID = -1, LOADING, READY };
        enum class Mode { DECODED, STREAM };

	public:
        Source(const std::string& filename, Mode mode = Mode::DECODED);
        Source(const Source&) = delete;
        Source& operator=(const Source&) = delete;
        ~Source();

        Status GetStatus() const { return m_status; }

	private:
        void* m_data_source = nullptr;
        void* m_data = nullptr;
        Status m_status;
        Mode m_mode;
	};
}