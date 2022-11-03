#include "mobs.h"
//what each mob had run
//when each mob is running on the screen, each codes runs
BOOL mob_isNear = 0;

void expansion_mob_size(Entity entities[], int mob_idx)
{
	float currentElapsedTime = CP_System_GetDt();

	Mob* mob = &(entities[mob_idx].mob);
	float mob_dia = mob->diameter; //initialising in the local variable
	float circle_expansion_cap = 20.0f;

	mob->diameter++; //plusing the global variable 
	

	if (mob_dia >= 100.0f)
	{
		
		entities[mob_idx].type = entity_null;
	}
}



void mob_explosion(int player_idx, Entity entities[], int mob_idx)
{
	
	
	Player* player = &(entities[player_idx].player);
	Mob* mob = &(entities[mob_idx].mob);

	//getting the diameter of the player and the mob for collision
	float player_dia = player->diameter;
	float mob_dia = mob->diameter;
	float explosion_radius = 100.0f;
	
	//creating of the mob
	CP_Settings_StrokeWeight(0.0f);
	CP_Settings_Fill(CP_Color_Create(51, 255, 173, 255));
	CP_Vector direction = getVectorBetweenPositions(&(mob->pos), &(player->pos));
	CP_Settings_Fill(CP_Color_Create(255, 0, 0, 255));
	CP_Graphics_DrawCircle(mob->pos.x, mob->pos.y, mob_dia);

	//mob_isNear = collisionCircle(mob->pos, mob_dia * 2.0f, player->pos, player_dia * 2.0f) ? 1 : 0;

	//mob_isNear = 0;
	//collision of enemy and the player
	if (collisionCircle(mob->pos, mob_dia * 2.0f ,player->pos, player_dia * 2.0f)) //give a certain distance
	{
		mob->is_exploding = 1;
		moveEntity(&(mob->pos), direction.x * 50.0f, direction.y * 50.0f); //the enemy will move
		
	}
	if (mob->is_exploding)
	{
		expansion_mob_size(entities, mob_idx); //and runs the function that expands the size of the mob
	}

	//if (collisionCircle(mob->pos, explosion_radius, player->pos, explosion_radius))
	//{
	//	//player health loss
	//}

}

Mob init_mob() {
	
	float Window_Width = (float)CP_System_GetWindowWidth();
	float Window_Height = (float)CP_System_GetWindowHeight();
	
	//setting the mob position and the diameter
	Mob mob;
	//Position p;
	
	//p.x = (Window_Width / 3);
	//p.y = (Window_Height / 3);
	mob.pos.x = CP_Random_RangeFloat(0.0f, Window_Width);
	mob.pos.y = CP_Random_RangeFloat(0.0f, Window_Height);
	mob.diameter = 50.0f;
	mob.health = 50.0f;
	mob.radius_damage = 100.0f;
	mob.is_exploding = 0;
	return mob;
}

void update_mob(int mob_idx, int player_idx, Entity entities[]) 
{
	mob_explosion(player_idx, entities, mob_idx);
}
