#include "utils.h"

void moveEntity(Position* p, float xspeed, float yspeed) {
	p->x += xspeed * CP_System_GetDt();
	p->y += yspeed * CP_System_GetDt();
}