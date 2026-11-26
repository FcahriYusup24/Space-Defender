#include "player.h"
#include "bullet.h"
#include "effects.h"

void player_init(Game *g)
{
    Player *p = &g->player;
    p->x = g->width / 2;
    p->y = g->base_y - 1;
    p->lives = g->diff->lives;
    p->bombs = g->diff->bombs;
    p->score = 0;
    p->cooldown = 0;
    p->shield = 0;
    p->multishot = 0;
    p->invuln = 0;
}

void player_move(Game *g, int dx)
{
    int nx = g->player.x + dx;
    if (nx < 2)
        nx = 2;
    if (nx > g->width - 3)
        nx = g->width - 3;
    g->player.x = nx;
}

static void fire_one(Game *g, int x, int dx)
{
    bullet_fire(g->player_bullets, MAX_PLAYER_BULLETS, x, g->player.y - 1, dx, -1, 1);
    g->stats.shots++;
}

void player_fire(Game *g)
{
    Player *p = &g->player;
    if (p->cooldown > 0)
        return;
    p->cooldown = SHOOT_COOLDOWN;

    fire_one(g, p->x, 0);
    if (p->multishot > 0) {
        fire_one(g, p->x - 1, -1);
        fire_one(g, p->x + 1,  1);
    }
}

void player_update(Game *g)
{
    Player *p = &g->player;
    if (p->cooldown > 0)  p->cooldown--;
    if (p->shield > 0)    p->shield--;
    if (p->multishot > 0) p->multishot--;
    if (p->invuln > 0)    p->invuln--;

    if (g->autofire)
        player_fire(g);
}

void player_hit(Game *g)
{
    Player *p = &g->player;
    if (p->shield > 0 || p->invuln > 0)
        return;
    p->lives--;
    p->invuln = INVULN_FRAMES;
    g->streak = 0;
    explosion_add(g, p->x, p->y);
}
