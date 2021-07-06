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

        switch (c) {
        case Colour::White:
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
            break;
        case Colour::Gray:
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
            break;
        case Colour::Blue:
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_BLUE);
            break;
        case Colour::Green:
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN);
            break;
        case Colour::Red:
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
            break;
        case Colour::Yellow:
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_RED);
            break;
        case Colour::Cyan:
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_BLUE);
            break;
        }
    }
}