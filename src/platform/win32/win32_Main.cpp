#include "platform/win32/win32_Core.hpp"
#include "core/Application.hpp"
#include "debug/Exception.hpp"
#include "debug/Console.hpp"
#include "debug/Debug.hpp"

int WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd) {
    DEBUG_ONLY(zore::Console console);

    int rc = EXIT_FAILURE;
    try {
		zore::Application::Init();
        rc = EXIT_SUCCESS;
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
	zore::Application::Cleanup();

    //system("pause");
    return rc;
}