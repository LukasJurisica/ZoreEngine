#include "utils/FileManager.hpp"
#include "debug/Debug.hpp"
#include "path_config.h"
#include <filesystem>
#include <fstream>

#if IS_DEBUG
std::string workingDirectory = std::string(BASE_DIRECTORY) + "/";
#else
std::string workingDirectory = "./";
#endif

namespace zore {

	void FileManager::Init() {
		// Get the current working file directory
		//std::filesystem::current_path();
		
		// Set the current working file directory
		//std::filesystem::current_path(workingDirectory);
	}

	void FileManager::ReadContent(std::string& result, const std::string& filename, bool includeEmptyLines) {
		result = "";
		// Open file
		std::ifstream f(workingDirectory + filename);
		ENSURE(f.is_open(), "Error opening file: " + filename);

		// Read file
		std::string line;
		while (std::getline(f, line))
			if (line != "" || includeEmptyLines)
				result += line + "\n";
	}

	void FileManager::ReadLines(std::vector<std::string>& result, const std::string& filename, bool includeEmptyLines) {
		result.clear();
		// Open file
		std::ifstream f(workingDirectory + filename);
		ENSURE(f.is_open(), "Error opening file: " + filename);

		// Read file
		std::string line;
		while (std::getline(f, line))
			if (line != "" || includeEmptyLines)
				result.push_back(line);
	}

	void FileManager::ReadChunks(std::vector<std::string>& result, const std::string& filename, const std::string& delimiter, bool includeEmptyLines) {

	}

	std::string FileManager::GetPath(const std::string& filename) {
		return workingDirectory + filename;
	}
}