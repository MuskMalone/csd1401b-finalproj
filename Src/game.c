
#include "game.h"
#include <stdio.h>
#include <stdlib.h>

#define BOSS_IDX 1

#define SIZE 5
// to disable vs warning for fopen function
#pragma warning(disable : 4996)

// @todo auto resize the array
Entity entities[ENTITY_CAP];
int wall_pos[SIZE][GRID_ROWS][GRID_COLS];
int door_pos[GRID_ROWS][GRID_COLS];
int room_wall_pos[GRID_ROWS][GRID_COLS];
static int count_new_room = 0;
int map_idx = 0;
int mobs = 0;
int check = 0;

typedef enum room_state {room_active, room_clear, loading} room_state;

room_state state = loading;


void generate_room(void) {

	int ch, idx;

	// loads the map delimited by \n into the wall_pos array
	FILE* map = fopen("Assets/maps.txt", "r");
	for (idx = 0; idx < SIZE; ++idx) {
		for (int i = 0; i < GRID_ROWS; i++) {
			for (int j = 0; j < GRID_COLS; ++j) {
				ch = fgetc(map);
				// if encounter a \n char, break out of the entire loop
				int unit = atoi((char*)&ch);
				switch (unit) {
				case 0:
					wall_pos[idx][i][j] = 0;
					break;
				case 1:
					wall_pos[idx][i][j] = 1;
					break;
				case 2:
					insert_to_entity_array(entity_mob, entities, init_mob);
					wall_pos[idx][i][j] = 0;
					break;
				}
			}
		}

		ch = fgetc(map);
	}

	fclose(map);
}

void generate_door(void) {

	int ch;

	FILE* door = fopen("Assets/NextRoom.txt", "r");
	for (int i = 0; i < GRID_ROWS; i++) {
		for (int j = 0; j < GRID_COLS; ++j) {
			ch = fgetc(door);
			// if encounter a \n char, break out of the entire loop
			if (ch == '\n') {

				break;
			}

			door_pos[i][j] = atoi((char*)&ch);

		}
	}

	fclose(door);

}

void generate_wall(void) {

	int ch;

	FILE* wall = fopen("Assets/Room_Wall.txt", "r");
	for (int i = 0; i < GRID_ROWS; i++) {
		for (int j = 0; j < GRID_COLS; ++j) {
			ch = fgetc(wall);
			// if encounter a \n char, break out of the entire loop
			if (ch == '\n') {

				break;
			}

			room_wall_pos[i][j] = atoi((char*)&ch);

		}
	}

	fclose(wall);

}

void draw_room(int idx) {

	for (int i = 0; i < GRID_ROWS; i++) {
		for (int j = 0; j < GRID_COLS; ++j) {
			if (wall_pos[idx][i][j]) {
				CP_Settings_StrokeWeight(0.0);
				CP_Graphics_DrawRect(j * WALL_DIM, i * WALL_DIM, WALL_DIM, WALL_DIM);
			}
		}
	}

}

void draw_door(void) {

	for (int i = 0; i < GRID_ROWS; i++) {
		for (int j = 0; j < GRID_COLS; ++j) {
			if (door_pos[i][j]) {
				CP_Settings_StrokeWeight(0.0);
				CP_Graphics_DrawRect(j * WALL_DIM, i * WALL_DIM, WALL_DIM, WALL_DIM);
			}
		}
	}
}

void draw_room_wall(void) {

	for (int i = 0; i < GRID_ROWS; i++) {
		for (int j = 0; j < GRID_COLS; ++j) {
			if (room_wall_pos[i][j]) {
				CP_Settings_StrokeWeight(0.0);
				CP_Graphics_DrawRect(j * WALL_DIM, i * WALL_DIM, WALL_DIM, WALL_DIM);
			}
		}
	}

}

void game_init(void)
{
	for (int i = 0; i < ENTITY_CAP; ++i) {
		entities[i].type = entity_null;
	}
	insert_to_entity_array(entity_player, entities, init_player);
	//insert_to_entity_array(entity_boss, entities, init_boss);

	generate_wall();
	generate_room();
	generate_door();
	
}

void game_update(void)
{

	CP_Graphics_ClearBackground(CP_Color_Create(150, 150, 150, 255));

	// calls each entity's update function
	for (int i = 0; i < ENTITY_CAP; ++i) {
		if (entities[i].type == entity_null) continue;
		switch (entities[i].type) {
		case entity_player: update_player(PLAYER_IDX, entities, wall_pos[map_idx]); break;
		case entity_mob: update_mob(i, PLAYER_IDX, entities); break;
		case entity_boss: update_boss(BOSS_IDX, PLAYER_IDX, entities, wall_pos[map_idx]); break;
		case entity_projectile: update_projectile(i, entities, wall_pos[map_idx]); break;
		}
	}

	if (state == room_active) {

		draw_room_wall();

		//idk wat functions to use here help
		/*if (mob == 0 || boss == 0) {

			state = room_clear;
		}*/

	}
	else if (state == room_clear) {

		draw_door();
		for (int i = 0; i < GRID_ROWS; ++i) {
			for (int j = 0; j < GRID_COLS; ++j) {

				if (door_pos[i][j]) {

					if (collisionCircleRect(entities[PLAYER_IDX].player.pos, entities[PLAYER_IDX].player.diameter / 2.0f, (Position) { WALL_DIM* (float)j, WALL_DIM* (float)i }, WALL_DIM, WALL_DIM)) { //when touch door

						state = loading;

					}
				}
			}
		}

	}
	else if (state == loading) {

		//i oso dk wat functions to use here help
		//entities[MOB].type = entity_null;
		map_idx = rand() % 3 + 2; //set map idx to a random range between 2 to 5
		draw_room(map_idx);
		state = room_active;

	}

	if (CP_Input_MouseClicked(MOUSE_BUTTON_1)) {
		Position Mousepos = (Position){ CP_Input_GetMouseX(),CP_Input_GetMouseY() };
		Position startposb;
		startposb.x = ((float)CP_System_GetWindowWidth() / 2);
		startposb.y = ((float)CP_System_GetWindowHeight() * 1 / 4);
		int p_idx = insert_to_entity_array(entity_projectile, entities, init_projectile);
		if (p_idx > -1) {
			set_projectile_values(&(entities[p_idx].projectile), 'p', 'm', 10, startposb, getVectorBetweenPositions(&(startposb), &(Mousepos)));
		}
	}

}

void game_exit(void)
{

}

