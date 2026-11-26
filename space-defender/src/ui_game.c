#include <ncurses.h>
#include <string.h>
#include "ui.h"
#include "ui_internal.h"
#include "enemy.h"
#include "boss.h"
#include "powerup.h"

static int enemy_pair(EnemyType t)
{
    switch (t) {
    case ENEMY_BASIC: return C_BASIC;
    case ENEMY_FAST:  return C_FAST;
    case ENEMY_TANK:  return C_TANK;
    default:          return C_SHOOTER;
    }
}

static int powerup_pair(PowerUpType t)
{
    switch (t) {
    case POWERUP_LIFE:   return C_WARN;
    case POWERUP_SHIELD: return C_PLAYER;
    case POWERUP_MULTI:  return C_BULLET;
    case POWERUP_BOMB:   return C_TANK;
    default:             return C_GOOD;
    }
}

static int base_pair(int hp)
{
    if (hp > 60) return C_GOOD;
    if (hp > 30) return C_BULLET;
    return C_WARN;
}

static void draw_base(const Game *g)
{
    int filled = g->width * g->base_hp / BASE_MAX_HP;
    int pair = base_pair(g->base_hp);
    int flash = g->base_flash > 0 && (g->base_flash & 1);

    attron(COLOR_PAIR(pair) | A_BOLD | (flash ? A_REVERSE : 0));
    for (int x = 0; x < filled; x++)
        mvaddch(g->base_y, x, '=');
    attroff(COLOR_PAIR(pair) | A_BOLD | A_REVERSE);

    attron(COLOR_PAIR(C_WARN));
    for (int x = filled; x < g->width; x++)
        mvaddch(g->base_y, x, '.');
    attroff(COLOR_PAIR(C_WARN));
}

static void draw_boss(const Game *g)
{
    const Boss *b = &g->boss;
    attron(COLOR_PAIR(C_BOSS) | A_BOLD);
    for (int r = 0; r < BOSS_HEIGHT; r++)
        mvprintw(b->y + r, b->x - BOSS_WIDTH / 2, "%s", boss_row(r));

    int cells = 24;
    int filled = b->hp * cells / b->max_hp;
    if (filled < 1) filled = 1;
    move(1, (g->width - (cells + 8)) / 2);
    printw("BOSS [");
    for (int i = 0; i < cells; i++)
        addch(i < filled ? '#' : '-');
    printw("]");
    attroff(COLOR_PAIR(C_BOSS) | A_BOLD);
}

static void draw_hud(const Game *g)
{
    const Player *p = &g->player;
    int mult = 1 + g->streak / STREAK_PER_MULT;
    if (mult > MAX_MULT) mult = MAX_MULT;

    attron(A_BOLD);
    mvprintw(0, 1, "SCORE %06d  LEVEL %d  SHIP ", p->score, g->level);
    attroff(A_BOLD);

    attron(COLOR_PAIR(C_PLAYER) | A_BOLD);
    for (int i = 0; i < p->lives; i++)
        addch('A');
    attroff(COLOR_PAIR(C_PLAYER) | A_BOLD);

    if (mult > 1) {
        attron(COLOR_PAIR(C_BULLET) | A_BOLD);
        printw("  COMBO x%d", mult);
        attroff(COLOR_PAIR(C_BULLET) | A_BOLD);
    }

    int x = g->width - 1;
    if (g->autofire)      { x -= 7;  mvprintw(0, x, "[AUTO] "); }
    if (p->multishot > 0) { x -= 9;  mvprintw(0, x, "[TRIPLE] "); }
    if (p->shield > 0)    { x -= 9;  mvprintw(0, x, "[SHIELD] "); }

    /* bottom status line */
    mvprintw(g->height - 1, 1, "BASE %3d%%   BOMBS ", g->base_hp);
    attron(COLOR_PAIR(C_TANK) | A_BOLD);
    for (int i = 0; i < p->bombs; i++)
        printw("[B]");
    attroff(COLOR_PAIR(C_TANK) | A_BOLD);
    printw("%*s", (3 - p->bombs) * 3, "");
    printw("  Arrows/AD move  SPACE fire  B bomb  F auto  P pause  Q quit");
}

void ui_draw_game(const Game *g)
{
    const Player *p = &g->player;
    erase();

    attron(COLOR_PAIR(C_STAR));
    for (int i = 0; i < MAX_STARS; i++)
        mvaddch(g->stars[i].y, g->stars[i].x, g->stars[i].speed <= 3 ? '*' : '.');
    attroff(COLOR_PAIR(C_STAR));

    draw_base(g);

    for (int i = 0; i < MAX_POWERUPS; i++) {
        const PowerUp *u = &g->powerups[i];
        if (!u->active)
            continue;
        attron(COLOR_PAIR(powerup_pair(u->type)) | A_BOLD);
        mvprintw(u->y, u->x - 1, "%s", powerup_sprite(u->type));
        attroff(COLOR_PAIR(powerup_pair(u->type)) | A_BOLD);
    }

    for (int i = 0; i < MAX_ENEMIES; i++) {
        const Enemy *e = &g->enemies[i];
        if (!e->active)
            continue;
        attron(COLOR_PAIR(enemy_pair(e->type)) | A_BOLD);
        mvprintw(e->y, e->x - 1, "%s", enemy_sprite(e->type));
        attroff(COLOR_PAIR(enemy_pair(e->type)) | A_BOLD);
    }

    if (g->boss.active)
        draw_boss(g);

    attron(COLOR_PAIR(C_BULLET) | A_BOLD);
    for (int i = 0; i < MAX_PLAYER_BULLETS; i++) {
        const Bullet *b = &g->player_bullets[i];
        if (b->active)
            mvaddch(b->y, b->x, b->dx < 0 ? '\\' : b->dx > 0 ? '/' : '|');
    }
    attroff(COLOR_PAIR(C_BULLET) | A_BOLD);

    attron(COLOR_PAIR(C_EBULLET) | A_BOLD);
    for (int i = 0; i < MAX_ENEMY_BULLETS; i++)
        if (g->enemy_bullets[i].active)
            mvaddch(g->enemy_bullets[i].y, g->enemy_bullets[i].x, 'v');
    attroff(COLOR_PAIR(C_EBULLET) | A_BOLD);

    static const char *EXPL[3] = { " * ", "\\*/", ". ." };
    attron(COLOR_PAIR(C_BULLET) | A_BOLD);
    for (int i = 0; i < MAX_EXPLOSIONS; i++) {
        const Explosion *x = &g->explosions[i];
        if (x->timer > 0) {
            int phase = (EXPLOSION_FRAMES - x->timer) / 2;
            mvprintw(x->y, x->x - 1, "%s", EXPL[phase > 2 ? 2 : phase]);
        }
    }
    attroff(COLOR_PAIR(C_BULLET) | A_BOLD);

    if (g->bomb_wave > 0) {
        int row = p->y - (BOMB_WAVE_FRAMES - g->bomb_wave) * (p->y - 1) / BOMB_WAVE_FRAMES;
        attron(COLOR_PAIR(C_TANK) | A_BOLD);
        mvhline(row, 0, '~', g->width);
        attroff(COLOR_PAIR(C_TANK) | A_BOLD);
    }

    int blink_off = (p->invuln > 0 && p->shield == 0 && (g->frame / 3) % 2 == 0);
    if (!blink_off) {
        attron(COLOR_PAIR(C_PLAYER) | A_BOLD);
        mvprintw(p->y, p->x - 1, "%s", p->shield > 0 ? "(A)" : "/A\\");
        attroff(COLOR_PAIR(C_PLAYER) | A_BOLD);
    }

    draw_hud(g);

    if (g->banner_timer > 0) {
        int warn = g->banner[0] == '!';
        attron(COLOR_PAIR(warn ? C_WARN : C_TITLE) | A_BOLD);
        if (!warn || (g->banner_timer / 6) % 2 == 0)
            ui_center(g->height / 2 - 2, "%s", g->banner);
        attroff(COLOR_PAIR(warn ? C_WARN : C_TITLE) | A_BOLD);
    }
    if (g->paused) {
        attron(A_REVERSE | A_BOLD);
        ui_center(g->height / 2, "  PAUSED - press P to resume  ");
        attroff(A_REVERSE | A_BOLD);
    }
    refresh();
}
