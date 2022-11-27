#pragma once
#include <math.h>
#include "cprocessing.h"
#define TRUE 1
#define FALSE 0
# define M_PI           3.14159265358979323846  /* pi */

#ifndef UTILS
typedef struct Position {
	float x;
	float y;
} Position;
#endif

/*
This function updates entity coordinates according to xpeed and yspeed and accounts for
deltatime. It also accesses pointer location for struct position within movable structs
*/
void moveEntity(Position* p, float xspeed, float yspeed);

/*
This function get normalized direction vector between two positions
*/
CP_Vector getVectorBetweenPositions(Position from, Position to);

/*
This function calculates the aabb collision for 4 - sided right - angle polygons
*/
int collisionAABB(Position a_min, Position a_max, Position b_min, Position b_max);

/*
This function calculates the if there is any collision between circles
*/
int collisionCircle(Position a_center, float a_radius, Position b_center, float b_radius);

/*
This function calculates the if there is any collision between circle and rectangle.
*/
int collisionCircleRect(Position circle, float radius, Position rect, float width, float height);

/*
This function calculates the positions between 2 positions.
*/
float positionDistance(Position a, Position b);

/*
This function checks if the mouse is in an area.
*/
int IsAreaClicked(float area_center_x, float area_center_y, float area_width, float area_height, float click_x, float click_y);

/*
This function checks if the mouse is in the circle
*/
int IsCircleClicked(float circle_center_x, float circle_center_y, float diameter, float click_x, float click_y);

/*
This function converts a angle to a vector.
*/
CP_Vector angleToVector(float angle);

/*
This function converts a vector to an angle on the x-axis.
*/
float vectorToAngle(CP_Vector a);