#pragma once;
#include "utils.h"
#include "cprocessing.h"
#include "gametypes.h"

entity_struct init_boss(void);
void update_boss(int boss_idx, int player_idx, Entity entities[], int wall_pos[GRID_ROWS][GRID_COLS]);
void damage_boss(Boss* b);
void Destory_Wall(int wall_pos[GRID_ROWS][GRID_COLS], Position Boss_Pos, int boss_diameter, int parry_rad, int parry_ammo, int wall_width, int wall_height);
void draw_boss(Boss* b);
void Cannon_Fire_Proj( Entity entities[], Player* player);
