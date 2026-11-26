#ifndef TYPES_H
#define TYPES_H

#include "config.h"

/* ---------- Difficulty ---------- */
typedef enum { DIFF_EASY, DIFF_NORMAL, DIFF_HARD, DIFF_COUNT } Difficulty;

typedef struct {
    const char *name;
    int spawn_pct;       /* spawn interval multiplier (higher = slower) */
    int base_dmg_pct;    /* damage taken by the base */
    int lives;           /* starting ships */
    int bombs;           /* starting bombs */
} DifficultyInfo;

typedef enum { END_QUIT, END_SHIP, END_BASE } EndReason;

/* ---------- Entities ---------- */
typedef struct {
    int x, y;
    int lives;
    int score;
    int bombs;
    int cooldown;
    int shield;        /* frames left */
    int multishot;     /* frames left */
    int invuln;        /* frames left */
} Player;

typedef struct {
    int x, y;
    int dx, dy;
    int period;        /* move every N frames */
    int active;
} Bullet;

typedef enum {
    ENEMY_BASIC,
    ENEMY_FAST,
    ENEMY_TANK,
    ENEMY_SHOOTER,
    ENEMY_TYPE_COUNT
} EnemyType;

typedef struct {
    int x, y;
    int active;
    int hp;
    int speed;         /* frames per row */
    int tick;
    int fire_timer;
    EnemyType type;
} Enemy;

typedef struct {
    int active;
    int x, y;          /* x = centre, y = top row */
    int hp, max_hp;
    int dir;
    int tick;
    int fire_timer;
    int number;        /* 1st, 2nd, ... boss */
} Boss;

typedef enum {
    POWERUP_LIFE,
    POWERUP_SHIELD,
    POWERUP_MULTI,
    POWERUP_BOMB,
    POWERUP_REPAIR,
    POWERUP_TYPE_COUNT
} PowerUpType;

typedef struct {
    int x, y;
    int active;
    PowerUpType type;
} PowerUp;

typedef struct { int x, y, speed; } Star;
typedef struct { int x, y, timer; } Explosion;

/* ---------- Statistics and result ---------- */
typedef struct {
    int shots;
    int hits;
    int kills;
    int max_streak;
    int bosses;
} Stats;

typedef struct {
    int score;
    int level;
    int kills;
    int accuracy;      /* percent */
    int max_streak;
    int bosses;
    int seconds;
    EndReason reason;
    Difficulty difficulty;
} GameResult;

/* ---------- Whole game state ---------- */
typedef struct {
    Player    player;
    Bullet    player_bullets[MAX_PLAYER_BULLETS];
    Bullet    enemy_bullets[MAX_ENEMY_BULLETS];
    Enemy     enemies[MAX_ENEMIES];
    PowerUp   powerups[MAX_POWERUPS];
    Star      stars[MAX_STARS];
    Explosion explosions[MAX_EXPLOSIONS];
    Boss      boss;
    Stats     stats;

    int width, height;
    int base_y;
    int base_hp;
    int base_flash;
    int level;
    int kills;             /* progress towards next level */
    int streak;
    int frame;
    int spawn_timer;
    int bomb_wave;
    int boss_done_level;
    int banner_timer;
    char banner[48];
    int paused;
    int autofire;
    int quit;
    EndReason end_reason;
    Difficulty difficulty;
    const DifficultyInfo *diff;
} Game;

#endif
