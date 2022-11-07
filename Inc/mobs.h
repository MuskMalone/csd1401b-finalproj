#pragma once;
#include "utils.h"
#include "cprocessing.h"
#include "gametypes.h"

<<<<<<< HEAD
entity_struct init_mob(void);
void update_mob(int mob_idx, int player_idx, Entity entities[], int wall_pos[GRID_ROWS][GRID_COLS]);
=======
Mob init_mob(void);
void update_mob(int mob_idx, int player_idx, Entity entities[]);
>>>>>>> parent of 2d4b2e2 (add to entity array)
void expansion_mob_size(Entity entities[], int mob_idx);
