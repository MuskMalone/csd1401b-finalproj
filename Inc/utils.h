#pragma once
#include <math.h>
#include "cprocessing.h"
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