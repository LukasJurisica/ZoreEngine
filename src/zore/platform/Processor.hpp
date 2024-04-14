#pragma once

#include <array>
#include <vector>
#include <string>

namespace zore {

	class Processor {
	public:
		static std::string GetVendor();

	private:
		class Info {
		public:
			Info();
			~Info() = default;

			std::vector<std::array<int, 4>> m_data;
		};

		static Info s_info;
	};
}