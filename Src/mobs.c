#include "mobs.h"
#include "player.h"
#include "projectiles.h"
#include "map.h"
#include <stdlib.h>
#include "camera.h"
//what each mob had run
//when each mob is running on the screen, each codes runs
CP_Image melee_mob[MELEE_MOB_SPRITE_COUNT];
CP_Image explode_mob[MELEE_MOB_SPRITE_COUNT];
CP_Image range_mob[RANGE_MOB_SPRITE_COUNT];

//collision of mob and the wall
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

//movement of the mob
void movement_mob(Mob* mob, float direction_x, float direction_y, int wall_pos[GRID_ROWS][GRID_COLS])
{
	int mob_at_xborder, mob_at_xwall, mob_at_yborder, mob_at_ywall;
	float xspeed = (float)(direction_x),
		yspeed = (float)(direction_y);
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
}

//explosion mechanic for exploding mob
void expansion_mob_size(Entity entities[], int mob_idx)
{
	Mob* mob = &(entities[mob_idx].mob);
	float mob_dia = mob->diameter; //initialising in the local variable
	mob->timer -= CP_System_GetDt();

	mob->diameter += 50.0f * CP_System_GetDt();
	
	//if times up
	if (mob->timer <= 0.0f)//(mob_dia >= 100.0f)
	{
		int p_idx = insert_to_entity_array(entity_projectile, entities, init_projectile);
		if (p_idx > 0)
		{
			Projectile* p = &(entities[p_idx].projectile);
			Mob* m = &(entities[mob_idx].mob);
			set_projectile_values(p, MOB_PROJ_SOURCE, PROJ_TYPE_STATIC, mob->diameter / 2.0f, m->pos, getVectorBetweenPositions(&(m->pos), &(entities[PLAYER_IDX].player.pos)));
		}
		entities[mob_idx].type = entity_null;
	}
}

//mob explosion creation
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

	CP_Vector direction = getVectorBetweenPositions(&(mob->pos), &(player->pos));


	if (mob->is_exploding)
	{
		expansion_mob_size(entities, mob_idx); //and runs the function that expands the size of the mob
	}

	if (collisionCircle(mob->pos, mob_dia * 5.0f, player->pos, player_dia * 5.0f)) //give a certain distance
	{
		movement_mob(mob, direction.x * mob_speed, direction.y * mob_speed, wall_pos); //the enemy will move

		if (collisionCircle(mob->pos, mob_dia * 2.0f, player->pos, player_dia * 2.0f))
		{
			mob->is_exploding = 1;
		}
	}
}

//ranged mob
void mob_ranged(int player_idx, Entity entities[], int mob_idx)
{
	Mob* mob = &(entities[mob_idx].mob);
	Player* player = &(entities[player_idx].player);
	Position position_player = player->pos;
	Position mob_pos = mob->pos;
	float mob_dia = mob->diameter;
	int proj_radius = 10;
	
	mob->timer -= CP_System_GetDt();
	if (mob->timer < 0.0f) {
		int p_idx = insert_to_entity_array(entity_projectile, entities, init_projectile);
		if (p_idx > 0) {
			set_projectile_values(&(entities[p_idx].projectile), MOB_PROJ_SOURCE, 'm', proj_radius, mob_pos, getVectorBetweenPositions(&(mob_pos), &(position_player)));
			// timer between 1 and 5 seconds
			mob->timer = MOB_RANGED_TIMER;
		}
		//ranged mob mechanics
	}
}

//melee mob
void mob_melee(int player_idx, Entity entities[], int mob_idx, int wall_pos[GRID_ROWS][GRID_COLS])
{
	float mob_speed = 100.0f;
	Mob* mob = &(entities[mob_idx].mob);
	Player* player = &(entities[player_idx].player);
	CP_Vector direction = getVectorBetweenPositions(&(mob->pos), &(player->pos));
	Position position_player = player->pos;
	Position mob_pos = mob->pos;
	float mob_dia = mob->diameter;
	float proj_radius = 25.0f;
	float player_dia = player->diameter;

	//check states
	if (mob->melee_state == mob_resting) { //resting
		if (collisionCircle(mob->pos, mob_dia * 5.0f, player->pos, player_dia * 5.0f)) //give a certain distance
		{
			mob->melee_state = mob_moving;
		}
	}
	else if (mob->melee_state == mob_moving) { //moving
		if (collisionCircle(mob->pos, mob_dia / 2.0f, player->pos, player_dia))
		{
			mob->melee_state = mob_attacking;
			return;
		}
		movement_mob(mob, direction.x * mob_speed, direction.y * mob_speed, wall_pos);

		//will move to somewhere near the player, not 100% on the player itself
	}
	else if (mob->melee_state == mob_attacking) { //attacking
		mob->timer -= CP_System_GetDt();

		if (mob->timer <= 0.0f)
		{
			mob->timer = MOB_MELEE_TIMER;
			int p_idx = insert_to_entity_array(entity_projectile, entities, init_projectile);
			if (p_idx > 0) {
				CP_Vector v = getVectorBetweenPositions(&(mob_pos), &(position_player)); //get direction of the player from the mob
				set_projectile_values(
						&(entities[p_idx].projectile),
						MOB_PROJ_SOURCE, 's', //e - enemy, s - static
						proj_radius,
						(Position) {
							mob->pos.x + (mob->diameter * v.x), 
							mob->pos.y + (mob->diameter * v.y)
						},
						v
					);
				mob->melee_state = mob_attacked;
			}
		}
	}
	else if (mob->melee_state == mob_attacked) { //attacked
		mob->timer -= 2.0f * CP_System_GetDt();
		if (mob->timer <= 0.0f) //run timer and check if timer less then 0
		{
			mob->timer = MOB_MELEE_TIMER;
			//mob will move if in certain radius
			if (collisionCircle(mob->pos, mob_dia * 5.0f, player->pos, player_dia * 5.0f)) //give a certain distance
			{
				mob->melee_state = mob_moving;
			}
			else {
				mob->melee_state = mob_resting;
			}
		}

	}
}

entity_struct init_mob() {
	
	float Window_Width = (float)CP_System_GetWindowWidth();
	float Window_Height = (float)CP_System_GetWindowHeight();
	
	//setting the mob position and the diameter
	int type = rand() % 3;
	Mob mob;
	mob.type = (attack_type)type;
	mob.diameter = WALL_DIM;
	mob.health = MOB_HEALTH;

	switch (mob.type) { //attack type
	case(range):
		mob.timer = MOB_RANGED_TIMER;
		break;
	case(melee):
		mob.timer = MOB_MELEE_TIMER;
		mob.melee_state = mob_resting;
		break;
	case(explode):
		mob.timer = MOB_EXP_TIMER;
		mob.is_exploding = 0;
		break;
	}
	//mob.degree = 0.0f;

	return (entity_struct) {.mob = mob};
}

void update_mob(int mob_idx, int player_idx, Entity entities[], int wall_pos[GRID_ROWS][GRID_COLS])
{
	Mob* mob = &(entities[mob_idx].mob);
	if (mob->health <= 0) { 
		entities[mob_idx].type = entity_null;
		return;
	}

	switch (mob->type) {  //attack type
	case(range):
		mob_ranged(player_idx, entities, mob_idx);
		break;
	case(melee):
		mob_melee(player_idx, entities, mob_idx, wall_pos);
		break;
	case(explode):
		mob_explosion(player_idx, entities, mob_idx, wall_pos);
		break;
	}
}

void damage_mob(Mob* mob) {
	(mob->health)--;
}
void draw_mob(Mob* mob) {
	static float animationMelee = 0;
	animationMelee += CP_System_GetDt();
	switch (mob->type) {
	case(range):
		CP_Settings_StrokeWeight(0.0f);
		CP_Settings_Fill(CP_Color_Create(0, 255, 0, 255));
		CP_Graphics_DrawCircle(mob->pos.x, mob->pos.y, mob->diameter);
		break;
	case(melee):
		CP_Image_Draw(melee_mob[(int)animationMelee % 2], mob->pos.x, mob->pos.y, mob->diameter, mob->diameter, 255);
		break;
	case(explode):
		CP_Image_Draw(explode_mob[(int)animationMelee % 2], mob->pos.x, mob->pos.y, mob->diameter, mob->diameter, 255);
		break;
	}
}

