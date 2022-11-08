#pragma once;
#include "utils.h"
#include "cprocessing.h"
#include "gametypes.h"

entity_struct init_mob(void);
void update_mob(int mob_idx, int player_idx, Entity entities[]);
void expansion_mob_size(Entity entities[], int mob_idx);
//void spawn_Mob(void);
