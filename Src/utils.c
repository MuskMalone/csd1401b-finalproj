#include "utils.h"

// updates entity coordinates according to xpeed and yspeed
// accounts for deltatime.
// replaces long code for moving objects
void moveEntity(Position* p, float xspeed, float yspeed) {
	p->x += xspeed * CP_System_GetDt();
	p->y += yspeed * CP_System_GetDt();
}

// get normalized direction vector between two positions
CP_Vector getVectorBetweenPositions(Position *from, Position *to) {
	float x = (to->x) - (from->x);
	float y = (to->y) - (from->y);
	return CP_Vector_Normalize(CP_Vector_Set(x, y));
}

// aabb collision for 4-sided right-angle polygons
int collisionAABB(Position a_min, Position a_max, Position b_min, Position b_max) {
	// for x axis
	if (a_min.x > b_max.x) return FALSE;
	else if (a_max.x < b_min.x) return FALSE;

	// for y axis
	if (a_min.y > b_max.y) return FALSE;
	else if (a_max.y < b_min.y) return FALSE;
	return TRUE;
}

// collision for circles
int collisionCircle(Position a_center, float a_radius, Position b_center, float b_radius) {
	float distance = CP_Vector_Distance(CP_Vector_Set(a_center.x, a_center.y), CP_Vector_Set(b_center.x, b_center.y)),
		total_radius = a_radius + b_radius;
	
	int out = (distance < total_radius) ? TRUE : FALSE;
	return out;
}
