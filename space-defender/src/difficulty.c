#include "difficulty.h"

static const DifficultyInfo TABLE[DIFF_COUNT] = {
    /* name      spawn  base  lives bombs */
    { "Easy",    130,    70,   4,    2 },
    { "Normal",  100,   100,   3,    1 },
    { "Hard",     75,   140,   2,    1 },
};

const DifficultyInfo *difficulty_get(Difficulty d)
{
    if (d < 0 || d >= DIFF_COUNT)
        d = DIFF_NORMAL;
    return &TABLE[d];
}

char difficulty_letter(Difficulty d)
{
    return difficulty_get(d)->name[0];
}
