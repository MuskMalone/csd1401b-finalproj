#pragma once;
#include "utils.h"
#include "cprocessing.h"
#include "gametypes.h"
#include "projectiles.h"

// returns the addr of the player intialized 
// to insert into entities
entity_struct init_player(void);
void update_player(int player_idx, Entity entities[], int wall_pos[GRID_ROWS][GRID_COLS]);
int damage_player(Player *p);
int set_player_pos(Player *player, Position pos);
void set_state(Player* p, player_state state);
void draw_player(Player* player, Position offset);