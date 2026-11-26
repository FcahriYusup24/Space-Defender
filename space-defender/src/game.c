#include <ncurses.h>
#include <stdlib.h>
#include "game.h"
#include "difficulty.h"
#include "bullet.h"
#include "player.h"
#include "enemy.h"
#include "boss.h"
#include "base.h"
#include "powerup.h"
#include "effects.h"
#include "stars.h"
#include "ui.h"

Game *game_create(int width, int height, Difficulty diff)
{
    Game *g = calloc(1, sizeof *g);          /* dynamic memory */
    if (!g)
        return NULL;

    g->width = width;
    g->height = height;
    g->base_y = height - 2;
    g->base_hp = BASE_MAX_HP;
    g->level = 1;
    g->spawn_timer = 20;
    g->difficulty = diff;
    g->diff = difficulty_get(diff);

    player_init(g);
    stars_init(g);
    banner_set(g, 70, "-- LEVEL 1 --");
    return g;
}

void game_destroy(Game *g)
{
    free(g);
}

const char *game_rank(int score)
{
    if (score <   200) return "Cadet";
    if (score <   600) return "Pilot";
    if (score <  1200) return "Lieutenant";
    if (score <  2500) return "Captain";
    if (score <  5000) return "Commander";
    return "Admiral";
}

/* ---------- scoring ---------- */

static int multiplier(const Game *g)
{
    int m = 1 + g->streak / STREAK_PER_MULT;
    return m > MAX_MULT ? MAX_MULT : m;
}

static void kill_enemy(Game *g, Enemy *e)
{
    e->active = 0;
    g->streak++;
    if (g->streak > g->stats.max_streak)
        g->stats.max_streak = g->streak;

    g->player.score += enemy_points(e->type) * multiplier(g);
    g->kills++;
    g->stats.kills++;
    explosion_add(g, e->x, e->y);

    if (rand() % 100 < POWERUP_CHANCE)
        powerup_spawn(g, e->x, e->y);
}

static void use_bomb(Game *g)
{
    if (g->player.bombs <= 0)
        return;
    g->player.bombs--;
    g->bomb_wave = BOMB_WAVE_FRAMES;

    for (int i = 0; i < MAX_ENEMIES; i++)
        if (g->enemies[i].active)
            kill_enemy(g, &g->enemies[i]);
    for (int i = 0; i < MAX_ENEMY_BULLETS; i++)
        g->enemy_bullets[i].active = 0;
    boss_hit(g, BOMB_BOSS_DAMAGE);
}

/* ---------- input ---------- */

static void handle_input(Game *g)
{
    int ch;
    while ((ch = getch()) != ERR) {
        int playing = !g->paused;
        switch (ch) {
        case KEY_LEFT:  case 'a': case 'A': if (playing) player_move(g, -2); break;
        case KEY_RIGHT: case 'd': case 'D': if (playing) player_move(g,  2); break;
        case ' ':                           if (playing) player_fire(g);     break;
        case 'b': case 'B':                 if (playing) use_bomb(g);        break;
        case 'f': case 'F': g->autofire = !g->autofire; break;
        case 'p': case 'P': g->paused = !g->paused;     break;
        case 'q': case 'Q': g->quit = 1;                break;
        default: break;
        }
    }
}

/* ---------- collisions ---------- */

static void collide_player_bullets(Game *g)
{
    for (int i = 0; i < MAX_PLAYER_BULLETS; i++) {
        Bullet *b = &g->player_bullets[i];
        if (!b->active)
            continue;

        if (g->boss.active &&
            abs(b->x - g->boss.x) <= BOSS_WIDTH / 2 &&
            b->y >= g->boss.y && b->y < g->boss.y + BOSS_HEIGHT) {
            b->active = 0;
            g->stats.hits++;
            boss_hit(g, 1);
            continue;
        }

        for (int j = 0; j < MAX_ENEMIES; j++) {
            Enemy *e = &g->enemies[j];
            if (e->active && abs(b->x - e->x) <= 1 && abs(b->y - e->y) <= 1) {
                b->active = 0;
                g->stats.hits++;
                if (--e->hp <= 0)
                    kill_enemy(g, e);
                break;
            }
        }
    }
}

static void collide_enemy_bullets(Game *g)
{
    for (int i = 0; i < MAX_ENEMY_BULLETS; i++) {
        Bullet *b = &g->enemy_bullets[i];
        if (!b->active)
            continue;
        if (b->y >= g->player.y && abs(b->x - g->player.x) <= 1) {
            b->active = 0;
            player_hit(g);
        } else if (b->y >= g->base_y) {
            b->active = 0;
            base_damage(g, ENEMY_BULLET_BASE_DMG);
        }
    }
}

/* ---------- main update ---------- */

static void update_level(Game *g)
{
    int new_level = 1 + g->kills / LEVEL_KILLS;
    if (new_level > g->level) {
        g->level = new_level;
        if (g->banner_timer <= 0)
            banner_set(g, 70, "-- LEVEL %d --", g->level);
    }

    if (g->level % BOSS_EVERY == 0 && g->boss_done_level < g->level && !g->boss.active) {
        boss_spawn(g);
        banner_set(g, 100, "!! WARNING: BOSS APPROACHING !!");
    }
}

static void spawn_enemies(Game *g)
{
    if (--g->spawn_timer > 0)
        return;

    enemy_spawn(g);

    int interval = 34 - g->level * 3;
    if (interval < 10)
        interval = 10;
    interval = interval * g->diff->spawn_pct / 100;
    if (g->boss.active)
        interval *= 2;
    g->spawn_timer = interval + rand() % 8;
}

static void game_update(Game *g)
{
    g->frame++;

    player_update(g);
    stars_update(g);
    bullets_update(g->player_bullets, MAX_PLAYER_BULLETS, g->frame, g->width, g->height);
    bullets_update(g->enemy_bullets,  MAX_ENEMY_BULLETS,  g->frame, g->width, g->height);

    spawn_enemies(g);
    enemies_update(g);
    boss_update(g);
    powerups_update(g);
    collide_player_bullets(g);
    collide_enemy_bullets(g);
    update_level(g);
    explosions_update(g);

    if (g->banner_timer > 0) g->banner_timer--;
    if (g->bomb_wave > 0)    g->bomb_wave--;
    if (g->base_flash > 0)   g->base_flash--;
}

void game_run(Game *g, GameResult *out)
{
    nodelay(stdscr, TRUE);
    flushinp();

    for (;;) {
        handle_input(g);
        if (g->quit)               { g->end_reason = END_QUIT; break; }
        if (!g->paused)
            game_update(g);
        if (g->base_hp <= 0)       { g->end_reason = END_BASE; break; }
        if (g->player.lives <= 0)  { g->end_reason = END_SHIP; break; }
        ui_draw_game(g);
        napms(FRAME_MS);
    }

    nodelay(stdscr, FALSE);
    flushinp();

    out->score = g->player.score;
    out->level = g->level;
    out->kills = g->stats.kills;
    out->accuracy = g->stats.shots > 0 ? g->stats.hits * 100 / g->stats.shots : 0;
    out->max_streak = g->stats.max_streak;
    out->bosses = g->stats.bosses;
    out->seconds = g->frame * FRAME_MS / 1000;
    out->reason = g->end_reason;
    out->difficulty = g->difficulty;
}
