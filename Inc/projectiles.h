#pragma once;
#include "utils.h"
#include "cprocessing.h"
#include "gametypes.h"

Projectile init_projectile(char Source, Position Start, CP_Vector Direction);
void update_projectile(int index, Entity entities[]);
void deflectprojectiles(char source, int index, Entity entities[], int speed);
