#ifndef PLAYER_H
#define PLAYER_H
#include "types.h"

void player_init(Game *g);
void player_move(Game *g, int dx);
void player_fire(Game *g);
void player_update(Game *g);
void player_hit(Game *g);

#endif
