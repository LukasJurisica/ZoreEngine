#include "zore/platform/macos/macos_core.hpp"
#include "zore/core/application.hpp"
#include "zore/debug.hpp"
#include <stdlib.h>

int main() {
	int return_code = EXIT_FAILURE;

	try {
		zore::Application::Init();
		return_code = EXIT_SUCCESS;
	}
	catch (const zore::Exception& e) {
		zore::Logger::Error(e.what());
		//MessageBox(nullptr, e.what(), e.GetType(), MB_OK | MB_ICONEXCLAMATION);
	}
	catch (const std::exception& e) {
		zore::Logger::Error(e.what());
		//MessageBox(nullptr, e.what(), "Generic Exception", MB_OK | MB_ICONEXCLAMATION);
	}
	catch (...) {
		//MessageBox(nullptr, "No details available", "Unknown Exception", MB_OK | MB_ICONEXCLAMATION);
	}
	
	if (return_code == EXIT_FAILURE)
		zore::Console::Dump();

	zore::Application::Cleanup();
	return return_code;
}