#include "base.h"

void base_damage(Game *g, int amount)
{
    int dmg = amount * g->diff->base_dmg_pct / 100;
    if (dmg < 1)
        dmg = 1;
    g->base_hp -= dmg;
    if (g->base_hp < 0)
        g->base_hp = 0;
    g->base_flash = 6;
    g->streak = 0;
}

void base_repair(Game *g, int amount)
{
    g->base_hp += amount;
    if (g->base_hp > BASE_MAX_HP)
        g->base_hp = BASE_MAX_HP;
}
