#include <stdlib.h>
#include "enemy.h"
#include "bullet.h"
#include "player.h"
#include "base.h"
#include "effects.h"

typedef struct {
    const char *sprite;
    int hp;
    int base_speed;
    int points;
    int base_dmg;      /* damage to the base if it gets through */
} EnemyInfo;

static const EnemyInfo INFO[ENEMY_TYPE_COUNT] = {
    /* sprite  hp speed pts dmg */
    { "\\W/",   1,  9,  10,  8 },   /* basic   */
    { "<V>",    1,  5,  20,  8 },   /* fast    */
    { "[#]",    3, 16,  40, 15 },   /* tank    */
    { "{S}",    2, 12,  30, 10 },   /* shooter */
};

const char *enemy_sprite(EnemyType t) { return INFO[t].sprite; }
int enemy_points(EnemyType t)         { return INFO[t].points; }

static EnemyType pick_type(int level)
{
    int r = rand() % 100;
    if (level >= 4 && r < 15) return ENEMY_SHOOTER;
    if (level >= 3 && r < 35) return ENEMY_TANK;
    if (level >= 2 && r < 60) return ENEMY_FAST;
    return ENEMY_BASIC;
}

void enemy_spawn(Game *g)
{
    for (int i = 0; i < MAX_ENEMIES; i++) {
        Enemy *e = &g->enemies[i];
        if (e->active)
            continue;

        EnemyType t = pick_type(g->level);
        int speed = INFO[t].base_speed - g->level / 2;
        if (speed < 2)
            speed = 2;

        e->active = 1;
        e->type = t;
        e->x = 2 + rand() % (g->width - 4);
        e->y = 1;
        e->hp = INFO[t].hp;
        e->speed = speed;
        e->tick = rand() % speed;
        e->fire_timer = 40 + rand() % 60;
        return;
    }
}

void enemies_update(Game *g)
{
    for (int i = 0; i < MAX_ENEMIES; i++) {
        Enemy *e = &g->enemies[i];
        if (!e->active)
            continue;

        if (++e->tick >= e->speed) {
            e->tick = 0;
            e->y++;
            if (e->type == ENEMY_FAST) {             /* zigzag */
                e->x += rand() % 3 - 1;
                if (e->x < 2) e->x = 2;
                if (e->x > g->width - 3) e->x = g->width - 3;
            }
        }

        if (e->type == ENEMY_SHOOTER && e->y < g->player.y - 3) {
            if (--e->fire_timer <= 0) {
                bullet_fire(g->enemy_bullets, MAX_ENEMY_BULLETS,
                            e->x, e->y + 1, 0, 1, 2);
                e->fire_timer = 50 + rand() % 50;
            }
        }

        if (e->y >= g->player.y && abs(e->x - g->player.x) <= 2) {
            e->active = 0;                            /* rammed the ship */
            explosion_add(g, e->x, e->y);
            player_hit(g);
        } else if (e->y >= g->base_y) {
            e->active = 0;                            /* hit the base */
            explosion_add(g, e->x, e->y);
            base_damage(g, INFO[e->type].base_dmg);
        }
    }
}
