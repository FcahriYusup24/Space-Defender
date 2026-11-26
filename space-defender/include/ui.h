#ifndef UI_H
#define UI_H
#include <stddef.h>
#include "types.h"
#include "highscore.h"

typedef enum { MENU_START, MENU_HELP, MENU_SCORES, MENU_QUIT } MenuChoice;
typedef enum { GO_RESTART, GO_MENU, GO_QUIT } GameOverChoice;

/* ui_core.c */
void ui_init(void);
void ui_shutdown(void);
int  ui_ensure_size(void);

/* ui_menu.c */
MenuChoice ui_menu(Difficulty *diff);

/* ui_game.c */
void ui_draw_game(const Game *g);

/* ui_screens.c */
void ui_help(void);
void ui_scores(const ScoreEntry table[HS_COUNT]);
void ui_prompt_name(char *buf, size_t size, int score);
GameOverChoice ui_game_over(const GameResult *r, int new_record,
                            const ScoreEntry table[HS_COUNT]);

#endif
