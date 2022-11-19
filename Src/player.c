#include <stdio.h>
#include <stdlib.h>
#include "player.h"
#include "camera.h"
#include "easing.h"


CP_Image player_front[PLAYER_SPRITE_COUNT];
CP_Image player_frontdiagleft[PLAYER_SPRITE_COUNT];
CP_Image player_frontdiagright[PLAYER_SPRITE_COUNT];
CP_Image player_back[PLAYER_SPRITE_COUNT];
CP_Image player_backdiagleft[PLAYER_SPRITE_COUNT];
CP_Image player_backdiagright[PLAYER_SPRITE_COUNT];
CP_Image player_left[PLAYER_SPRITE_COUNT];
CP_Image player_right[PLAYER_SPRITE_COUNT];

static float sprite_timer = 0.0f;
static float stamina = 255.0f;
static float radius_reduction = 4.8f;
static float dashed_duration = .0f;
static int is_cooldown = 0;
static float cooldown = .0f;
static int melee_deflect_triggered = 0;

CP_Image player_deflect_arrow;
CP_Image Player_Barrier_Img;
CP_Image *player_sprite_ptr;


int release_held_projectiles(Player * player, Entity entities[]) {
	int out = 0;
	set_state(player, resting);
	melee_deflect_triggered = 0;
	ACCESS_ALL_ENTITIES{
		if (entities[i].type == entity_projectile) {
			Projectile* proj = &(entities[i].projectile);
			if (collisionCircle(player->pos, MAX_PARRYRADIUS, proj->pos, proj->radius) && proj->source != PLAYER_PROJ_SOURCE1 && proj->type != PROJ_TYPE_STATIC) {
				int dir_x, dir_y;

				set_projectile_values(
					&(entities[i].projectile), 
					PLAYER_PROJ_SOURCE1, PROJ_TYPE_MOBILE, 
					10, 
					proj->pos, 
					getVectorBetweenPositions(
						player->pos, 
						(Position) { 
							CP_Input_GetMouseX(), 
							CP_Input_GetMouseY() 
						}
					)
				);
				proj->speed = 1000;
				out = 1;
			}
		}
	}
	return out;
}

static void init_cooldown(void) {
	if (!is_cooldown) {
		stamina = 0.0f;
		cooldown = COOLDOWN_DURATION;
		is_cooldown = 1;
	}
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
static int player_deflect_projectile(Player *p, Entity entities[]) {
	int out = 0;
	ACCESS_ALL_ENTITIES {
		if (entities[i].type == entity_projectile) {
			Projectile* projectile = &(entities[i].projectile);
			int collided = collisionCircle(p->pos, p->parryrad, projectile->pos, projectile->radius);
			if (collided && projectile->source != (char) PLAYER_PROJ_SOURCE1 && projectile->type != PROJ_TYPE_MOBILE) {
				deflectprojectiles((char)PLAYER_PROJ_SOURCE1, i, entities);
				out = 1;
			}
		}
	}
	return out;
}
void set_state(Player* p, player_state state) {
	// only allow state from dashing to resting;
	if (p->state == dead) { p->state = state; return; }
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
	player_sprite_ptr = player_front;
	player.health = PLAYER_HEALTH_SPRITE_COUNT;

	player.speed = NORMAL_SPEED;
	player.horizontal_dir = 0, player.vertical_dir = 0;
	player.state = resting;
	player.diameter = PLAYER_DIAMETER;//50.0f;
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
	if (CP_Input_MouseDown(MOUSE_BUTTON_1)) {
		if (!is_cooldown)
			set_state(player, holding);
		if (!melee_deflect_triggered) {
			if (is_cooldown) {
				if (cooldown <= MAX_COOLDOWN)
					cooldown += 1.0f;
				shake_camera(PROJ_HOLDING_SHAKE, 0);
			}
			else {
				if (stamina >= STAMINA_COST) {
					if (player_deflect_projectile(player, entities)) shake_camera(PROJ_DEFLECT_SHAKE, 1);
					stamina -= STAMINA_COST; // if stamina 
				}
				else {
					init_cooldown();
				}
			}
			melee_deflect_triggered = 1;
		}
		else {
			if (is_cooldown) {
				if (cooldown <= MAX_COOLDOWN)
				cooldown += 3.0 * CP_System_GetDt();
			}
			else {
				if (stamina >= STAMINA_COST_HOLD) {
					stamina -= STAMINA_COST_HOLD;
					shake_camera(PROJ_HOLDING_SHAKE, 1);
				}
				else {
					release_held_projectiles(player, entities);
					init_cooldown();
				}
			}

		}
	}

	if (CP_Input_MouseReleased(MOUSE_BUTTON_1)) {
		if (!is_cooldown)
			if (release_held_projectiles(player, entities)) shake_camera(PROJ_DEFLECT_SHAKE, 1);
		set_state(player, resting);
	}
	if (CP_Input_MouseTriggered(MOUSE_BUTTON_2)) {
		// make sure player is moving
		if ((player->horizontal_dir || player->vertical_dir) && player->state != holding) {

			// if cooldown, penalize the player by adding cooldown
			if (is_cooldown) {
				if (cooldown <= MAX_COOLDOWN)
					cooldown += 1.0f;
				shake_camera(PROJ_HOLDING_SHAKE, 0);
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
		set_state(player, resting);
		player->horizontal_dir = 0;
	}
	if (CP_Input_KeyReleased(KEY_W) || CP_Input_KeyReleased(KEY_S)) {
		set_state(player, resting);
		player->vertical_dir = 0;
	}

	// if holding
	if (player->state == holding) {
		CP_Vector mouse_dir = getVectorBetweenPositions(player->pos, (Position) { CP_Input_GetMouseX(), CP_Input_GetMouseY() });
		float start_x = player->pos.x + (mouse_dir.x * (MAX_PARRYRADIUS / 2.0f)),
			start_y = player->pos.y + (mouse_dir.y * (MAX_PARRYRADIUS / 2.0f));
		ACCESS_ALL_ENTITIES {
			if (entities[i].type == entity_projectile) {
				Projectile* proj = &(entities[i].projectile);
				if (collisionCircle(player->pos, MAX_PARRYRADIUS, proj->pos, proj->radius) && proj->source != PLAYER_PROJ_SOURCE1 && proj->type != PROJ_TYPE_STATIC) {
					proj->speed = 0; proj->source = PLAYER_PROJ_SOURCE2;
					CP_Vector dir = getVectorBetweenPositions(proj->pos, (Position) { start_x, start_y });
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
}
void player_injured_particles(Player *player) {
	create_particle_burst(
		2.0f,
		EaseOutExpo,
		CP_Color_Create(0, 0, 0, 255),
		player->pos,
		player->diameter * 2.0f,
		10.0f,
		20.0f,
		0.0f,
		360.f,
		10
	);
}
int damage_player(Player *p) {
	if (p->state != dashing) {
		flash_hue(CP_Color_Create(255, 0, 0, 0), DAMAGE_TINT_TIMER, 10, 80);
		shake_camera(DAMAGE_SHAKE, 1);
		player_injured_particles(p);
		p->health -= 1;
		return 1;
	}
	return 0;
}

void set_player_position(Player* player, Position pos) {
	player->pos = pos;
}
void draw_player(Player* player) {

	if (player->horizontal_dir == 1 && player->vertical_dir == 0) {
		player_sprite_ptr = player_right;
	}
	else if (player->horizontal_dir == -1 && player->vertical_dir == 0) {
		player_sprite_ptr = player_left;
	}
	else if (player->horizontal_dir == 0 && player->vertical_dir == 1) {
		player_sprite_ptr = player_front;
	}
	else if (player->horizontal_dir == 0 && player->vertical_dir == -1) {
		player_sprite_ptr = player_back;
	}
	else if (player->horizontal_dir == 1 && player->vertical_dir == 1) {
		player_sprite_ptr = player_frontdiagright;
	}
	else if (player->horizontal_dir == -1 && player->vertical_dir == 1) {
		player_sprite_ptr = player_frontdiagleft;
	}
	else if (player->horizontal_dir == 1 && player->vertical_dir == -1) {
		player_sprite_ptr = player_backdiagright;
	}
	else if (player->horizontal_dir == -1 && player->vertical_dir == -1) {
		player_sprite_ptr = player_backdiagleft;
	}

	if (player->state == holding) {
		CP_Vector mouse_dir = getVectorBetweenPositions(player->pos, (Position) { CP_Input_GetMouseX(), CP_Input_GetMouseY() });
		float start_x = player->pos.x + (mouse_dir.x * (MAX_PARRYRADIUS * 1.2f)),
			start_y = player->pos.y + (mouse_dir.y * (MAX_PARRYRADIUS * 1.2f));		
		CP_Image_DrawAdvanced(player_deflect_arrow, start_x, start_y, WALL_DIM, WALL_DIM, 255, vectorToAngle(mouse_dir));
		CP_Image_Draw(player_sprite_ptr[PLAYER_SPRITE_COUNT - 1], get_camera_x_pos(player->pos.x), get_camera_y_pos(player->pos.y), player->diameter, player->diameter, 255);
	}
	else if (player->state == resting) {
		CP_Image_Draw(player_sprite_ptr[1], get_camera_x_pos(player->pos.x), get_camera_y_pos(player->pos.y), player->diameter, player->diameter, 255);
	}
	else {
		sprite_timer += 5.0f * CP_System_GetDt();
		CP_Image_Draw(player_sprite_ptr[(int)sprite_timer%4], get_camera_x_pos(player->pos.x), get_camera_y_pos(player->pos.y), player->diameter, player->diameter, 255);
	}

	//CP_Settings_StrokeWeight(0.0f);
	CP_Image_Draw(Player_Barrier_Img, get_camera_x_pos(player->pos.x), get_camera_y_pos(player->pos.y), player->parryrad*2, player->parryrad * 2, stamina);
	//CP_Settings_Fill(CP_Color_Create(51, 255, 173, 255));
	//CP_Graphics_DrawCircle(get_camera_x_pos(player->pos.x), get_camera_y_pos(player->pos.y), player->diameter);
}