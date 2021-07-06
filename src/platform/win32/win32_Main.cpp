#include "platform/win32/win32_Core.hpp"
#include "core/Application.hpp"
#include "debug/Exception.hpp"
#include <iostream>

int WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd) {
    try {
        zore::Application::EntryPoint();
        return EXIT_SUCCESS;
    }
    catch (const zore::Exception & e) {
        MessageBox(nullptr, e.what(), e.GetType(), MB_OK | MB_ICONEXCLAMATION);
    }
    catch (const std::exception & e) {
        MessageBox(nullptr, e.what(), "Generic Exception", MB_OK | MB_ICONEXCLAMATION);
    }
    catch (...) {
        MessageBox(nullptr, "No details available", "Unknown Exception", MB_OK | MB_ICONEXCLAMATION);
    }

    return EXIT_FAILURE;
}