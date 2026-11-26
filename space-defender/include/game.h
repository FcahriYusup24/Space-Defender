#ifndef GAME_H
#define GAME_H
#include "types.h"

Game *game_create(int width, int height, Difficulty diff);
void  game_destroy(Game *g);
void  game_run(Game *g, GameResult *out);
const char *game_rank(int score);

#endif
