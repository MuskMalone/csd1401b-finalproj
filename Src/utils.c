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