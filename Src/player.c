#include "player.h"
#define MAX_PARRYRADIUS 50.0f;

float baseweight = 255.0f;
float radius_reduction = 4.8f;
int check_collision(Position const *p, float diameter, int wall_pos[GRID_ROWS][GRID_COLS]) {
	for (int i = 0; i < GRID_ROWS; ++i) {
		for (int j = 0; j < GRID_COLS; ++j) {
			if (wall_pos[i][j]) {
				int collided = collisionCircleRect(*p, diameter / 2.0f, (Position) { WALL_DIM* (float)j, WALL_DIM* (float)i }, WALL_DIM, WALL_DIM);
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
		moveEntity(&(player->pos), player->speed, 0);
		int player_at_border = !(player->pos.x < (CP_System_GetWindowWidth() - ((player->diameter) / 2.0f))),
			player_at_wall = check_collision(&(player->pos), player->diameter, wall_pos);
		if (!(player_at_border || player_at_wall)) {
			moveEntity(&(player->pos), player->speed, 0);
		}
		moveEntity(&(player->pos), -player->speed, 0);
	}
	else if (CP_Input_KeyDown(KEY_A)) {
		moveEntity(&(player->pos), -player->speed, 0);
		int player_at_border = !(player->pos.x > (0.0f + ((player->diameter) / 2.0f))),
			player_at_wall = check_collision(&(player->pos), player->diameter, wall_pos);
		if (!(player_at_border || player_at_wall)) {
			moveEntity(&(player->pos), -player->speed, 0);
		}
		moveEntity(&(player->pos), player->speed, 0);
	}
	if (CP_Input_KeyDown(KEY_W)) {
		moveEntity(&(player->pos), 0, -player->speed);
		int player_at_border = !(player->pos.y > (0.0f + ((player->diameter) / 2.0f))),
			player_at_wall = check_collision(&(player->pos), player->diameter, wall_pos);
		if (!(player_at_border || player_at_wall)) {
			moveEntity(&(player->pos), 0, -player->speed);
		}
		moveEntity(&(player->pos), 0, player->speed);
	}
	else if (CP_Input_KeyDown(KEY_S)) {
		moveEntity(&(player->pos), 0, player->speed);
		int player_at_border = !(player->pos.y < (CP_System_GetWindowHeight() - ((player->diameter) / 2.0f))),
			player_at_wall = check_collision(&(player->pos), player->diameter, wall_pos);
		if (!(player_at_border || player_at_wall)) {
			moveEntity(&(player->pos), 0, player->speed);
		}
		moveEntity(&(player->pos), 0, -player->speed);
	}
	CP_Settings_Fill(CP_Color_Create(255, 0, 0, 255));
}

void damage_player(Player *p) {

	p->health -= 1;
}