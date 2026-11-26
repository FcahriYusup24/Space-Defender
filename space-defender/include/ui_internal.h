#ifndef UI_INTERNAL_H
#define UI_INTERNAL_H

/* Colour pairs shared by the ui_*.c files */
enum {
    C_PLAYER = 1, C_BULLET, C_EBULLET, C_STAR, C_TITLE, C_WARN,
    C_BASIC, C_FAST, C_TANK, C_SHOOTER, C_BOSS, C_GOOD
};

void ui_center(int y, const char *fmt, ...) __attribute__((format(printf, 2, 3)));

#endif
