#ifndef CONFIG_H
#define CONFIG_H

#define GAME_TITLE "SPACE DEFENDER"

/* Terminal and timing */
#define MIN_WIDTH   70
#define MIN_HEIGHT  24
#define FRAME_MS    33          /* ~30 FPS */

/* Array sizes */
#define MAX_PLAYER_BULLETS 32
#define MAX_ENEMY_BULLETS  48
#define MAX_ENEMIES        24
#define MAX_POWERUPS        8
#define MAX_STARS          50
#define MAX_EXPLOSIONS     32

/* Player */
#define MAX_LIVES        5
#define MAX_BOMBS        3
#define SHOOT_COOLDOWN   5      /* frames */
#define SHIELD_FRAMES  240
#define MULTI_FRAMES   300
#define INVULN_FRAMES   45

/* Base (the thing we defend) */
#define BASE_MAX_HP          100
#define BASE_REPAIR_AMOUNT    25
#define ENEMY_BULLET_BASE_DMG  3

/* Progression */
#ifndef LEVEL_KILLS
#define LEVEL_KILLS      10     /* kills needed per level */
#endif
#define BOSS_EVERY        5     /* boss on every 5th level */
#define POWERUP_CHANCE   15     /* percent drop chance */
#define STREAK_PER_MULT   5     /* kills in a row per +1 multiplier */
#define MAX_MULT          4

/* Effects */
#define EXPLOSION_FRAMES   6
#define BOMB_WAVE_FRAMES  10
#define BOMB_BOSS_DAMAGE  12

/* High scores */
#define HS_COUNT    5
#define NAME_LEN   12
#define HS_FILE    "highscores.txt"

#endif
