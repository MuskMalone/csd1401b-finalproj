#pragma once;
#include "utils.h"
#include "cprocessing.h"
#include "gametypes.h"

Mob init_mob(void);
void update_mob(int mob_idx, int player_idx, Entity entities[]);