#pragma once
#include <math.h>
#include "cprocessing.h"
#define TRUE 1
#define FALSE 0
# define M_PI           3.14159265358979323846  /* pi */
// put all structs and enums under ifndef
#ifndef UTILS
typedef struct Position {
	float x;
	float y;
} Position;
#endif
// accesses pointer location for struct position within movable structs
void moveEntity(Position* p, float xspeed, float yspeed);

CP_Vector getVectorBetweenPositions(Position from, Position to);

// aabb collision
// works for collision between 
// 2 unrotated 4-side right-angle polygons (square/rectangle)
int collisionAABB(Position a_min, Position a_max, Position b_min, Position b_max);

// circle collision 
// works for collision between 2 circles
int collisionCircle(Position a_center, float a_radius, Position b_center, float b_radius);

// circle to rectangle collision
// works for collision between a circle and a rectangle
int collisionCircleRect(Position circle, float radius, Position rect, float width, float height);

float positionDistance(Position a, Position b);

//void DeflectProjectiles(char source, int projectile_index, Entity entities[]);

int IsAreaClicked(float area_center_x, float area_center_y, float area_width, float area_height, float click_x, float click_y);

int IsCircleClicked(float circle_center_x, float circle_center_y, float diameter, float click_x, float click_y);
CP_Vector angleToVector(float angle);

//angle from the x axis
float vectorToAngle(CP_Vector a);