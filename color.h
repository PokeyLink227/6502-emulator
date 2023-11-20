#ifndef __POKEY_COLOR__
#define __POKEY_COLOR__

void init_color();

#if defined(_WIN32)
    #include <windows.h>

    void init_color() {
        HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
        DWORD dwMode = 0;
        GetConsoleMode(hStdout, &dwMode);
        SetConsoleMode(hStdout, dwMode|ENABLE_VIRTUAL_TERMINAL_PROCESSING|ENABLE_PROCESSED_OUTPUT);
    }

#else
    void init_color() {
    }
#endif

#endif /* POKEY_COLOR */
