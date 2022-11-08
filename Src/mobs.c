#include "mobs.h"
#include "player.h"
#include "projectiles.h"
#include "map.h"
//what each mob had run
//when each mob is running on the screen, each codes runs
BOOL mob_isNear = 0;

int collision_mob_wall(Position p, float diameter, int wall_pos[GRID_ROWS][GRID_COLS]) {
	for (int i = 0; i < GRID_ROWS; ++i) {
		for (int j = 0; j < GRID_COLS; ++j) {
			if (wall_pos[i][j]) {
				int collided = collisionCircleRect(p, diameter / 2.0f, (Position) { WALL_DIM* (float)j, WALL_DIM* (float)i }, WALL_DIM, WALL_DIM);
				if (collided) {
					return collided;
				}
			}
		}
	}
	return 0;
}

void expansion_mob_size(Entity entities[], int mob_idx)
{
	float currentElapsedTime = CP_System_GetDt();

	Mob* mob = &(entities[mob_idx].mob);
	float mob_dia = mob->diameter; //initialising in the local variable
	float circle_expansion_cap = 20.0f;

	//mob->diameter++; //plusing the global variable 
	

	if (mob_dia >= 100.0f)
	{
		entities[mob_idx].type = entity_null;
	}
}

void mob_explosion(int player_idx, Entity entities[], int mob_idx, int wall_pos[GRID_ROWS][GRID_COLS])
{
	float mob_speed = 100.0f;
	
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

	if (collisionCircle(mob->pos, mob_dia * 5.0f, player->pos, player_dia * 5.0f)) //give a certain distance
	{
		//moveEntity(&(mob->pos), direction.x * 50.0f, direction.y * 50.0f); //the enemy will move
		int mob_at_xborder, mob_at_xwall, mob_at_yborder, mob_at_ywall;
		float xspeed = (float)(direction.x * mob_speed),
			yspeed = (float)(direction.y * mob_speed);
		float futureX = mob->pos.x + xspeed * CP_System_GetDt(), 
			futureY = mob->pos.y + yspeed * CP_System_GetDt();

		mob_at_xborder = !((futureX < (CP_System_GetWindowWidth() - ((mob->diameter) / 2.0f))) && (futureX > (0.0f + ((mob->diameter) / 2.0f)))),
			mob_at_xwall = collision_mob_wall((Position) { .x = futureX, .y = mob->pos.y }, mob->diameter, wall_pos);

		mob_at_yborder = !((futureY < (CP_System_GetWindowHeight() - ((mob->diameter) / 2.0f))) && (futureY > (0.0f + ((mob->diameter) / 2.0f)))),
			mob_at_ywall = collision_mob_wall((Position) { .x = mob->pos.x, .y = futureY }, mob->diameter, wall_pos);

		if (!(mob_at_xborder || mob_at_xwall))
			moveEntity(&(mob->pos), xspeed, 0.0f); //the enemy will move
		if (!(mob_at_yborder || mob_at_ywall))
			moveEntity(&(mob->pos), 0.0f, yspeed); //the enemy will move

		if (collisionCircle(mob->pos, mob_dia * 2.0f, player->pos, player_dia * 2.0f))
		{
			mob->is_exploding = 1;
		}
	}
	if (mob->is_exploding)
	{
		expansion_mob_size(entities, mob_idx); //and runs the function that expands the size of the mob
	}

	//
	/*if(collisionCircle(mob->pos, projectile size, player->pos, projectile size))
	{
		mob->health -= 1;
	}*/
}

void mob_ranged(int index, int player_idx, Entity entities[], int mob_idx, int wall_pos[GRID_ROWS][GRID_COLS])
{
	int p_idx = insert_to_entity_array(entity_projectile, entities, init_projectile);
	set_projectile_values(&(entities[p_idx].projectile), 'p', 'm', 10, startposb, getVectorBetweenPositions(&(startposb), &(Mousepos)));


}

entity_struct init_mob() {
	
	float Window_Width = (float)CP_System_GetWindowWidth();
	float Window_Height = (float)CP_System_GetWindowHeight();
	
	//setting the mob position and the diameter
	Mob mob;
	//Position p;
	
	//p.x = (Window_Width / 3);
	//p.y = (Window_Height / 3);
	mob.pos.x = CP_Random_RangeFloat(0.0f, Window_Width);//100.0f;
	mob.pos.y = CP_Random_RangeFloat(0.0f, Window_Height);//100.0f;
	mob.diameter = 50.0f;
	mob.health = 50.0f;
	mob.radius_damage = 100.0f;
	mob.is_exploding = 0;
	return (entity_struct) {.mob = mob};
}

void update_mob(int mob_idx, int player_idx, Entity entities[], int wall_pos[GRID_ROWS][GRID_COLS])
{
	mob_explosion(player_idx, entities, mob_idx, wall_pos);
}

