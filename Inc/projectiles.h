#pragma once;
#include "utils.h"
#include "cprocessing.h"
#include "gametypes.h"
#define PROJ_MELEE_LIFESPAN .5f
#define MAX_REBOUND_COUNT 2
#define PLAYER_PROJ_SOURCE1 'p'
#define PLAYER_PROJ_SOURCE2 'q'
#define PROJ_TYPE_MOBILE 'm'
#define PROJ_TYPE_STATIC 's'
#define PROJ_VERTICAL_WALL 'x'
#define PROJ_HORIZONTAL_WALL 'y'
#define PROJ_NOT_REBOUNDING 'n'

entity_struct init_projectile(void);
void set_projectile_values(Projectile* Proj, char Source, char type, int radius, Position Start_Pos, CP_Vector Direction_Vector);
void update_projectile(int index, Entity entities[], int wall_pos[GRID_ROWS][GRID_COLS]);
void deflectprojectiles(char source, int index, Entity entities[]);
int Wall_Edge_Check(Projectile* proj, Position rect, float width, float height);
int Entities_Collision_Check(Projectile* proj, int index, Entity entities[]);
