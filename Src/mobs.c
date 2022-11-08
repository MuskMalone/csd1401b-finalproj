#include "mobs.h"
//what each mob had run
//when each mob is running on the screen, each codes runs
void mob_melee(int player_idx, Entity entities[], int mob_idx)
{
	
	Player* player = &(entities[player_idx].player);
	Mob* mob = &(entities[mob_idx].mob);

	//getting the diameter of the player and the mob for collision
	float player_dia = player->diameter;
	float mob_dia = mob->diameter;

	//


	//creating of the mob
	CP_Settings_StrokeWeight(0.0f);
	CP_Settings_Fill(CP_Color_Create(51, 255, 173, 255));
	CP_Vector direction = getVectorBetweenPositions(&(mob->pos), &(player->pos));
	moveEntity(&(mob->pos), direction.x * 50.0f, direction.y * 50.0f);
	CP_Settings_Fill(CP_Color_Create(255, 0, 0, 255));
	CP_Graphics_DrawCircle(mob->pos.x, mob->pos.y, mob_dia);

	/*for (int i = 0; i < max_mob_idx; i++)
	{
		++mob_idx;
		Mob* mob = &(entities[mob_idx].mob);
		CP_Vector direction = getVectorBetweenPositions(&(mob->pos), &(player->pos));
		moveEntity(&(mob->pos), direction.x * 50.0f, direction.y * 50.0f);
		CP_Settings_Fill(CP_Color_Create(255, 0, 0, 255));
		CP_Graphics_DrawCircle(mob->pos.x, mob->pos.y, mob_dia);
	}*/

	//collision of enemey and the player
	if (collisionCircle(mob->pos, mob_dia/2.0f,player->pos, player_dia/2.0f))
	{
		entities[mob_idx].type = entity_null; //'destroy' the enemy
	}

	
}

Mob init_mob() {
	
	float Window_Width = (float)CP_System_GetWindowWidth();
	float Window_Height = (float)CP_System_GetWindowHeight();
	
	//setting the mob position and the diameter
	Mob mob;
	Position p;
	p.x = (Window_Width / 3) - (50 / 3);
	p.y = (Window_Height / 3) - (50 / 3);
	mob.pos = p;
	mob.diameter = 50.0f;
	mob.health = 50.0f;
	return mob;
}

void update_mob(int mob_idx, int player_idx, Entity entities[]) {

	mob_melee(player_idx, entities, mob_idx);


}