#include <stdio.h>
#include <string.h>
#include "highscore.h"
#include "types.h"

static void clear_table(ScoreEntry table[HS_COUNT])
{
    for (int i = 0; i < HS_COUNT; i++) {
        strcpy(table[i].name, "---");
        table[i].score = 0;
        table[i].diff = DIFF_NORMAL;
    }
}

void hs_load(ScoreEntry table[HS_COUNT])
{
    clear_table(table);

    FILE *f = fopen(HS_FILE, "r");
    if (!f)
        return;

    for (int i = 0; i < HS_COUNT; i++) {
        char name[NAME_LEN];
        int score, diff;
        if (fscanf(f, "%11s %d %d", name, &score, &diff) != 3)
            break;
        if (diff < 0 || diff >= DIFF_COUNT)
            diff = DIFF_NORMAL;
        strcpy(table[i].name, name);
        table[i].score = score;
        table[i].diff = diff;
    }
    fclose(f);
}

void hs_save(const ScoreEntry table[HS_COUNT])
{
    FILE *f = fopen(HS_FILE, "w");
    if (!f)
        return;
    for (int i = 0; i < HS_COUNT; i++)
        fprintf(f, "%s %d %d\n", table[i].name, table[i].score, table[i].diff);
    fclose(f);
}

int hs_qualifies(const ScoreEntry table[HS_COUNT], int score)
{
    return score > 0 && score > table[HS_COUNT - 1].score;
}

void hs_insert(ScoreEntry table[HS_COUNT], const char *name, int score, int diff)
{
    ScoreEntry e;
    snprintf(e.name, sizeof e.name, "%s", name);
    for (char *c = e.name; *c; c++)
        if (*c == ' ' || *c == '\t')
            *c = '_';
    if (e.name[0] == '\0')
        strcpy(e.name, "PLAYER");
    e.score = score;
    e.diff = diff;

    int pos = HS_COUNT - 1;
    while (pos > 0 && table[pos - 1].score < score) {
        table[pos] = table[pos - 1];
        pos--;
    }
    table[pos] = e;
}
