#include "projectiles.h"

Projectile init_projectile(char Source, Position Start, CP_Vector Direction) {
	Projectile Proj;
	Proj.pos.x = Start.x;
	Proj.pos.y = Start.y;
	Proj.Direction = Direction;
	Proj.radius = 10.0f;
	Proj.speed = 150;
	Proj.source = Source;
	return Proj;
}
void update_projectile(int index, Entity entities[]) {
	Projectile* proj = &(entities[index].projectile);
	int red_rgb = proj->source == 'p' ? 0 : 255;
	int blue_rgb = proj->source == 'p' ? 255 : 0;
	CP_Settings_Fill(CP_Color_Create(red_rgb, 0, blue_rgb, 255));
	CP_Graphics_DrawCircle(proj->pos.x, proj->pos.y, proj->radius*2);
	moveEntity(&(proj->pos), proj->Direction.x * proj->speed, proj->Direction.y * proj->speed);
}

void deflectprojectiles(char source,int index, Entity entities[]) {
	Projectile* proj = &(entities[index].projectile);
	proj->source = source;
	proj->Direction = CP_Vector_Negate(proj->Direction);
	proj->speed *= 2;
}