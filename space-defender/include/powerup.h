#ifndef POWERUP_H
#define POWERUP_H
#include "types.h"

void powerup_spawn(Game *g, int x, int y);
void powerup_spawn_type(Game *g, int x, int y, PowerUpType t);
void powerups_update(Game *g);
const char *powerup_sprite(PowerUpType t);

#endif
