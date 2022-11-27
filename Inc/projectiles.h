/*
@file projectiles.h
@author Han Qin Ding
@course CSD1401
@section B
@brief code for projectile entity
*/
/*______________________________________________________________________*/
#pragma once
#include "utils.h"
#include "cprocessing.h"
#include "gametypes.h"
#define PROJ_MELEE_LIFESPAN .4f
#define PROJ_EXPLOSION_FRAME_DT (PROJ_MELEE_LIFESPAN / (float) (EXPLOSION_PROJECTILE_SPRITE_COUNT - 1))
#define PROJ_WEAPON_FRAME_DT (PROJ_MELEE_LIFESPAN / (float) (WEAPON_PROJECTILE_SPRITE_COUNT - 1))
#define MAX_REBOUND_COUNT 2
#define PLAYER_PROJ_SOURCE1 'p'
#define PLAYER_PROJ_SOURCE2 'q'
#define PROJ_TYPE_MOBILE 'm' //can be used by player or enemies
#define PROJ_TYPE_STATIC 's' //can be used by player or enemies
#define PROJ_TYPE_WEAPON 'w' //can be used only by enemies
#define PROJ_VERTICAL_WALL 'x'
#define PROJ_HORIZONTAL_WALL 'y'
#define PROJ_NOT_REBOUNDING 'n'
#define BOSS_PROJ_SOURCE 'b'
#define MOB_PROJ_SOURCE 'e'


entity_struct init_projectile(void);
void set_projectile_values(Projectile* Proj, char Source, char type, int radius, Position Start_Pos, CP_Vector Direction_Vector);
void update_projectile(int index, Entity entities[], int wall_pos[GRID_ROWS][GRID_COLS]);
void deflectprojectiles(char source, int index, Entity entities[]);
int Wall_Edge_Check(Projectile* proj, Position rect, float width, float height);
int Entities_Collision_Check(Projectile* proj, int index, Entity entities[]);
void draw_projectile(Projectile* proj);
