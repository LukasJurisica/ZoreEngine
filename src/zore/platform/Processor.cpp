#include "zore/platform/Processor.hpp"
#include "zore/Platform.hpp"
#include "zore/Debug.hpp"

#if defined(PLATFORM_WINDOWS)
#include <intrin.h>
#elif defined(PLATFORM_LINUX)
#include <cpuid.h>
#endif

#include <vector>

namespace zore {

	Processor::Info Processor::s_info = Processor::Info();

	static uint32_t getCpuIdMax() {
#if defined(PLATFORM_WINDOWS)
		int data[4];
		__cpuid(data, 0);
		return data[0];
#elif defined(PLATFORM_LINUX)
		return __get_cpuid_max(0, nullptr);
#endif
	}

	static void getCpuId(uint32_t level, int reg[4]) {
#if defined(PLATFORM_WINDOWS)
		__cpuid(reg, level);
#elif defined(PLATFORM_LINUX)
		__get_cpuid(level, &reg[0], &reg[1], &reg[2], &reg[3]);
#endif
	}

	std::string Processor::GetVendor() {
		int vendor[3];
		memset(vendor, 0, sizeof(vendor));
		vendor[0] = s_info.m_data[0][1];
		vendor[1] = s_info.m_data[0][3];
		vendor[2] = s_info.m_data[0][2];
		std::string result = std::string(reinterpret_cast<char*>(vendor), sizeof(vendor));

		if (result == "AuthenticAMD")
			return "AMD";
		else if (result == "GenuineIntel")
			return "Intel";
		return result;
	}

	Processor::Info::Info() {
		int data[4];
		uint32_t function_count = getCpuIdMax();

		for (uint32_t i = 0; i < function_count; i++) {
			getCpuId(i, data);
			m_data.emplace_back(std::to_array(data));
		}
	}
}