#include "utils/FileManager.hpp"
#include "debug/Debug.hpp"
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
		std::ofstream f(filename);
		f << data;
	}

	std::string FileManager::GetPath(const std::string& filename) {
		return filename;
	}
}