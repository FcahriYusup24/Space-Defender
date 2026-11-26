#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "highscore.h"

int main(void)
{
    remove(HS_FILE);

    ScoreEntry t[HS_COUNT];
    hs_load(t);
    assert(t[0].score == 0);
    assert(!hs_qualifies(t, 0));
    assert(hs_qualifies(t, 10));

    hs_insert(t, "Ali Baba", 120, 1);
    hs_insert(t, "", 300, 2);
    hs_insert(t, "Zed", 50, 0);
    assert(t[0].score == 300 && strcmp(t[0].name, "PLAYER") == 0);
    assert(t[1].score == 120 && strcmp(t[1].name, "Ali_Baba") == 0);
    assert(t[2].score == 50);

    hs_save(t);
    ScoreEntry u[HS_COUNT];
    hs_load(u);
    for (int i = 0; i < HS_COUNT; i++) {
        assert(strcmp(t[i].name, u[i].name) == 0);
        assert(t[i].score == u[i].score && t[i].diff == u[i].diff);
    }

    for (int s = 1; s <= 10; s++)
        hs_insert(u, "X", s * 100, 1);
    for (int i = 1; i < HS_COUNT; i++)
        assert(u[i - 1].score >= u[i].score);
    assert(u[0].score == 1000 && !hs_qualifies(u, 500));

    remove(HS_FILE);
    puts("highscore: OK");
    return 0;
}
