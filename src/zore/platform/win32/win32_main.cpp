#include "zore/platform/win32/win32_core.hpp"
#include "zore/core/application.hpp"
#include "zore/debug.hpp"
#include <shellapi.h>
#include <WinDef.h>

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow) {
	int return_code = EXIT_FAILURE;
	HWND hwnd = nullptr;

	try {
		int argc;
		LPWSTR* argv = CommandLineToArgvW(GetCommandLineW(), &argc);
		std::vector<std::wstring> command_line_arguments;
		if (argv != NULL) {
			for (int i = 0; i < argc; i++)
				command_line_arguments.emplace_back(argv[i]);
			LocalFree(argv);
		}

		zore::Application::Init(command_line_arguments);
		return_code = EXIT_SUCCESS;
	}
	catch (const zore::Exception& e) {
		zore::Logger::Error(e.what());
		zore::Window::GetNativeHandle(&hwnd);
		MessageBox(hwnd, e.what(), e.GetType(), MB_OK | MB_ICONEXCLAMATION | MB_APPLMODAL);
	}
	catch (const std::exception& e) {
		zore::Logger::Error(e.what());
		zore::Window::GetNativeHandle(&hwnd);
		MessageBox(hwnd, e.what(), "Generic Exception", MB_OK | MB_ICONEXCLAMATION | MB_APPLMODAL);
	}
	catch (...) {
		zore::Window::GetNativeHandle(&hwnd);
		MessageBox(hwnd, "No details available", "Unknown Exception", MB_OK | MB_ICONEXCLAMATION | MB_APPLMODAL);
	}
	
	if (return_code == EXIT_FAILURE)
		zore::Console::Dump();

	zore::Application::Cleanup();
	return return_code;
}