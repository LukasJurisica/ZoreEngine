#include "debug/Console.hpp"
#include "platform/win32/win32_exception.hpp"

#include <iostream>

namespace zore {

    Console::Console() {
        //Create a console for this application
        AllocConsole();
        FILE* fp;

        // Redirect STDIN if the console has an input handle
        if (GetStdHandle(STD_INPUT_HANDLE) == INVALID_HANDLE_VALUE)
            throw WND_LAST_ERROR();
        freopen_s(&fp, "CONIN$", "r", stdin);
        setvbuf(stdin, NULL, _IONBF, 0);
        // Redirect STDOUT if the console has an output handle
        if (GetStdHandle(STD_OUTPUT_HANDLE) == INVALID_HANDLE_VALUE)
            throw WND_LAST_ERROR();
        freopen_s(&fp, "CONOUT$", "w", stdout);
        setvbuf(stdout, NULL, _IONBF, 0);
        // Redirect STDERR if the console has an error handle
        if (GetStdHandle(STD_ERROR_HANDLE) == INVALID_HANDLE_VALUE)
            throw WND_LAST_ERROR();
        freopen_s(&fp, "CONOUT$", "w", stderr);
        setvbuf(stderr, NULL, _IONBF, 0);

        // Make C++ standard streams point to console as well.
        std::ios::sync_with_stdio(true);

        // Clear the error state for each of the C++ standard streams.
        std::cin.clear();
        std::cout.clear();
        std::cerr.clear();
        std::wcin.clear();
        std::wcout.clear();
        std::wcerr.clear();
    }

    Console::~Console() {
        FreeConsole();
    }

    void Console::SetTextColor(Colour c) {
        if (activeColour == c)
            return;
        activeColour = c;

        char ConsoleColourLookupTable[7] = {
            FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE,
            FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE,
            FOREGROUND_INTENSITY | FOREGROUND_BLUE,
            FOREGROUND_INTENSITY | FOREGROUND_GREEN,
            FOREGROUND_INTENSITY | FOREGROUND_RED,
            FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_RED,
            FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_BLUE
        };

        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), ConsoleColourLookupTable[static_cast<int>(c)]);
    }
}