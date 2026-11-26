#include <ncurses.h>
#include <string.h>
#include "ui.h"
#include "ui_internal.h"
#include "difficulty.h"

/* 5x5 block font, only the letters we need for the logo */
static const char *GLYPH[][5] = {
    /* D */ { "####.", "#...#", "#...#", "#...#", "####." },
    /* E */ { "#####", "#....", "####.", "#....", "#####" },
    /* F */ { "#####", "#....", "####.", "#....", "#...." },
    /* N */ { "#...#", "##..#", "#.#.#", "#..##", "#...#" },
    /* R */ { "####.", "#...#", "####.", "#.#..", "#..##" },
};

static int glyph_index(char c)
{
    switch (c) {
    case 'D': return 0;
    case 'E': return 1;
    case 'F': return 2;
    case 'N': return 3;
    default:  return 4;   /* 'R' */
    }
}

static void draw_logo(int top)
{
    const char *word = "DEFENDER";
    int len = (int)strlen(word);
    int width = len * 6 - 1;
    int x0 = (COLS - width) / 2;
    if (x0 < 0) x0 = 0;

    attron(COLOR_PAIR(C_TITLE) | A_BOLD);
    ui_center(top, "S  P  A  C  E");
    for (int row = 0; row < 5; row++) {
        for (int i = 0; i < len; i++) {
            const char *line = GLYPH[glyph_index(word[i])][row];
            for (int k = 0; k < 5; k++)
                if (line[k] == '#')
                    mvaddch(top + 2 + row, x0 + i * 6 + k, '#');
        }
    }
    attroff(COLOR_PAIR(C_TITLE) | A_BOLD);
}

MenuChoice ui_menu(Difficulty *diff)
{
    enum { ITEM_START, ITEM_DIFF, ITEM_HELP, ITEM_SCORES, ITEM_QUIT, ITEM_COUNT };
    int sel = 0;

    for (;;) {
        erase();
        int top = (LINES - 18) / 2;
        if (top < 0) top = 0;

        draw_logo(top);

        attron(COLOR_PAIR(C_PLAYER) | A_BOLD);
        ui_center(top + 8, "/A\\");
        attroff(COLOR_PAIR(C_PLAYER) | A_BOLD);
        ui_center(top + 9, "Defend the base. Survive the waves. Beat the bosses.");

        char label[ITEM_COUNT][40] = {
            "Start Mission", "", "How to Play", "High Scores", "Quit"
        };
        snprintf(label[ITEM_DIFF], sizeof label[ITEM_DIFF],
                 "Difficulty:  < %s >", difficulty_get(*diff)->name);

        for (int i = 0; i < ITEM_COUNT; i++) {
            if (i == sel) {
                attron(A_REVERSE | A_BOLD);
                ui_center(top + 11 + i, "  %-24s  ", label[i]);
                attroff(A_REVERSE | A_BOLD);
            } else {
                ui_center(top + 11 + i, "  %-24s  ", label[i]);
            }
        }
        ui_center(top + 17, "Up/Down: select   Left/Right: difficulty   Enter: confirm");
        refresh();

        int ch = getch();
        switch (ch) {
        case KEY_UP:   case 'w': case 'W': sel = (sel + ITEM_COUNT - 1) % ITEM_COUNT; break;
        case KEY_DOWN: case 's': case 'S': sel = (sel + 1) % ITEM_COUNT;              break;
        case KEY_LEFT:
            if (sel == ITEM_DIFF) *diff = (Difficulty)((*diff + DIFF_COUNT - 1) % DIFF_COUNT);
            break;
        case KEY_RIGHT:
            if (sel == ITEM_DIFF) *diff = (Difficulty)((*diff + 1) % DIFF_COUNT);
            break;
        case 'q': case 'Q': return MENU_QUIT;
        case '\n': case '\r': case KEY_ENTER:
            switch (sel) {
            case ITEM_START:  return MENU_START;
            case ITEM_DIFF:   *diff = (Difficulty)((*diff + 1) % DIFF_COUNT); break;
            case ITEM_HELP:   return MENU_HELP;
            case ITEM_SCORES: return MENU_SCORES;
            default:          return MENU_QUIT;
            }
            break;
        default: break;
        }
    }
}
