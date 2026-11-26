#include "bullet.h"

void bullet_fire(Bullet *arr, int n, int x, int y, int dx, int dy, int period)
{
    for (int i = 0; i < n; i++) {
        if (!arr[i].active) {
            arr[i].active = 1;
            arr[i].x = x;
            arr[i].y = y;
            arr[i].dx = dx;
            arr[i].dy = dy;
            arr[i].period = period;
            return;
        }
    }
}

void bullets_update(Bullet *arr, int n, int frame, int width, int height)
{
    for (int i = 0; i < n; i++) {
        Bullet *b = &arr[i];
        if (!b->active)
            continue;

        if (frame % b->period == 0) {
            b->y += b->dy;
            /* diagonal bullets drift sideways every other step */
            if (b->dx != 0 && ((frame / b->period) & 1) == 0)
                b->x += b->dx;
        }

        if (b->y < 1 || b->y >= height - 1 || b->x < 0 || b->x >= width)
            b->active = 0;
    }
}
