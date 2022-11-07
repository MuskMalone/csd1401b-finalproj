#pragma once;
#include "utils.h"
#include "cprocessing.h"
#include "gametypes.h"
#include "projectiles.h"

// returns the addr of the player intialized 
// to insert into entities
Player init_player(void);
void update_player(int player_idx, Entity entities[], int wall_pos[GRID_ROWS][GRID_COLS]);
int damage_player(Player *p);