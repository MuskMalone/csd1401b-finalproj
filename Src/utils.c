#include "utils.h"

// updates entity coordinates according to xpeed and yspeed
// accounts for deltatime.
// replaces long code for moving objects
void moveEntity(Position* p, float xspeed, float yspeed) {
	p->x += xspeed * CP_System_GetDt();
	p->y += yspeed * CP_System_GetDt();
}

// get normalized direction vector between two positions
CP_Vector getVectorBetweenPositions(Position from, Position to) {
	float x = (to.x) - (from.x);
	float y = (to.y) - (from.y);
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
	float distance = positionDistance(a_center, b_center), total_radius = a_radius + b_radius;
	
	int out = (distance < total_radius) ? TRUE : FALSE;
	return out;
}

int collisionCircleRect(Position circle, float radius, Position rect, float width, float height) {
	// temporary variables to set edges for testing
	float testX = circle.x;
	float testY = circle.y;

	// which edge is closest
	if (circle.x < rect.x) testX = rect.x;      // test left edge
	else if (circle.x > rect.x + width) testX = rect.x + width;   // right edge
	if (circle.y < rect.y) testY = rect.y;      // top edge
	else if (circle.y > rect.y + height) testY = rect.y + height;   // bottom edge

	// get distance from closest edges
	float distX = circle.x - testX;
	float distY = circle.y - testY;
	float distance = sqrt((distX * distX) + (distY * distY));

	// if the distance is less than the radius, collision!
	if (distance <= radius) {
		return TRUE;
	}
	return FALSE;
}

float positionDistance(Position a, Position b) {
	float distX = a.x - b.x;
	float distY = a.y - b.y;
	return sqrt((distX * distX) + (distY * distY));
}

int IsAreaClicked(float area_center_x, float area_center_y, float area_width, float area_height, float click_x, float click_y)
{
	float boundaryBotY = area_center_y + area_height; //Find the highest point the mousePos can go 
	float boundaryTopY = area_center_y - area_height; //Find the lowest point the mousePos can go

	float boundaryLeftX = area_center_x - area_width; //Find the most left the mousePos can go 
	float boundaryRightX = area_center_x + area_width; //Find the most right the mousePos can go

	if (click_x >= boundaryLeftX && click_x <= boundaryRightX && click_y <= boundaryBotY && click_y >= boundaryTopY) //if ur mouse is within the box
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

int IsCircleClicked(float circle_center_x, float circle_center_y, float diameter, float click_x, float click_y)
{
	if (click_x <= circle_center_x + diameter && click_x >= circle_center_x - diameter && click_y <= circle_center_y + diameter && click_y >= circle_center_y - diameter)  //if ur mouse is within the circle
	{
		return 1;
	}
	else
	{
		return 0;
	}

}
CP_Vector angleToVector(float angle) {
	float radians = angle * (M_PI / 180.0f);
	return CP_Vector_Normalize(CP_Vector_Set(cos(radians), sin(radians)));

}
float vectorToAngle(CP_Vector a, Position from) {
	float angle = atanf(fabs(a.y) / fabs(a.x)) * (180.0f/M_PI);
	if (a.y > 0 && a.x < 0) return 180.0f - angle;
	else if (a.y < 0 && a.x < 0) return angle + 180.0f;
	else if (a.y < 0 && a.x > 0) return 360.0-angle;
	return angle;
}

/*
void DeflectProjectiles(char source, int projectile_index, Entity entities[]) {
	entities[projectile_index].projectile.source = source;
	CP_Vector_Negate(entities[projectile_index].projectile.Direction);
}*/