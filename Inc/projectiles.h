#pragma once;
#include "utils.h"
#include "cprocessing.h"
#include "gametypes.h"

Projectile init_projectile(char Source, Position Start, CP_Vector Direction, char type);
void update_projectile(int index, Entity entities[], int wall_pos[GRID_ROWS][GRID_COLS]);
void deflectprojectiles(char source, int index, Entity entities[]);
int Wall_Edge_Check(Projectile* proj, Position rect, float width, float height);
int Entities_Collision_Check(Projectile* proj, int index, Entity entities[]);
