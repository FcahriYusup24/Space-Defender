#include <ncurses.h>
#include <stdlib.h>
#include <time.h>
#include "game.h"
#include "highscore.h"
#include "ui.h"

/* Plays missions until the player returns to the menu.
 * Returns 1 to go on with the program, 0 to quit it. */
static int play_session(ScoreEntry table[HS_COUNT], Difficulty diff)
{
    for (;;) {
        if (!ui_ensure_size())
            return 1;

        Game *g = game_create(COLS, LINES, diff);
        if (!g)
            return 0;

        GameResult res;
        game_run(g, &res);
        game_destroy(g);

        int record = hs_qualifies(table, res.score);
        if (record) {
            char name[NAME_LEN];
            ui_prompt_name(name, sizeof name, res.score);
            hs_insert(table, name, res.score, (int)diff);
            hs_save(table);
        }

        GameOverChoice c = ui_game_over(&res, record, table);
        if (c == GO_QUIT) return 0;
        if (c == GO_MENU) return 1;
    }
}

int main(void)
{
    srand((unsigned)time(NULL));
    ui_init();

    ScoreEntry table[HS_COUNT];
    hs_load(table);

    Difficulty diff = DIFF_NORMAL;
    int running = 1;
    while (running) {
        switch (ui_menu(&diff)) {
        case MENU_START:  running = play_session(table, diff); break;
        case MENU_HELP:   ui_help();                           break;
        case MENU_SCORES: ui_scores(table);                    break;
        case MENU_QUIT:   running = 0;                         break;
        }
    }

    ui_shutdown();
    return 0;
}
