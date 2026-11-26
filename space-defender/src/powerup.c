#include <stdlib.h>
#include "powerup.h"
#include "base.h"

const char *powerup_sprite(PowerUpType t)
{
    switch (t) {
    case POWERUP_LIFE:   return "[L]";
    case POWERUP_SHIELD: return "[S]";
    case POWERUP_MULTI:  return "[3]";
    case POWERUP_BOMB:   return "[B]";
    case POWERUP_REPAIR: return "[R]";
    default:             return "[?]";
    }
}

void powerup_spawn_type(Game *g, int x, int y, PowerUpType t)
{
    for (int i = 0; i < MAX_POWERUPS; i++) {
        PowerUp *p = &g->powerups[i];
        if (!p->active) {
            p->active = 1;
            p->x = x;
            p->y = y;
            p->type = t;
            return;
        }
    }
}

void powerup_spawn(Game *g, int x, int y)
{
    int r = rand() % 100;
    PowerUpType t;

    if      (r < 10) t = POWERUP_LIFE;
    else if (r < 35) t = POWERUP_SHIELD;
    else if (r < 60) t = POWERUP_MULTI;
    else if (r < 75) t = POWERUP_BOMB;
    else             t = POWERUP_REPAIR;

    powerup_spawn_type(g, x, y, t);
}

static void apply(Game *g, PowerUpType t)
{
    Player *pl = &g->player;
    switch (t) {
    case POWERUP_LIFE:
        if (pl->lives < MAX_LIVES) pl->lives++;
        else                       pl->score += 50;
        break;
    case POWERUP_SHIELD:
        pl->shield = SHIELD_FRAMES;
        break;
    case POWERUP_MULTI:
        pl->multishot = MULTI_FRAMES;
        break;
    case POWERUP_BOMB:
        if (pl->bombs < MAX_BOMBS) pl->bombs++;
        else                       pl->score += 50;
        break;
    case POWERUP_REPAIR:
        base_repair(g, BASE_REPAIR_AMOUNT);
        break;
    default:
        break;
    }
}

void powerups_update(Game *g)
{
    for (int i = 0; i < MAX_POWERUPS; i++) {
        PowerUp *p = &g->powerups[i];
        if (!p->active)
            continue;

        if (g->frame % 6 == 0)
            p->y++;

        if (p->y >= g->player.y) {
            if (abs(p->x - g->player.x) <= 2)
                apply(g, p->type);
            p->active = 0;
        }
    }
}
