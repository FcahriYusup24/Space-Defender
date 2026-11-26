#ifndef HIGHSCORE_H
#define HIGHSCORE_H
#include "config.h"

typedef struct {
    char name[NAME_LEN];
    int score;
    int diff;          /* Difficulty value */
} ScoreEntry;

void hs_load(ScoreEntry table[HS_COUNT]);
void hs_save(const ScoreEntry table[HS_COUNT]);
int  hs_qualifies(const ScoreEntry table[HS_COUNT], int score);
void hs_insert(ScoreEntry table[HS_COUNT], const char *name, int score, int diff);

#endif
