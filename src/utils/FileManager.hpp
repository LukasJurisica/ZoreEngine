#pragma once
#include "utils/DataTypes.hpp"
#include <string>
#include <vector>

namespace zore {

	class FileManager {
	public:
		static void Init();
		static void ReadContent(std::string& result, const std::string& filename, bool includeEmptyLines = true);
		static void ReadLines(std::vector<std::string>& result, const std::string& filename, bool includeEmptyLines = true);
		static void ReadChunks(std::vector<std::string>& result, const std::string& filename, const std::string& delimiter, bool includeEmptyLines = true);
		static std::string GetPath(const std::string& filename);
	};
}