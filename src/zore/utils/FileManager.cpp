#include "zore/utils/FileManager.hpp"
#include "zore/utils/StringUtils.hpp"
#include "zore/debug/Debug.hpp"
#include "path_config.h"

#include <filesystem>
#include <fstream>

namespace zore {

	void FileManager::Init() {
		// Get the current working file directory
		//std::filesystem::current_path();
		
#if IS_DEBUG
		std::filesystem::current_path(std::string(BASE_DIRECTORY) + "/");
#else
		std::filesystem::current_path("./");
#endif
	}

	void FileManager::ReadContent(std::string& result, const std::string& filename, bool mustExist, bool includeEmptyLines) {
		// Open file
		std::ifstream f(filename);
		if (f.is_open()) {
			// Read file
			std::string line;
			while (std::getline(f, line))
				if (line != "" || includeEmptyLines)
					result += line + "\n";
		}
		else if (mustExist)
			throw ZORE_EXCEPTION("Error opening file: " + filename);
	}

	void FileManager::ReadLines(std::vector<std::string>& result, const std::string& filename, bool mustExist, bool includeEmptyLines) {
		// Open file
		std::ifstream f(filename);
		if (f.is_open()) {
			// Read file
			std::string line;
			while (std::getline(f, line))
				if (line != "" || includeEmptyLines)
					result.emplace_back(line);
		}
		else if (mustExist)
			throw ZORE_EXCEPTION("Error opening file: " + filename);
	}

	void FileManager::ReadChunks(std::vector<std::string>& result, const std::string& filename, const std::string& delimiter, bool includeEmptyLines) {

	}

	void FileManager::WriteContent(const std::string& data, const std::string& filename, bool overwrite) {
		EnsureDir(filename.substr(0, filename.rfind("/")));
		std::ofstream f(filename, overwrite ? std::ios::out : std::ios::out | std::ios::app);
		if (f.good())
			f << data;
		else
			Logger::Error("Error writing to file: " + filename);
	}

	void FileManager::IncrementFilenameIfExists(std::string& filename) {
		if (std::filesystem::exists(filename)) {
			std::vector<std::string> tokens;
			StringUtils::SplitOnChr(tokens, filename, ".");

			uint32_t index = static_cast<uint32_t>(tokens[0].length()) + 2u;
			std::string test = tokens[0] + "_01." + tokens[1];
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

	std::string FileManager::GetAbsolutePath(const std::string& filename) {
		return std::filesystem::current_path().u8string() + filename;
	}

	void FileManager::EnsureDir(const std::string& path) {
		if (!std::filesystem::exists(path))
			std::filesystem::create_directories(path);
	}
}