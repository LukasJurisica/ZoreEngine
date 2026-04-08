#include "zore/core/file_manager.hpp"
#include "zore/utils/string.hpp"
#include "zore/debug.hpp"
#include "path_config.h"
#include <filesystem>

namespace zore {

	//========================================================================
	//	File Manager Utility
	//========================================================================

	void File::Manager::Init(const std::string& path) {
#if IS_DEBUG
		std::filesystem::current_path(std::string(BASE_DIRECTORY) + path);
#else
		std::filesystem::current_path("." + path);
#endif
	}

	std::string File::Manager::Path(const std::string& path) {
		return std::filesystem::current_path().generic_string() + (path[0] == '/' ? "" : "/") + path;
	}

	void File::Manager::EnsureDir(const std::string& path) {
		if (!std::filesystem::exists(path))
			std::filesystem::create_directories(path);
	}

	//========================================================================
	//	File Class Utility
	//========================================================================

	File::Iterator::Iterator(std::fstream* stream) : m_stream(stream) {
		++(*this);
	}

	const std::string& File::Iterator::operator*() const {
		return m_line;
	}

	File::Iterator& File::Iterator::operator++() {
		if (m_stream && std::getline(*m_stream, m_line))
			return *this;
		m_stream = nullptr;
		return *this;
	}

	bool File::Iterator::operator!=(const Iterator& other) const {
		return m_stream != other.m_stream;
	}

	//========================================================================
	//	File Class
	//========================================================================

	File::File(const std::string& filename, Mode mode) : m_filename(filename), m_mode(mode) {
		int flags = 0;
		switch (mode) {
		case Mode::READ: flags = std::ios::in; break;
		case Mode::APPEND: flags = std::ios::out | std::ios::app; break;
		case Mode::TRUNCATE: flags = std::ios::out | std::ios::trunc; break;
		}

		m_stream.open(filename, flags);
	}

	bool File::Exists(const std::string& filename) {
		return std::filesystem::exists(filename);
	}

	File File::Open(const std::string& filename, Mode mode) {
		return File(filename, mode);
	}

	File::File(File&& other) noexcept {
		m_stream = std::move(other.m_stream);
		m_filename = other.m_filename;
		m_mode = other.m_mode;
	}

	File& File::operator=(File&& other) noexcept {
		m_stream = std::move(other.m_stream);
		m_filename = other.m_filename;
		m_mode = other.m_mode;
		return *this;
	}

	File::~File() {
		m_stream.close();
	}

	bool File::IsOpen() const {
		return m_stream.is_open();
	}

	void File::Close() {
		m_stream.close();
	}

	std::string File::Read() {
		if (!IsOpen())
			throw ZORE_EXCEPTION("Error opening file: " + m_filename);
		Reset();
		size_t size = Size();
		std::string buffer(size, '\0');
		m_stream.read(buffer.data(), size);
		return buffer;
	}

	bool File::ReadLine(std::string& line) {
		return static_cast<bool>(std::getline(m_stream, line));
	}

	void File::Write(std::string_view content) {
		m_stream.close();
		m_stream = std::fstream(m_filename, std::ios::trunc);
		m_stream.write(content.data(), content.size());
		m_stream.flush();
	}

	void File::Append(std::string_view content) {
		m_stream.seekp(0, std::ios::end);
		m_stream.write(content.data(), content.size());
		m_stream.flush();
	}

	void File::Reset() {
		m_stream.clear();
		m_stream.seekg(0);
		m_stream.seekp(0);
	}

	size_t File::Size() {
		auto pos = m_stream.tellg();
		m_stream.seekg(0, std::ios::end);
		size_t size = m_stream.tellg();
		m_stream.seekg(pos);
		return size;
	}

	File::Iterator File::begin() {
		if (!m_stream.is_open())
			Reset();
		if (!m_stream.is_open())
			throw ZORE_EXCEPTION("Error opening file: " + m_filename);
		return Iterator(&m_stream);
	}

	File::Iterator File::end() {
		return Iterator(nullptr);
	}
	/*
	void FileManager::IncrementFilenameIfExists(std::string& filename) {
		if (std::filesystem::exists(filename)) {
			std::vector<std::string_view> tokens;
			String::SplitV(tokens, filename, ".");

			uint32_t index = static_cast<uint32_t>(tokens[0].length()) + 2u;
			std::string test = std::format("{}_01.{}", tokens[0], tokens[1]);
			for (int count = 1; std::filesystem::exists(test); count++) {
				if (count % 10)
					test[index] += 1;
				else {
					test[index - 1] += 1;
					test[index] = '0';
				}
			}
			filename = test;
		}
	}
	*/
}