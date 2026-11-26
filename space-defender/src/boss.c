#include <stdlib.h>
#include "boss.h"
#include "bullet.h"
#include "effects.h"
#include "powerup.h"

static const char *ROWS[BOSS_HEIGHT] = {
    " /=====\\ ",
    "<|(O O)|>",
    " \\_VVV_/ ",
};

const char *boss_row(int row) { return ROWS[row]; }

void boss_spawn(Game *g)
{
    Boss *b = &g->boss;
    b->active = 1;
    b->number = g->stats.bosses + 1;
    b->max_hp = 20 + 12 * b->number;
    b->hp = b->max_hp;
    b->x = g->width / 2;
    b->y = 3;
    b->dir = 1;
    b->tick = 0;
    b->fire_timer = 45;
}

static void boss_fire(Game *g)
{
    Boss *b = &g->boss;
    int y = b->y + BOSS_HEIGHT;
    int enraged = b->hp * 2 < b->max_hp;

    bullet_fire(g->enemy_bullets, MAX_ENEMY_BULLETS, b->x,     y,  0, 1, 2);
    bullet_fire(g->enemy_bullets, MAX_ENEMY_BULLETS, b->x - 3, y, -1, 1, 2);
    bullet_fire(g->enemy_bullets, MAX_ENEMY_BULLETS, b->x + 3, y,  1, 1, 2);
    if (enraged) {
        bullet_fire(g->enemy_bullets, MAX_ENEMY_BULLETS, b->x - 4, y, -2, 1, 2);
        bullet_fire(g->enemy_bullets, MAX_ENEMY_BULLETS, b->x + 4, y,  2, 1, 2);
    }
}

void boss_update(Game *g)
{
    Boss *b = &g->boss;
    if (!b->active)
        return;

    if (++b->tick >= 2) {
        b->tick = 0;
        b->x += b->dir;
        if (b->x - BOSS_WIDTH / 2 <= 1 || b->x + BOSS_WIDTH / 2 >= g->width - 2)
            b->dir = -b->dir;
    }

    if (--b->fire_timer <= 0) {
        boss_fire(g);
        b->fire_timer = (b->hp * 2 < b->max_hp) ? 22 : 38;
    }
}

static void boss_defeat(Game *g)
{
    Boss *b = &g->boss;
    int reward = 500 * b->number;

    b->active = 0;
    g->player.score += reward;
    g->stats.bosses++;
    g->boss_done_level = g->level;
    g->kills = g->level * LEVEL_KILLS;       /* jump to the next level */
    g->streak += 5;

    explosion_big(g, b->x, b->y);
    powerup_spawn_type(g, b->x - 3, b->y + BOSS_HEIGHT, POWERUP_BOMB);
    powerup_spawn_type(g, b->x + 3, b->y + BOSS_HEIGHT, POWERUP_REPAIR);
    banner_set(g, 100, "BOSS DEFEATED!  +%d", reward);
}

void boss_hit(Game *g, int damage)
{
    Boss *b = &g->boss;
    if (!b->active)
        return;
    b->hp -= damage;
    if (b->hp <= 0)
        boss_defeat(g);
}
