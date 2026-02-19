#ifndef NCURSES_HELPER_HPP
#define NCURSES_HELPER_HPP
#include <ncurses.h>

void fancy_print(WINDOW *win, int row, int col, const char *text, int speed_ms, int clip);
void auto_wrap(WINDOW *win, int startline, const char *text, int thresh);

#endif
