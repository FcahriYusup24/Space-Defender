#include <ncurses.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include "ui.h"
#include "ui_internal.h"

void ui_center(int y, const char *fmt, ...)
{
    char buf[160];
    va_list ap;
    va_start(ap, fmt);
    vsnprintf(buf, sizeof buf, fmt, ap);
    va_end(ap);

    int x = (COLS - (int)strlen(buf)) / 2;
    if (x < 0)
        x = 0;
    mvprintw(y, x, "%s", buf);
}

void ui_init(void)
{
    initscr();
    noecho();
    cbreak();
    keypad(stdscr, TRUE);
    curs_set(0);

    if (has_colors()) {
        start_color();
        use_default_colors();
        init_pair(C_PLAYER,  COLOR_CYAN,    -1);
        init_pair(C_BULLET,  COLOR_YELLOW,  -1);
        init_pair(C_EBULLET, COLOR_RED,     -1);
        init_pair(C_STAR,    COLOR_WHITE,   -1);
        init_pair(C_TITLE,   COLOR_GREEN,   -1);
        init_pair(C_WARN,    COLOR_RED,     -1);
        init_pair(C_BASIC,   COLOR_RED,     -1);
        init_pair(C_FAST,    COLOR_YELLOW,  -1);
        init_pair(C_TANK,    COLOR_MAGENTA, -1);
        init_pair(C_SHOOTER, COLOR_BLUE,    -1);
        init_pair(C_BOSS,    COLOR_RED,     -1);
        init_pair(C_GOOD,    COLOR_GREEN,   -1);
    }
}

void ui_shutdown(void)
{
    endwin();
}

int ui_ensure_size(void)
{
    while (LINES < MIN_HEIGHT || COLS < MIN_WIDTH) {
        erase();
        ui_center(LINES / 2 - 1, "Terminal is too small (%dx%d).", COLS, LINES);
        ui_center(LINES / 2,     "Please enlarge it to at least %dx%d.", MIN_WIDTH, MIN_HEIGHT);
        ui_center(LINES / 2 + 1, "Press Q to go back.");
        refresh();
        int ch = getch();
        if (ch == 'q' || ch == 'Q')
            return 0;
    }
    return 1;
}
