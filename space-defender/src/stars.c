#include <stdlib.h>
#include "stars.h"

void stars_init(Game *g)
{
    for (int i = 0; i < MAX_STARS; i++) {
        g->stars[i].x = rand() % g->width;
        g->stars[i].y = 2 + rand() % (g->height - 5);
        g->stars[i].speed = 2 + rand() % 4;
    }
}

void stars_update(Game *g)
{
    for (int i = 0; i < MAX_STARS; i++) {
        Star *s = &g->stars[i];
        if (g->frame % s->speed == 0 && ++s->y >= g->base_y) {
            s->y = 2;
            s->x = rand() % g->width;
        }
    }
}
