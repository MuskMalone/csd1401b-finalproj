#pragma once;
#include "utils.h"
#include "cprocessing.h"
#include "gametypes.h"
#include "projectiles.h"
#define COOLDOWN_DURATION 2.0f
#define MAX_COOLDOWN 3.0f
#define MAX_PARRYRADIUS WALL_DIM * 1.4f
#define PLAYER_DIAMETER 1.25f * WALL_DIM
#define DASH_DURATION .15f
#define STAMINA_COST 70.0f
#define STAMINA_COST_HOLD 90.0f * CP_System_GetDt()
#define NORMAL_SPEED WALL_DIM * 2.0f
#define DASH_SPEED NORMAL_SPEED*5.0f
#define HOLDING_PROJ_SPEED 50.0f
#define ACCESS_ALL_ENTITIES for (int i = 0; i < ENTITY_CAP; ++i) 
#define MELEE_DEFLECT_SHAKE 150.0f
#define PROJ_DEFLECT_SHAKE 50.0f
#define PROJ_HOLDING_SHAKE 2.0f
#define DAMAGE_SHAKE 15.0f
#define DAMAGE_TINT_TIMER .5f

// returns the addr of the player intialized 
// to insert into entities
entity_struct init_player(void);
void update_player(int player_idx, Entity entities[], int wall_pos[GRID_ROWS][GRID_COLS]);
int damage_player(Player *p);
int set_player_pos(Player *player, Position pos);
void set_state(Player* p, player_state state);
void draw_player(Player* player);