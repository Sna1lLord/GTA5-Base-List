#ifndef _LOGGER_HPP
#define _LOGGER_HPP

#include <common.hpp>

#define __FILENAME__ (strrchr(__FILE__, '../') ? strrchr(__FILE__, '../') + 1 : strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)

class Logger {
public:
    static Logger* Instance();
    void Initialize();
    void Uninitialize();

    void Log(const char* format, ...) {
        va_list args;
        va_start(args, format);

        printf("%c [Breeze]::", 219);
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
        printf("[%s] ", __FILENAME__);
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_BLUE | FOREGROUND_INTENSITY);
        vprintf(format, args);

        va_end(args);
    }
};

#endif