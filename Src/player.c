#include <stdio.h>
#include <stdlib.h>
#include "player.h"
#define MAX_PARRYRADIUS 50.0f;
#define DASH_DURATION .3f;

float baseweight = 255.0f;
float radius_reduction = 4.8f;
float dashed_duration = .0f;

int check_collision(Position p, float diameter, int wall_pos[GRID_ROWS][GRID_COLS]) {
	for (int i = 0; i < GRID_ROWS; ++i) {
		for (int j = 0; j < GRID_COLS; ++j) {
			if (wall_pos[i][j]) {
				int collided = collisionCircleRect(p, diameter / 2.0f, (Position) { WALL_DIM* (float)j, WALL_DIM* (float)i }, WALL_DIM, WALL_DIM);
				if (collided) return collided;
			}

		}
	}
	return 0;
}
Player init_player(void) {
	Player player;
	float Window_Width = CP_System_GetWindowWidth();
	float Window_Height = CP_System_GetWindowHeight();
	Position p;


	player.speed = 200;
	player.horizontal_dir = 0, player.vertical_dir = 0;
	player.state = resting;
	player.diameter = 50.0f;
	p.x = (Window_Width / 2) - (player.diameter / 2);
	p.y = (Window_Height / 2) - (player.diameter / 2);
	player.pos = p;
	player.parryrad = MAX_PARRYRADIUS;

	return player;
}
void update_player(int player_idx, Entity entities[], int wall_pos[GRID_ROWS][GRID_COLS]) {
	Player* player = &(entities[player_idx].player);
	for (int i = 0, sw = 2, radius_size = radius_reduction, parry_color = 255, parry_weight = baseweight; i < 8; ++i) {	//Creates the Barrier Effect
		if (i == 8 - 1) {	//Sets the white color ring
			radius_size = radius_reduction;
			parry_color = 255;
			parry_weight = baseweight;
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
		CP_Graphics_DrawCircle(player->pos.x, player->pos.y, player->parryrad * 2 - radius_size);
	}
	//Prints the player Object
	CP_Settings_StrokeWeight(0.0f);
	CP_Settings_Fill(CP_Color_Create(51, 255, 173, 255));
	CP_Graphics_DrawCircle(player->pos.x, player->pos.y, player->diameter);


	//Reduces the barrier strength whenever the player clicks spacebar
	// If you want to reduce the opacity of the barrier, uncomment the "basewieght" variable in the "if" statement bellow
	// If you want to reduce the size of the barrier, uncomment the "radius_reduction" variable in the "if" statement bellow
	if (CP_Input_KeyTriggered(KEY_SPACE)) {

		if (baseweight >= 100)  baseweight -= 100; // if baseweight 
		//radius_reduction += 2;
	}


	//Increases the barrier's opacity over time ( Uncomment this if you want to change the opacity of the barrier when user click space)
	if (baseweight < 255) {
		baseweight += 60 * CP_System_GetDt();
	}

	//Basic Player movement
	if (CP_Input_KeyDown(KEY_D)) {
		player->state = moving;
		player->horizontal_dir = 1;
	}
	else if (CP_Input_KeyDown(KEY_A)) {
		player->state = moving;
		player->horizontal_dir = -1;
	}
	if (CP_Input_KeyDown(KEY_W)) {
		player->state = moving;
		player->vertical_dir = -1;
	}
	else if (CP_Input_KeyDown(KEY_S)) {
		player->state = moving;
		player->vertical_dir = 1;
	}
	if (CP_Input_KeyReleased(KEY_D) || CP_Input_KeyReleased(KEY_A)) {
		player->horizontal_dir = 0;
	}
	if (CP_Input_KeyReleased(KEY_W) || CP_Input_KeyReleased(KEY_S)) {
		player->vertical_dir = 0;
	}
	if ((player->horizontal_dir == 0) && (player->vertical_dir == 0)) {
		player->state = resting;
	}
	else {
		// check for collision
		int player_at_xborder, player_at_xwall, player_at_yborder, player_at_ywall;
		float xspeed = (float)player->horizontal_dir * player->speed,
			yspeed = (float)player->vertical_dir * player->speed;
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

	CP_Settings_Fill(CP_Color_Create(255, 0, 0, 255));
}

void damage_player(Player *p) {

	p->health -= 1;
}