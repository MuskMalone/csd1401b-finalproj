#pragma once
#include <math.h>
#include "cprocessing.h"
typedef struct Position {
	float x;
	float y;
} Position;

// accesses pointer location for struct position within movable structs
void moveEntity(Position* p, float xspeed, float yspeed);