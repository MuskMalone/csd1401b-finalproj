
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

int tilemap[GRID_ROWS][GRID_COLS];
CP_Image Flat_Floor = NULL;
CP_Image Rock_Floor = NULL;
CP_Image grave = NULL;
CP_Image Anvil = NULL;
CP_Image Barrel = NULL;
CP_Image ImageList[10];

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
			//
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

	CP_Settings_ImageWrapMode(CP_IMAGE_WRAP_CLAMP_EDGE);
	CP_Graphics_ClearBackground(CP_Color_Create(255, 255, 255, 255));
	//CP_Settings_ImageWrapMode(CP_IMAGE_WRAP_CLAMP_EDGE);
	for (int i = 0; i < GRID_ROWS; i++) {
		for (int j = 0; j < GRID_COLS; ++j) {
			if (tilemap[i][j] > 1)
				CP_Image_Draw(Flat_Floor, (j * WALL_DIM) + WALL_DIM / 2, (i * WALL_DIM) + WALL_DIM / 2, WALL_DIM, WALL_DIM, 255);
			else {
				CP_Graphics_DrawRect((j * WALL_DIM) + WALL_DIM / 2, (i * WALL_DIM) + WALL_DIM / 2, WALL_DIM, WALL_DIM);
				CP_Image_Draw(ImageList[tilemap[i][j]], (j * WALL_DIM) + WALL_DIM / 2, (i * WALL_DIM) + WALL_DIM / 2, WALL_DIM, WALL_DIM, 255);
			}
		}
	}
	for (int i = 0; i < GRID_ROWS; i++) {
		for (int j = 0; j < GRID_COLS; ++j) {
			if (tilemap[i][j] > 1) {
				//CP_Image_Draw(ImageList[tilemap[i][j]], (j * WALL_DIM) + WALL_DIM / 2, (i * WALL_DIM) + WALL_DIM / 2, WALL_DIM * 1.2, WALL_DIM * 1.2, 255);
				CP_Graphics_DrawRect((j * WALL_DIM) + WALL_DIM / 2, (i * WALL_DIM) + WALL_DIM / 2, WALL_DIM, WALL_DIM);
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
	
		Flat_Floor = CP_Image_Load("./Assets/Tiles/tile_0000.png");
	Rock_Floor = CP_Image_Load("./Assets/Tiles/tile_0012.png");
	grave = CP_Image_Load("./Assets/Tiles/tile_0065.png");
	Anvil = CP_Image_Load("./Assets/Tiles/tile_0074.png");
	Barrel = CP_Image_Load("./Assets/Tiles/tile_0082.png");
	ImageList[0] = Flat_Floor;
	ImageList[1] = Rock_Floor;
	ImageList[2] = grave;
	ImageList[3] = Anvil;
	ImageList[4] = Barrel;

	
}

void game_update(void)
{

	CP_Graphics_ClearBackground(CP_Color_Create(150, 150, 150, 255));
	
	// draws the tiles
	//Floor = CP_Image_Load("./Assets/Tiles/tile_0000.png");

	if (CP_Input_KeyTriggered(KEY_Q)) {
		Position Mousepos = (Position){ CP_Input_GetMouseX(),CP_Input_GetMouseY() };
		Position startposb;
		startposb.x = ((float)CP_System_GetWindowWidth() / 2);
		startposb.y = ((float)CP_System_GetWindowHeight() * 1 / 4);
		int p_idx = insert_to_entity_array(entity_projectile, entities, init_projectile);
		if (p_idx > -1) {
			set_projectile_values(&(entities[p_idx].projectile), 'a', 'm', 10, startposb, getVectorBetweenPositions(&(startposb), &(Mousepos)));
		}
	}
	if (CP_Input_KeyTriggered(KEY_F)) {
		clear_all_entities();
	}

	if (state == room_pause) {

	}
	else {
		if (state == loading) {
			map_idx = 1;//rand() % 5; //set map idx to a random range between 0 to 4
			generate_current_map();
			CP_Settings_ImageWrapMode(CP_IMAGE_WRAP_CLAMP_EDGE);
			//draws the tiles
			CP_Graphics_ClearBackground(CP_Color_Create(255, 255, 255, 255));
			for (int i = 0; i < GRID_ROWS; i++) {
				for (int j = 0; j < GRID_COLS; ++j) {
					if (room_wall_pos[i][j]) {
						unsigned int random_int = CP_Random_RangeInt(0, 90);
						tilemap[i][j] = (random_int < 30) ? 2 : (random_int < 60) ? 3 : 4;
					}
					else {
						unsigned int random_int = CP_Random_RangeInt(0, 100);
						tilemap[i][j] = (random_int > 5) ? 0 : 1;
					}
				}

			}
			//Wall = CP_Image_Load("./Assets/Justins_face.png");
			state = room_active;
		}
		else {
			draw_room_wall();
			for (int i = 0; i < ENTITY_CAP; ++i) {
				if (entities[i].type == entity_null) continue;
				switch (entities[i].type) {
				case entity_player: update_player(PLAYER_IDX, entities, room_wall_pos); break;
				case entity_mob: update_mob(i, PLAYER_IDX, entities, room_wall_pos); break;
				case entity_boss: update_boss(BOSS_IDX, PLAYER_IDX, entities, room_wall_pos); break;
				case entity_projectile: update_projectile(i, entities, room_wall_pos); break;
				}
			}
			
			if (state == room_active) {
				for (int i = 0; i < ENTITY_CAP; ++i) {
					// if the entities are not player or null
					if (entities[i].type != entity_player && entities[i].type != entity_null && entities[i].type != entity_projectile)
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
	/*
	if (CP_Input_KeyTriggered(KEY_1))
		CP_Settings_ImageWrapMode(CP_IMAGE_WRAP_CLAMP);
	else if (CP_Input_KeyTriggered(KEY_2))
		
	else if (CP_Input_KeyTriggered(KEY_3))
		CP_Settings_ImageWrapMode(CP_IMAGE_WRAP_REPEAT);
	else if (CP_Input_KeyTriggered(KEY_4))
		CP_Settings_ImageWrapMode(CP_IMAGE_WRAP_MIRROR);
		*/
}

void game_exit(void)
{

}

