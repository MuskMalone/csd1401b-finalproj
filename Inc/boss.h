#pragma once;
#include "utils.h"
#include "cprocessing.h"
#include "gametypes.h"

Boss init_boss(void);
void update_boss(int boss_idx, Entity entities[]);