#include "zore/platform/processor.hpp"
#include "zore/platform.hpp"
#include "zore/debug.hpp"

#if defined(PLATFORM_WINDOWS)
#include <intrin.h>
#elif defined(PLATFORM_LINUX)
#include <cpuid.h>
#elif defined(PLATFORM_MACOS)
#include <sys/sysctl.h>
#endif

namespace zore {

	//========================================================================
	//	Processor Class
	//========================================================================

	static int* s_data = nullptr;

	void Processor::Init() {
		delete[] s_data;
		uint32_t function_count = GetCPUIDCount();
		s_data = new int[function_count * 4];
		for (uint32_t i = 0; i < function_count; i++)
			GetCPUID(i, &s_data[i * 4]);
	}

	void Processor::Cleanup() {
		delete[] s_data;
	}

	std::string Processor::GetVendor() {
		int vendor[3] = { Get(0, 1), Get(0, 3), Get(0, 2) };
		std::string result = std::string(reinterpret_cast<char*>(vendor), sizeof(vendor));

		if (result == "AuthenticAMD")
			return "AMD";
		else if (result == "GenuineIntel")
			return "Intel";
		return result;
	}

	int Processor::Get(int function_id, int reg) {
		return s_data[(function_id * 4) + reg];
	}

	int Processor::GetCPUIDCount() {
#if defined(PLATFORM_WINDOWS)
		int data[4];
		GetCPUID(0, data);
		return data[0];
#elif defined(PLATFORM_LINUX)
		return __get_cpuid_max(0, nullptr);
#elif defined(PLATFORM_MACOS)
        return 0; // Not implemented, use sysctl calls if needed
#endif
	}

	void Processor::GetCPUID(int function_id, int info[4]) {
#if defined(PLATFORM_WINDOWS)
		__cpuid(info, function_id);
#elif defined(PLATFORM_LINUX)
		__get_cpuid(function_id, &info[0], &info[1], &info[2], &info[3]);
#elif defined(PLATFORM_MACOS)
        // Not implemented, use sysctl calls if needed
#endif  
	}
}