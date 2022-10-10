#include "mobs.h"

Mob init_mob() {
	Mob mob;
	float Window_Width = CP_System_GetWindowWidth();
	float Window_Height = CP_System_GetWindowHeight();
	Position p;
	p.x = (Window_Width / 3) - (50 / 3);
	p.y = (Window_Height / 3) - (50 / 3);
	mob.pos = p;
	return mob;
}
void update_mob(int mob_idx, int player_idx, Entity entities[]) {
	Mob* mob = &(entities[mob_idx].mob);
	Player* player = &(entities[player_idx].player);
	//Prints the player Object
	CP_Settings_StrokeWeight(0.0f);
	CP_Settings_Fill(CP_Color_Create(51, 255, 173, 255));
	CP_Graphics_DrawCircle(mob->pos.x, mob->pos.y, 50);
	
	CP_Vector direction = getVectorBetweenPositions(&(mob->pos), &(player->pos));
	
	moveEntity(&(mob->pos), direction.x * 10, direction.y * 10);
	
	CP_Settings_Fill(CP_Color_Create(255, 0, 0, 255));
}