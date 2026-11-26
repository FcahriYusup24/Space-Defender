#ifndef ENEMY_H
#define ENEMY_H
#include "types.h"

void enemy_spawn(Game *g);
void enemies_update(Game *g);
const char *enemy_sprite(EnemyType t);
int enemy_points(EnemyType t);

#endif
