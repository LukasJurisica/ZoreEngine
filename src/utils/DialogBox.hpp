#pragma once
#include <string>
#include <vector>

struct FilterDesc {
	std::string desc;
	std::string ext;
};

namespace zore {

	class Dialog {
	public:
		static std::string OpenFile(const std::vector<FilterDesc>& filters = {}, bool includeAllFiles = false);
		static std::string SaveFile(const std::vector<FilterDesc>& filters = {}, bool includeAllFiles = false);
	};
}