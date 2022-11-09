
#include "game.h"
#include <stdio.h>
#include <stdlib.h>

#define BOSS_IDX 1

#define SIZE 5
// to disable vs warning for fopen function
#pragma warning(disable : 4996)

// @todo auto resize the array
Entity entities[ENTITY_CAP];
int map_pos[SIZE][GRID_ROWS][GRID_COLS];
int door_pos[GRID_ROWS][GRID_COLS];
int room_wall_pos[GRID_ROWS][GRID_COLS];
static int count_new_room = 0;
static int map_idx = 0;

typedef enum room_state { room_pause, room_active, room_clear, loading } room_state;
enum tile_type { FLOOR_TILE, WALL_TILE, MOB_TILE, BOSS_TILE };

static room_state state = loading;


static void load_maps(void) {

	int ch, idx;

	// loads the map delimited by \n into the wall_pos array
	FILE* map = fopen("Assets/maps.txt", "r");
	for (idx = 0; idx < SIZE; ++idx) {
		for (int i = 0; i < GRID_ROWS; i++) {
			for (int j = 0; j < GRID_COLS; ++j) {
				ch = fgetc(map);
				// if encounter a \n char, break out of the entire loop

				// loads mob positions, wall positions, boss positions maybe into 
				// map_pos
				// 0 = nothing, 1 = wall, 2 = entity
				map_pos[idx][i][j] = atoi((char*)&ch);
			}
		}

		ch = fgetc(map);
	}

	fclose(map);
}

static void generate_door(void) {

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

//clears all entities except player
static void clear_all_entities(void) {
	for (int i = 0; i < ENTITY_CAP; ++i) {
		if (entities[i].type != entity_player)
			//marks idx ripe for overriding
			entities[i].type = entity_null;
	}
}

// generate the map that the player will use for the room
static void generate_current_map(void) {
	int idx = 0;
	for (int i = 0; i < GRID_ROWS; i++) {
		for (int j = 0; j < GRID_COLS; ++j) {
			int unit = map_pos[map_idx][i][j];
			switch (unit) {
			case FLOOR_TILE:
				room_wall_pos[i][j] = FLOOR_TILE;
				break;
			case WALL_TILE:
				room_wall_pos[i][j] = WALL_TILE;
				break;
			case MOB_TILE:
				room_wall_pos[i][j] = FLOOR_TILE;
				idx = insert_to_entity_array(entity_mob, entities, init_mob);
				if (idx > 0)
					entities[idx].mob.pos = (Position){ ((float)j * WALL_DIM) + (WALL_DIM / 2.0f), ((float)i * WALL_DIM) + (WALL_DIM / 2.0f) };
				break;
			case BOSS_TILE:
				room_wall_pos[i][j] = FLOOR_TILE;
				idx = insert_to_entity_array(entity_boss, entities, init_boss);
				if (idx > 0)
					entities[idx].boss.pos = (Position){ ((float)j * WALL_DIM) + (WALL_DIM / 2.0f), ((float)i * WALL_DIM) + (WALL_DIM / 2.0f) };
				break;
			}
		}
	}
}

static void draw_door(void) {
	for (int i = 0; i < GRID_ROWS; i++) {
		for (int j = 0; j < GRID_COLS; ++j) {
			if (door_pos[i][j]) {
				CP_Settings_StrokeWeight(0.0);
				CP_Graphics_DrawRect(j * WALL_DIM, i * WALL_DIM, WALL_DIM, WALL_DIM);
			}
		}
	}
}

static void draw_room_wall(void) {

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
	//initialized the player as idx 0
	for (int i = 0; i < ENTITY_CAP; ++i) {
		entities[i].type = entity_null;
	}
	insert_to_entity_array(entity_player, entities, init_player);

	load_maps();
	generate_door();
	
}

void game_update(void)
{

	CP_Graphics_ClearBackground(CP_Color_Create(150, 150, 150, 255));

	// calls each entity's update function


	if (state == room_pause) {

	}
	else {
		if (state == loading) {
			map_idx = rand() % 5; //set map idx to a random range between 0 to 4
			generate_current_map();
			state = room_active;
		}
		else {
			draw_room_wall();
			for (int i = 0; i < ENTITY_CAP; ++i) {
				if (entities[i].type == entity_null) continue;
				switch (entities[i].type) {
				case entity_player: update_player(PLAYER_IDX, entities, room_wall_pos); break;
				case entity_mob: update_mob(i, PLAYER_IDX, entities); break;
				case entity_boss: update_boss(BOSS_IDX, PLAYER_IDX, entities, room_wall_pos); break;
				case entity_projectile: update_projectile(i, entities, room_wall_pos); break;
				}
			}
			
			if (state == room_active) {
				for (int i = 0; i < ENTITY_CAP; ++i) {
					// if the entities are not player or null
					if (entities[i].type != entity_player && entities[i].type != entity_null)
						break;
					if (i == ENTITY_CAP - 1)
						state = room_clear;
				}
			}
			else if (state == room_clear) {
				clear_all_entities();
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
		}
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
	if (CP_Input_KeyTriggered(KEY_F)) {
		clear_all_entities();
	}
}

void game_exit(void)
{

}

