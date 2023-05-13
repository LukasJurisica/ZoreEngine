#include "zore/platform/win32/win32_Core.hpp"
#include "zore/core/Application.hpp"
#include "zore/debug/Console.hpp"
#include "zore/debug/Debug.hpp"

#include <WinDef.h>

int WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd) {
	DEBUG_ONLY(zore::Console console);

	try {
		zore::Application::Init();
		return EXIT_SUCCESS;
	}
	catch (const zore::Exception& e) {
		MessageBox(nullptr, e.what(), e.GetType(), MB_OK | MB_ICONEXCLAMATION);
	}
	catch (const std::exception& e) {
		MessageBox(nullptr, e.what(), "Generic Exception", MB_OK | MB_ICONEXCLAMATION);
	}
	catch (...) {
		MessageBox(nullptr, "No details available", "Unknown Exception", MB_OK | MB_ICONEXCLAMATION);
	}
	
	return EXIT_FAILURE;
}