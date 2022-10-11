#pragma once
#include <math.h>
#include "cprocessing.h"
#define TRUE 1
#define FALSE 0
// put all structs and enums under ifndef
#ifndef UTILS
typedef struct Position {
	float x;
	float y;
} Position;
#endif
// accesses pointer location for struct position within movable structs
void moveEntity(Position* p, float xspeed, float yspeed);

CP_Vector getVectorBetweenPositions(Position* from, Position* to);

int collisionAABB(Position a_min, Position a_max, Position b_min, Position b_max);

int collisionCircle(Position a_center, float a_radius, Position b_center, float b_radius);
