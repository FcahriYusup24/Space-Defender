#ifndef EFFECTS_H
#define EFFECTS_H
#include "types.h"

void explosion_add(Game *g, int x, int y);
void explosion_big(Game *g, int x, int y);
void explosions_update(Game *g);
void banner_set(Game *g, int frames, const char *fmt, ...);

#endif
