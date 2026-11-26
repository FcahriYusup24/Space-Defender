#ifndef BOSS_H
#define BOSS_H
#include "types.h"

#define BOSS_WIDTH 9
#define BOSS_HEIGHT 3

void boss_spawn(Game *g);
void boss_update(Game *g);
void boss_hit(Game *g, int damage);
const char *boss_row(int row);

#endif
