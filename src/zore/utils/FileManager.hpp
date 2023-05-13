#pragma once
#include <string>
#include <vector>

namespace zore {

	class FileManager {
	public:
		static void Init();
		static void ReadContent(std::string& result, const std::string& filename, bool mustExist = true, bool includeEmptyLines = true);
		static void ReadLines(std::vector<std::string>& result, const std::string& filename, bool mustExist = true, bool includeEmptyLines = true);
		static void ReadChunks(std::vector<std::string>& result, const std::string& filename, const std::string& delimiter, bool includeEmptyLines = true);
		static void WriteContent(const std::string& data, const std::string& filename, bool overwrite = true);
		static void IncrementFilenameIfExists(std::string& filename);
		static std::string GetAbsolutePath(const std::string& filename);
		static void EnsureDir(const std::string& path);
	};
}