/***
 *                       _
 *                      | |
 *     _ __ ___    ___  | |__       ___
 *    | '_ ` _ \  / _ \ | '_ \     / __|
 *    | | | | | || (_) || |_) | _ | (__
 *    |_| |_| |_| \___/ |_.__/ (_) \___|
 *
 *
 */
 /*
 @file mob.c
 @author Vanessa Chua
 @course CSD1401
 @section B
 @brief  code for mob entity
 */
 /*______________________________________________________________________*/
#include "mobs.h"
#include "player.h" //player pos, etc
#include "projectiles.h" //projectiles for the mobs
#include "easing.h"
#include "camera.h" //sprites
#include "sounds.h" //sound

//sprites for the mobs, check camera.c
CP_Image melee_mob_left[MELEE_MOB_SPRITE_COUNT];
CP_Image explode_mob_left[MELEE_MOB_SPRITE_COUNT];
CP_Image melee_mob_right[MELEE_MOB_SPRITE_COUNT];
CP_Image explode_mob_right[MELEE_MOB_SPRITE_COUNT];
CP_Image range_mob[RANGE_MOB_SPRITE_COUNT];

//collision for the mobs, static for this class only
static int collision_mob_wall(Position p, float diameter, int wall_pos[GRID_ROWS][GRID_COLS]) 
{
	//checking for every grid and rows of wall position, 2D array
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

//projectile for the exploding mob to attack the player
void explode_mob(int mob_idx, Entity entities[] ) 
{
	Mob* mob = &(entities[mob_idx].mob);
	int p_idx = insert_to_entity_array(entity_projectile, entities, init_projectile);
	if (p_idx > 0)
	{
		//setting the projectile values for the exploding mob to the player
		Projectile* p = &(entities[p_idx].projectile);
		set_projectile_values(p, MOB_PROJ_SOURCE, PROJ_TYPE_STATIC, (int)(mob->diameter / 2.0f), mob->pos, getVectorBetweenPositions(mob->pos, entities[PLAYER_IDX].player.pos));
	}
	//creating particle effects for the projectile
	create_particle_burst(
		2.0f,
		EaseOutExpo,
		CP_Color_Create(255, 0, 0, 255),
		mob->pos,
		mob->diameter,
		10.0f,
		20.0f,
		0.0f,
		360.f,
		10
	);
	entities[mob_idx].type = entity_null;
}

//for increasing size of the mob when player is nearby
void expansion_mob_size(Entity entities[], int mob_idx)
{
	Mob* mob = &(entities[mob_idx].mob);
	Player* player = &(entities[PLAYER_IDX].player);
	float mob_dia = mob->diameter; //initialising in the local variable
	mob->timer -= CP_System_GetDt();

	mob->diameter += 50.0f * CP_System_GetDt(); //increasing diameter of the mob
	
	//if times up, play sound effects and explode
	if (mob->timer <= 0.0f)
	{
		play_sound(EXPLOSION2);
		explode_mob(mob_idx, entities);
	}
	else if (collisionCircle(player->pos, player->diameter/2.0f, mob->pos, mob->diameter/2.0f)) {
		play_sound(EXPLOSION2);
		explode_mob(mob_idx, entities);
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

	CP_Vector direction = getVectorBetweenPositions(mob->pos, player->pos);

	//movement of enemies when reaches a certain radius
	if (collisionCircle(mob->pos, mob_dia * 5.0f, player->pos, player_dia * 5.0f)) //give a certain distance
	{
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
			mob->is_exploding = 1; //set true
		}
	}

	if (mob->is_exploding) //if is set to is_exploding true
	{
		expansion_mob_size(entities, mob_idx); //and runs the function that expands the size of the mob
	}

}


//mob ranged creation
void mob_ranged(int player_idx, Entity entities[], int mob_idx)
{
	Mob* mob = &(entities[mob_idx].mob);
	Player* player = &(entities[player_idx].player);
	Position position_player = player->pos;
	Position mob_pos = mob->pos;
	float mob_dia = mob->diameter;
	int proj_radius = 10;

	//setting projectile for range mob
	mob->timer -= CP_System_GetDt();
	if (mob->timer < 0.0f) {
		int p_idx = insert_to_entity_array(entity_projectile, entities, init_projectile);
		if (p_idx > 0) {
			set_projectile_values(&(entities[p_idx].projectile), MOB_PROJ_SOURCE, PROJ_TYPE_MOBILE, proj_radius, mob_pos, getVectorBetweenPositions(mob_pos, position_player));
			// timer between 1 and 5 seconds
			mob->timer = MOB_RANGED_TIMER;
		}
	}
}

//mob melee creation
void mob_melee(int player_idx, Entity entities[], int mob_idx, int wall_pos[GRID_ROWS][GRID_COLS])
{
	float mob_speed = 100.0f;
	Mob* mob = &(entities[mob_idx].mob);
	Player* player = &(entities[player_idx].player);
	CP_Vector direction = getVectorBetweenPositions(mob->pos, player->pos);
	Position position_player = player->pos;
	Position mob_pos = mob->pos;
	float mob_dia = mob->diameter;
	float proj_radius = 25.0f;
	float player_dia = player->diameter;

	//check state
	if (mob->melee_state == mob_resting)  //if its resting
	{
		//check for distance, if its in certain radius
		if (collisionCircle(mob->pos, mob_dia * 5.0f, player->pos, player_dia * 5.0f)) //give a certain distance
		{
			mob->melee_state = mob_moving; //it change the states to moving
		}
	}
	else if (mob->melee_state == mob_moving) //if its moving
	{
		if (collisionCircle(mob->pos, mob_dia / 2.0f, player->pos, player_dia)) //if it collides half of the mob and the player
		{
			mob->melee_state = mob_attacking; //it change state to attacking
			return;
		}

		//movement of the enemies
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

		//will move to somewhere near the player, not 100% on the player itself
	}
	else if (mob->melee_state == mob_attacking) //if its attacking
	{
		mob->timer -= CP_System_GetDt(); //set timer

		if (mob->timer <= 0.0f) //if times up
		{
			mob->timer = MOB_MELEE_TIMER; //reset
			int p_idx = insert_to_entity_array(entity_projectile, entities, init_projectile); //get entity projectile
			if (p_idx > 0)
			{
				CP_Vector v = getVectorBetweenPositions(mob_pos, position_player); //get pos bwn mob and player
				set_projectile_values(
						&(entities[p_idx].projectile),
						MOB_PROJ_SOURCE, PROJ_TYPE_WEAPON,
						(int)proj_radius,
						(Position) {
							mob->pos.x + (mob->diameter * v.x), 
							mob->pos.y + (mob->diameter * v.y)
						},
						v
					); //set projectile for melee
				mob->melee_state = mob_attacked; //change state to attack
			}
		}
	}
	else if (mob->melee_state == mob_attacked) { //if its attack state
		mob->timer -= 2.0f * CP_System_GetDt(); //set timer
		if (mob->timer <= 0.0f) //if timer is up
		{
			mob->timer = MOB_MELEE_TIMER; //reset
			if (collisionCircle(mob->pos, mob_dia * 5.0f, player->pos, player_dia * 5.0f)) //give a certain distance
			{
				mob->melee_state = mob_moving; //set state to moving
			}
			else {
				mob->melee_state = mob_resting; //else set to resting
			}
		}

	}
}

//start function
entity_struct init_mob() 
{
	//get the size of the windows	
	float Window_Width = (float)CP_System_GetWindowWidth();
	float Window_Height = (float)CP_System_GetWindowHeight();
	
	//setting the mob position and the diameter
	int type = rand() % 3;
	Mob mob;
	mob.type = (attack_type)type;
	mob.diameter = WALL_DIM;
	mob.health = MOB_HEALTH;

	//switch case of mob type, range, melee, explode
	switch (mob.type) {
	case(range): //range
		mob.timer = MOB_RANGED_TIMER; //timer for range
		mob.diameter = WALL_DIM * 1.5f; //diameter of mob
		break;
	case(melee):
		mob.timer = MOB_MELEE_TIMER; //timer for melee
		mob.melee_state = mob_resting; //first state when start
		break;
	case(explode):
		mob.timer = MOB_EXP_TIMER; //timer for explode
		mob.is_exploding = 0; //set false when start
		break;
	}
	return (entity_struct) {.mob = mob};
}

//update function
void update_mob(int mob_idx, int player_idx, Entity entities[], int wall_pos[GRID_ROWS][GRID_COLS])
{
	Mob* mob = &(entities[mob_idx].mob);
	mob->direction = getVectorBetweenPositions(mob->pos, entities[player_idx].player.pos);
	if (mob->health <= 0) //if die
	{
		//particle effect
		create_particle_burst(
			2.0f,
			EaseOutExpo,
			CP_Color_Create(255, 255, 255, 255),
			mob->pos,
			mob->diameter * 2.0f,
			10.0f,
			20.0f,
			0.0f,
			360.f,
			10
		);
		play_sound(SPLAT); //sound effect
		entities[mob_idx].type = entity_null; //"destroy mob"
		return;
	}

	//switch case for mob type
	switch (mob->type) {
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

//mob damage, decrements health
void damage_mob(Mob* mob) 
{
	(mob->health)--;
}

//drawing of mob
void draw_mob(Mob* mob) 
{
	static float animationMelee = 0;
	float angle = vectorToAngle(mob->direction);

	animationMelee += CP_System_GetDt();
	//mob type creation
	switch (mob->type) {
	case(range):
		CP_Image_Draw(range_mob[(int)(angle / 45.0f)], get_camera_x_pos(mob->pos.x), get_camera_y_pos(mob->pos.y), mob->diameter, mob->diameter, 255);
		break;
	case(melee):
		if (angle >= 90.0f && angle <= 270.0f)
			CP_Image_Draw(melee_mob_left[(int)animationMelee % 2], get_camera_x_pos(mob->pos.x), get_camera_y_pos(mob->pos.y), mob->diameter, mob->diameter, 255);
		else
			CP_Image_Draw(melee_mob_right[(int)animationMelee % 2], get_camera_x_pos(mob->pos.x), get_camera_y_pos(mob->pos.y), mob->diameter, mob->diameter, 255);
		break;
	case(explode):
		if (angle >= 90.0f && angle <= 270.0f)
			CP_Image_Draw(explode_mob_left[(int)animationMelee % 2], get_camera_x_pos(mob->pos.x), get_camera_y_pos(mob->pos.y), mob->diameter, mob->diameter, 255);
		else 
			CP_Image_Draw(explode_mob_right[(int)animationMelee % 2], get_camera_x_pos(mob->pos.x), get_camera_y_pos(mob->pos.y), mob->diameter, mob->diameter, 255);
		break;
	}
}

