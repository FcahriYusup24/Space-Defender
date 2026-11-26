#include <assert.h>
#include <stdio.h>
#include "difficulty.h"

int main(void)
{
    assert(difficulty_get(DIFF_EASY)->lives > difficulty_get(DIFF_HARD)->lives);
    assert(difficulty_get(DIFF_HARD)->base_dmg_pct > difficulty_get(DIFF_EASY)->base_dmg_pct);
    assert(difficulty_get(DIFF_HARD)->spawn_pct < difficulty_get(DIFF_EASY)->spawn_pct);
    assert(difficulty_get((Difficulty)99) == difficulty_get(DIFF_NORMAL));
    assert(difficulty_letter(DIFF_EASY) == 'E');
    assert(difficulty_letter(DIFF_NORMAL) == 'N');
    assert(difficulty_letter(DIFF_HARD) == 'H');
    puts("difficulty: OK");
    return 0;
}
