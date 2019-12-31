#include "utils/FileManager.hpp"

#include <fstream>

#include "utils/Logger.hpp"

namespace FileManager {
	std::string readTextFile(std::string filename) {
		// Open file
		std::ifstream f(filename);
		if (f.fail())
			Logger::error("Error opening file: " + std::string(filename));

		// Read file
		std::string content;
		std::string line;
		while (std::getline(f, line))
			content += line + "\n";

		// Close file
		f.close();

		// Return Result
		return content;
	}
}