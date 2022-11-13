#include <stdio.h>
#include <stdlib.h>
#include "player.h"
#define COOLDOWN_DURATION 2.0f
#define MAX_COOLDOWN 8.0f
#define MAX_PARRYRADIUS WALL_DIM * 1.4f
#define DASH_DURATION .15f
#define STAMINA_COST 70.0f
#define STAMINA_COST_HOLD 90.0f * CP_System_GetDt()
#define NORMAL_SPEED 200
#define DASH_SPEED NORMAL_SPEED*5
#define HOLDING_PROJ_SPEED 50.0f
#define ACCESS_ALL_ENTITIES for (int i = 0; i < ENTITY_CAP; ++i) 

static float stamina = 255.0f;
static float radius_reduction = 4.8f;
static float dashed_duration = .0f;
static int is_cooldown = 0;
static float cooldown = .0f;
static int melee_deflect_triggered = 0;

void release_held_projectiles(Player * player, Entity entities[]) {
	set_state(player, resting);
	melee_deflect_triggered = 0;
	ACCESS_ALL_ENTITIES{
		if (entities[i].type == entity_projectile) {
			Projectile* proj = &(entities[i].projectile);
			if (collisionCircle(player->pos, MAX_PARRYRADIUS, proj->pos, proj->radius) && proj->source != PLAYER_PROJ_SOURCE1 && proj->type != PROJ_TYPE_STATIC) {
				int dir_x, dir_y;
				if (player->horizontal_dir == 0 && player->vertical_dir == 0) {
					//sets a random direction
					dir_x = rand() % 1, dir_y = rand() % 2;
					(dir_x) ? (dir_x = 1) : (dir_x = -1);
					(dir_y) ? (dir_y = 1) : (dir_y = -1);
				}
				else {
					dir_x = player->horizontal_dir;
					dir_y = player->vertical_dir;
				}
				set_projectile_values(&(entities[i].projectile), PLAYER_PROJ_SOURCE1, 'm', 10, proj->pos, CP_Vector_Normalize(CP_Vector_Set((float)dir_x, (float)dir_y)));
				proj->speed = 1000;
			}
		}
	}
}

static void init_cooldown(void) {
	stamina = 0.0f;
	cooldown = COOLDOWN_DURATION;
	is_cooldown = 1;
}
static int check_collision(Position p, float diameter, int wall_pos[GRID_ROWS][GRID_COLS]) {
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
static void player_deflect_projectile(Player *p, Entity entities[]) {
	ACCESS_ALL_ENTITIES {
		if (entities[i].type == entity_projectile) {
			Projectile* projectile = &(entities[i].projectile);
			int collided = collisionCircle(p->pos, p->parryrad, projectile->pos, projectile->radius);
			if (collided && projectile->source != (char) PLAYER_PROJ_SOURCE1 && projectile->type == PROJ_TYPE_STATIC) {
				deflectprojectiles((char)PLAYER_PROJ_SOURCE1, i, entities);
			}
		}
	}
}
void set_state(Player* p, player_state state) {
	// only allow state from dashing to resting;
	if (p->state == dashing) {
		if (state == resting) p->state = state;
		return;
	}
	else if (p->state == holding) {
		if (state == resting) p->state = state;
		return;
	}
	p->state = state;
}
entity_struct init_player(void) {
	Player player;
	float Window_Width = CP_System_GetWindowWidth();
	float Window_Height = CP_System_GetWindowHeight();
	Position p;

	player.health = 5;
	player.speed = NORMAL_SPEED;
	player.horizontal_dir = 0, player.vertical_dir = 0;
	player.state = resting;
	player.diameter = WALL_DIM;//50.0f;
	p.x = (Window_Width / 2) - (player.diameter / 2);
	p.y = (Window_Height / 2) - (player.diameter / 2);
	player.pos = p;
	player.parryrad = MAX_PARRYRADIUS;

	//init static global variables here
	stamina = 255.0f;
	radius_reduction = 4.8f;
	dashed_duration = .0f;
	is_cooldown = 0;
	cooldown = .0f;
	melee_deflect_triggered = 0;

	return (entity_struct) { .player = player };
}
void update_player(int player_idx, Entity entities[], int wall_pos[GRID_ROWS][GRID_COLS]) {
	Player* player = &(entities[player_idx].player);

	// if player is dead, stop doing anything;aa
	if (player->health <= 0) {
		set_state(player, dead);
		return;
	}
	//Reduces the barrier strength whenever the player clicks spacebar
	// If you want to reduce the opacity of the barrier, uncomment the "basewieght" variable in the "if" statement bellow
	// If you want to reduce the size of the barrier, uncomment the "radius_reduction" variable in the "if" statement bellow
	if (CP_Input_KeyDown(KEY_K)) {
		if (!is_cooldown)
			set_state(player, holding);
		if (!melee_deflect_triggered) {
			if (is_cooldown) {
				if (cooldown <= MAX_COOLDOWN)
					cooldown += .5f;
			}
			else {
				if (stamina >= STAMINA_COST) {
					player_deflect_projectile(player, entities);
					stamina -= STAMINA_COST; // if stamina 
				}
				else {
					init_cooldown();
				}
			}
			melee_deflect_triggered = 1;
		}
		else {
			if (is_cooldown && cooldown <= MAX_COOLDOWN) {
				cooldown += 3.0 * CP_System_GetDt();
			}
			else {
				if (stamina >= STAMINA_COST_HOLD) {
					stamina -= STAMINA_COST_HOLD;
				}
				else {
					release_held_projectiles(player, entities);
					init_cooldown();
				}
			}

		}
	}

	if (CP_Input_KeyReleased(KEY_K)) {
		release_held_projectiles(player, entities);
	}
	if (CP_Input_KeyTriggered(KEY_L)) {
		// make sure player is moving
		if ((player->horizontal_dir || player->vertical_dir) && player->state != holding) {

			// if cooldown, penalize the player by adding cooldown
			if (is_cooldown) {
				if (cooldown <= MAX_COOLDOWN)
					cooldown += .5f;
			}
			else {
				// reduce stamina
				if (stamina >= STAMINA_COST) {
					stamina -= STAMINA_COST;
				}
				else {
					init_cooldown();
				}
				if (player->state != dashing) {
					set_state(player, dashing);
				}
			}
		}

	}

	//Basic Player movement
	if (CP_Input_KeyDown(KEY_D)) {
		set_state(player, moving);
		player->horizontal_dir = 1;
	}
	else if (CP_Input_KeyDown(KEY_A)) {
		set_state(player, moving);
		player->horizontal_dir = -1;
	}
	if (CP_Input_KeyDown(KEY_W)) {
		set_state(player, moving);
		player->vertical_dir = -1;
	}
	else if (CP_Input_KeyDown(KEY_S)) {
		set_state(player, moving);
		player->vertical_dir = 1;
	}

	if (CP_Input_KeyReleased(KEY_D) || CP_Input_KeyReleased(KEY_A)) {
		player->horizontal_dir = 0;
	}
	if (CP_Input_KeyReleased(KEY_W) || CP_Input_KeyReleased(KEY_S)) {
		player->vertical_dir = 0;
	}

	// if holding
	if (player->state == holding) {
		float line_dist_x = WALL_DIM * (float)player->horizontal_dir, line_dist_y = WALL_DIM * (float)player->vertical_dir;
		float start_x = player->pos.x + ((float)player->horizontal_dir * (MAX_PARRYRADIUS / 2.0f)),
			start_y = player->pos.y + ((float)player->vertical_dir * (MAX_PARRYRADIUS / 2.0f));
		ACCESS_ALL_ENTITIES {
			if (entities[i].type == entity_projectile) {
				Projectile* proj = &(entities[i].projectile);
				if (collisionCircle(player->pos, MAX_PARRYRADIUS, proj->pos, proj->radius) && proj->source != PLAYER_PROJ_SOURCE1 && proj->type != PROJ_TYPE_STATIC) {
					proj->speed = 0; proj->source = PLAYER_PROJ_SOURCE2;
					Position pos = (Position){ start_x, start_y };
					CP_Vector dir = getVectorBetweenPositions(&(proj->pos), &pos);
					moveEntity(&(proj->pos), dir.x * HOLDING_PROJ_SPEED, dir.y * HOLDING_PROJ_SPEED);
				}

			}
		}
	}
	else {
		// if dashing
		if (player->state == dashing) {
			player->speed = DASH_SPEED;
			dashed_duration += CP_System_GetDt();
			if (dashed_duration > DASH_DURATION) {
				player->speed = NORMAL_SPEED;
				set_state(player, resting);
				dashed_duration = .0f;
			}
		}
		// moving or resting state
		else if (player->state == moving){
			player->speed = NORMAL_SPEED;
		}
		int player_at_xborder, player_at_xwall, player_at_yborder, player_at_ywall;
		float xspeed = (float)(player->horizontal_dir * player->speed),
			yspeed = (float)(player->vertical_dir * player->speed);
		float futureX = player->pos.x + xspeed * CP_System_GetDt(),
			futureY = player->pos.y + yspeed * CP_System_GetDt();

		player_at_xborder = !((futureX < (CP_System_GetWindowWidth() - ((player->diameter) / 2.0f))) && (futureX > (0.0f + ((player->diameter) / 2.0f)))),
			player_at_xwall = check_collision((Position) { .x = futureX, .y = player->pos.y }, player->diameter, wall_pos);

		player_at_yborder = !((futureY < (CP_System_GetWindowHeight() - ((player->diameter) / 2.0f))) && (futureY > (0.0f + ((player->diameter) / 2.0f)))),
			player_at_ywall = check_collision((Position) { .x = player->pos.x, .y = futureY }, player->diameter, wall_pos);

		if (!(player_at_xborder || player_at_xwall))
			moveEntity(&(player->pos), xspeed, 0.0f);
		if (!(player_at_yborder || player_at_ywall))
			moveEntity(&(player->pos), 0.0f, yspeed);
	}

	if (is_cooldown) {
		if (cooldown >= .0f)
			cooldown -= CP_System_GetDt();
		else {
			is_cooldown = 0;
			cooldown = .0f;
		}

	}
	else {
		if (stamina < 255.0f && player->state != holding) {
			stamina += 60.0f * CP_System_GetDt();
		}
	}
	draw_player(player);
}

int damage_player(Player *p) {
	if (p->state != dashing) {
		p->health -= 1;
		return 1;
	}
	return 0;
}

void set_player_position(Player* player, Position pos) {
	player->pos = pos;
}
void draw_player(Player* player) {
	CP_Settings_Fill(CP_Color_Create(255, 0, 0, 255));

	CP_Settings_Fill(CP_Color_Create(0, 0, 0, 255));
	CP_Settings_TextSize(20.0f);
	char buffer[500] = { 0 };
	sprintf_s(buffer, _countof(buffer), "player state: %d, cooldown: %f, health: %d, stamina: %f", player->state, cooldown, player->health, 3.5f * ((float)CP_System_GetDisplayHeight() / 100.0f));//stamina);
	CP_Font_DrawText(buffer, 30, 30);

	if (player->state == holding) {
		float line_dist_x = WALL_DIM * (float)player->horizontal_dir, line_dist_y = WALL_DIM * (float)player->vertical_dir;
		float start_x = player->pos.x + ((float)player->horizontal_dir * (MAX_PARRYRADIUS / 2.0f)),
			start_y = player->pos.y + ((float)player->vertical_dir * (MAX_PARRYRADIUS / 2.0f));
		CP_Settings_Stroke(CP_Color_Create(255, 160, 20, 255));
		CP_Settings_StrokeWeight(10.0f);
		CP_Graphics_DrawLine(start_x, start_y, start_x + line_dist_x, start_y + line_dist_y);
	}
	for (int i = 0, sw = 2, radius_size = (int)radius_reduction, parry_color = 255, parry_weight = (int)stamina; i < 8; ++i) {	//Creates the Barrier Effect
		if (i == 8 - 1) {	//Sets the white color ring
			radius_size = radius_reduction;
			parry_color = 255;
			parry_weight = stamina;
			sw = 3;
		}
		else { // Sets the translucent blue barrier effect 
			radius_size += 4;
			parry_color -= 30;
			parry_weight -= 255 / 8;
		}
		CP_Settings_StrokeWeight(sw);
		CP_Settings_Stroke(CP_Color_Create(parry_color, 255, 255, parry_weight));
		CP_Settings_Fill(CP_Color_Create(218, 240, 255, 0));
		CP_Graphics_DrawCircle(player->pos.x, player->pos.y, (player->parryrad * 2.0f) - (float)radius_size);
	}
	//Increases the barrier's opacity over time ( Uncomment this if you want to change the opacity of the barrier when user click space)
	//Prints the player Object
	CP_Settings_StrokeWeight(0.0f);
	CP_Settings_Fill(CP_Color_Create(51, 255, 173, 255));
	CP_Graphics_DrawCircle(player->pos.x, player->pos.y, player->diameter);
}