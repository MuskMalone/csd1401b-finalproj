
#include "game.h"
#include <stdio.h>
#include <stdlib.h>

#define BOSS_IDX 1

#define SIZE 6
// to disable vs warning for fopen function
#pragma warning(disable : 4996)

// @todo auto resize the array
Entity entities[ENTITY_CAP];
int wall_pos[SIZE][GRID_ROWS][GRID_COLS];
int door_pos[GRID_ROWS][GRID_COLS];
static int count_new_room = 0;
static int map_idx;
int check = 0;
int enemy = 0;

void generate_room(void) {

	int ch, idx;

	// loads the map delimited by \n into the wall_pos array
	FILE* map = fopen("Assets/maps.txt", "r");
	for (idx = 0; idx < SIZE; ++idx) {
		for (int i = 0; i < GRID_ROWS; i++) {
			for (int j = 0; j < GRID_COLS; ++j) {
				ch = fgetc(map);
				// if encounter a \n char, break out of the entire loop

				wall_pos[idx][i][j] = atoi((char*)&ch);

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

void draw_door(int check) {

	if (check == 1) {
		for (int i = 0; i < GRID_ROWS; i++) {
			for (int j = 0; j < GRID_COLS; ++j) {
				if (door_pos[i][j]) {
					CP_Settings_StrokeWeight(0.0);
					CP_Graphics_DrawRect(j * WALL_DIM, i * WALL_DIM, WALL_DIM, WALL_DIM);
				}
			}
		}
	}
}

void game_init(void)
{
	//int max_mobs = 20;
	//int mob_idx = 1;
	for (int i = 0; i < ENTITY_CAP; ++i) {
		entities[i].type = entity_null;
	}
	Player p = init_player();
	entities[PLAYER_IDX].type = entity_player;
	entities[PLAYER_IDX].player = p;

	Boss b = init_boss();
	entities[BOSS_IDX].type = entity_boss;
	entities[BOSS_IDX].boss = b;
	Mob m = init_mob();
	entities[2].type = entity_mob;
	entities[2].mob = m;
	//spawn of enemies
	/*for (int i = 2; i < ENTITY_CAP; ++i)
	{
		if (entities[i].type == entity_null)
		{
			Mob m = init_mob();
			entities[i].type = entity_mob;
			entities[i].mob = m;
		}
		
	}*/

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

	if (entities[PLAYER_IDX].player.health != 0) { //need check if player is alive

		//set map idx to the starting room when new_room is 0
		if (count_new_room == 0) {

			map_idx = 0;
		}
		//set map idx to the boss room when 4 room is cleared
		else if (count_new_room == 5) {

			map_idx = 1;

			//when boss is dead, reset count to 0
			if (entities[BOSS_IDX].boss.health == 0) {

				count_new_room = 0;
			}
		}

		if (enemy == 0) { //check if room is empty

			check = 1; //spawn doors when no enemy

			for (int i = 0; i < GRID_ROWS; ++i) {
				for (int j = 0; j < GRID_COLS; ++j) {

					if (door_pos[i][j]) {

						if (collisionCircleRect(entities[PLAYER_IDX].player.pos, entities[PLAYER_IDX].player.diameter / 2.0f, (Position) { WALL_DIM* (float)j, WALL_DIM* (float)i }, WALL_DIM, WALL_DIM)) { //when touch door

							map_idx = rand() % 3 + 2; //set map idx to a random range between 2 to 5
							count_new_room++; //count how many time next room is spawn

						}
					}

				}
			}


		}
	}

	draw_room(map_idx);
	draw_door(check);

	if (CP_Input_MouseClicked(MOUSE_BUTTON_1)) {
		for (int i = 0; i < ENTITY_CAP; ++i) {
			if (entities[i].type == entity_null) {
				//entities[BOSS_IDX].boss.health--;
				Position Mousepos = (Position){ CP_Input_GetMouseX(),CP_Input_GetMouseY() };
				Position startposb;
				startposb.x = ((float)CP_System_GetWindowWidth() * 3 / 4) - (10.0f);
				startposb.y = ((float)CP_System_GetWindowHeight() / 2) - (10.0f);
				Projectile projb = init_projectile('p', startposb, getVectorBetweenPositions(&(startposb), &(Mousepos)), 'r');
				entities[i].type = entity_projectile;
				entities[i].projectile = projb;
				break;
			}
		}
	}

}

void game_exit(void)
{

}

