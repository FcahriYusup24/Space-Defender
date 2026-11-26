#include <ncurses.h>
#include <string.h>
#include "ui.h"
#include "ui_internal.h"
#include "game.h"
#include "difficulty.h"

void ui_help(void)
{
    static const char *lines[] = {
        "MISSION",
        "  Defend the base from the alien fleet. Every enemy that gets through",
        "  damages the base. Lose the base or all your ships and it is over.",
        "",
        "CONTROLS",
        "  Left/Right or A/D  move       SPACE  fire        F  auto-fire",
        "  B  bomb (clears the screen)   P  pause           Q  end mission",
        "",
        "ENEMIES",
        "  \\W/ basic    <V> fast (zigzag)    [#] tank (3 hits)",
        "  {S} shooter (fires back)          Boss every 5 levels",
        "",
        "POWER-UPS (dropped by destroyed enemies)",
        "  [L] extra ship   [S] shield   [3] triple shot",
        "  [B] bomb         [R] repair the base",
        "",
        "SCORING",
        "  Destroy enemies in a row without being hit to build a combo",
        "  (up to x4). Taking damage or letting an enemy through resets it.",
    };
    const int n = (int)(sizeof lines / sizeof lines[0]);

    erase();
    attron(COLOR_PAIR(C_TITLE) | A_BOLD);
    ui_center(1, "HOW TO PLAY");
    attroff(COLOR_PAIR(C_TITLE) | A_BOLD);
    int x = (COLS - 70) / 2;
    if (x < 0) x = 0;
    for (int i = 0; i < n; i++)
        mvprintw(3 + i, x, "%s", lines[i]);
    ui_center(4 + n, "Press any key to go back");
    refresh();
    getch();
}

static void draw_table(int y, const ScoreEntry table[HS_COUNT])
{
    for (int i = 0; i < HS_COUNT; i++)
        ui_center(y + i, "%d. %-12s %6d  [%c]", i + 1, table[i].name,
                  table[i].score, difficulty_letter((Difficulty)table[i].diff));
}

void ui_scores(const ScoreEntry table[HS_COUNT])
{
    erase();
    attron(COLOR_PAIR(C_TITLE) | A_BOLD);
    ui_center(LINES / 2 - 5, "HALL OF FAME");
    attroff(COLOR_PAIR(C_TITLE) | A_BOLD);
    draw_table(LINES / 2 - 3, table);
    ui_center(LINES / 2 + 3, "[E]asy  [N]ormal  [H]ard");
    ui_center(LINES / 2 + 5, "Press any key to go back");
    refresh();
    getch();
}

void ui_prompt_name(char *buf, size_t size, int score)
{
    erase();
    attron(COLOR_PAIR(C_TITLE) | A_BOLD);
    ui_center(LINES / 2 - 3, "NEW HIGH SCORE!");
    attroff(COLOR_PAIR(C_TITLE) | A_BOLD);
    ui_center(LINES / 2 - 1, "Score: %d   Rank: %s", score, game_rank(score));
    ui_center(LINES / 2 + 1, "Enter your name (max %d chars):", NAME_LEN - 1);
    move(LINES / 2 + 2, COLS / 2 - 6);
    refresh();

    echo();
    curs_set(1);
    buf[0] = '\0';
    getnstr(buf, (int)size - 1);
    noecho();
    curs_set(0);
}

GameOverChoice ui_game_over(const GameResult *r, int new_record,
                            const ScoreEntry table[HS_COUNT])
{
    const char *title =
        r->reason == END_BASE ? "B A S E   D E S T R O Y E D" :
        r->reason == END_SHIP ? "S H I P   D E S T R O Y E D" :
                                "M I S S I O N   A B O R T E D";
    int y = 1;

    erase();
    attron(COLOR_PAIR(C_WARN) | A_BOLD);
    ui_center(y, "%s", title);
    attroff(COLOR_PAIR(C_WARN) | A_BOLD);

    ui_center(y + 2, "Rank: %s", game_rank(r->score));
    ui_center(y + 4,  "Score:         %d", r->score);
    ui_center(y + 5,  "Level reached: %d", r->level);
    ui_center(y + 6,  "Enemies:       %d", r->kills);
    ui_center(y + 7,  "Accuracy:      %d%%", r->accuracy);
    ui_center(y + 8,  "Best combo:    %d", r->max_streak);
    ui_center(y + 9,  "Bosses beaten: %d", r->bosses);
    ui_center(y + 10, "Time:          %d:%02d", r->seconds / 60, r->seconds % 60);
    ui_center(y + 11, "Difficulty:    %s", difficulty_get(r->difficulty)->name);

    if (new_record) {
        attron(COLOR_PAIR(C_BULLET) | A_BOLD);
        ui_center(y + 13, "** NEW RECORD **");
        attroff(COLOR_PAIR(C_BULLET) | A_BOLD);
    }

    ui_center(y + 15, "-- HALL OF FAME --");
    draw_table(y + 16, table);
    ui_center(y + 22, "[R] Play again    [M] Menu    [Q] Quit");
    refresh();

    for (;;) {
        int ch = getch();
        if (ch == 'r' || ch == 'R') return GO_RESTART;
        if (ch == 'm' || ch == 'M') return GO_MENU;
        if (ch == 'q' || ch == 'Q') return GO_QUIT;
    }
}
