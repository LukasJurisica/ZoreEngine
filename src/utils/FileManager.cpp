#include "utils/FileManager.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"
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

	void FileManager::ReadContent(std::string& result, const std::string& filename, bool includeEmptyLines) {
		// Open file
		std::ifstream f(filename);
		ENSURE(f.is_open(), "Error opening file: " + filename);

		// Read file
		std::string line;
		while (std::getline(f, line))
			if (line != "" || includeEmptyLines)
				result += line + "\n";
	}

	void FileManager::ReadLines(std::vector<std::string>& result, const std::string& filename, bool includeEmptyLines) {
		// Open file
		std::ifstream f(filename);
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
		return filename;
	}
}