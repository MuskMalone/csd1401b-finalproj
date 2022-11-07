#pragma once;
#include "utils.h"
#include "cprocessing.h"
#include "gametypes.h"

Mob init_mob(void);
void update_mob(int mob_idx, int player_idx, Entity entities[], int wall_pos[GRID_ROWS][GRID_COLS]);
void expansion_mob_size(Entity entities[], int mob_idx);
