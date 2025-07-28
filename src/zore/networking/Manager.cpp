#include "zore/networking/Manager.hpp"
#include "zore/networking/Core.hpp"
#include "zore/Debug.hpp"

namespace zore::net {

	static bool s_initialized = false;

	void Manager::Init() {
#ifdef PLATFORM_WINDOWS
		WSADATA wsa_data;
		ENSURE(WSAStartup(MAKEWORD(2, 2), &wsa_data) == 0, "Failed to initialize Winsock.");
		ENSURE(LOBYTE(wsa_data.wVersion) == 2 && HIBYTE(wsa_data.wVersion) == 2, "Version 2.2 of Winsock is not available.");
#endif
		s_initialized = true;
		Logger::Info("Networking Initialization Complete.");
	}

	void Manager::Cleanup() {
#ifdef PLATFORM_WINDOWS
		WSACleanup();
#endif
		s_initialized = false;
		Logger::Info("Networking Cleanup Complete.");
	}

	std::string Manager::GetHostName() {
		char buffer[256];
		if (gethostname(buffer, 256) == -1) {
			Logger::Error(GetLastError("gethostname"));
			return "Failed to get hostname";
		}
		return std::string(buffer);
	}

	bool Manager::IsInitialized() {
		return s_initialized;
	}
}




