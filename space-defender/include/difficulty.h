#ifndef DIFFICULTY_H
#define DIFFICULTY_H
#include "types.h"

const DifficultyInfo *difficulty_get(Difficulty d);
char difficulty_letter(Difficulty d);

#endif
