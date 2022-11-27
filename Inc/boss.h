/*
@file boss.h
@author Han Qin Ding
@course CSD1401
@section B
@brief code for boss entity
*/
/*______________________________________________________________________*/
#pragma once
#include "utils.h"
#include "cprocessing.h"
#include "gametypes.h"

entity_struct init_boss(void);
void update_boss(int boss_idx, int player_idx, Entity entities[], int wall_pos[GRID_ROWS][GRID_COLS]);
void damage_boss(Boss* b, Player* p);
void Destroy_Wall(int wall_pos[GRID_ROWS][GRID_COLS], Position Boss_Pos, float boss_diameter, float parry_rad, float parry_ammo, float wall_width, float wall_height);
void draw_boss(Boss* b);
void Cannon_Fire_Proj( Entity entities[], Player* player);
