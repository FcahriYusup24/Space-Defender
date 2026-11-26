#include <stdarg.h>
#include <stdio.h>
#include "effects.h"

void explosion_add(Game *g, int x, int y)
{
    for (int i = 0; i < MAX_EXPLOSIONS; i++) {
        if (g->explosions[i].timer <= 0) {
            g->explosions[i].x = x;
            g->explosions[i].y = y;
            g->explosions[i].timer = EXPLOSION_FRAMES;
            return;
        }
    }
}

void explosion_big(Game *g, int x, int y)
{
    for (int dy = -1; dy <= 3; dy += 2)
        for (int dx = -6; dx <= 6; dx += 3)
            explosion_add(g, x + dx, y + dy);
}

void explosions_update(Game *g)
{
    for (int i = 0; i < MAX_EXPLOSIONS; i++)
        if (g->explosions[i].timer > 0)
            g->explosions[i].timer--;
}

void banner_set(Game *g, int frames, const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    vsnprintf(g->banner, sizeof g->banner, fmt, ap);
    va_end(ap);
    g->banner_timer = frames;
}
