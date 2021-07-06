#include "utils/FileManager.hpp"
#include "debug/Debug.hpp"
#include "path_config.h"
#include <fstream>

#if IS_DEBUG
#define PATH (BASE_DIRECTORY "/")
#else
#define PATH "./"
#endif

namespace zore {

	void FileManager::ReadContent(std::string& result, const std::string& filename, bool includeEmptyLines) {
		result = "";
		// Open file
		std::ifstream f("./" + filename);
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
		std::ifstream f("./" + filename);
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
		return std::string(PATH) + filename;
	}
}