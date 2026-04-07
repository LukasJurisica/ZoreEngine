#pragma once

#include <string>

namespace zore {

	class Processor {
	public:
		static void Init();
		static void Free();
		static std::string GetVendor();

	private:
		static inline int Get(int function_id, int reg);
		static inline int GetCPUIDCount();
		static inline void GetCPUID(int function_id, int info[4]);
	};
}