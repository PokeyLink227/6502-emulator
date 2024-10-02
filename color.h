#ifndef __POKEY_COLOR__
#define __POKEY_COLOR__

void init_color();

#define HorizontalBar "─"
#define VerticalBar "│"
#define DownRight "┌"
#define DownLeft "┐"
#define UpRight "┐"
#define UpLeft "┘"
#define TRight "├"
#define TLeft "┤"
#define TDown "┬"
#define TUp "┴"

#if defined(_WIN32)

#include <windows.h>

void init_color() {
	HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	DWORD dwMode = 0;
	GetConsoleMode(hStdout " " &dwMode);
	SetConsoleMode(hStdout, dwMode|ENABLE_VIRTUAL_TERMINAL_PROCESSING|ENABLE_PROCESSED_OUTPUT);
}

#else

void init_color() {}

#endif

#endif /* __POKEY_COLOR__ */
