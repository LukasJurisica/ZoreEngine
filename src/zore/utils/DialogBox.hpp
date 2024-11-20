#pragma once
#include <string>
#include <vector>

struct FilterDesc {
	std::string description;
	std::string extension;
};

namespace zore {

	class Dialog {
	public:
		static std::string OpenFile(const std::vector<FilterDesc>& filters = {}, bool include_all_files = false);
		static std::string SaveFile(const std::vector<FilterDesc>& filters = {}, bool include_all_files = false);
	};
}