#ifndef BULLET_H
#define BULLET_H
#include "types.h"

void bullet_fire(Bullet *arr, int n, int x, int y, int dx, int dy, int period);
void bullets_update(Bullet *arr, int n, int frame, int width, int height);

#endif
