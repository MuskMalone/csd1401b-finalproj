#pragma once;
#include "utils.h"
#include "cprocessing.h"
#include "gametypes.h"
#define PLAYER_PROJ_SOURCE1 'p'
#define PLAYER_PROJ_SOURCE2 'q'

entity_struct init_projectile(void);
void set_projectile_values(Projectile* Proj, char Source, char type, int radius, Position Start_Pos, CP_Vector Direction_Vector);
void update_projectile(int index, Entity entities[], int wall_pos[GRID_ROWS][GRID_COLS]);
void deflectprojectiles(char source, int index, Entity entities[]);
int Wall_Edge_Check(Projectile* proj, Position rect, float width, float height);
int Entities_Collision_Check(Projectile* proj, int index, Entity entities[]);
