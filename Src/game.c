
#include "gamestates.h"
#include "camera.h"
#include "game.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define BOSS_IDX 1
#define BOSS_ROOM_INTERVAL 6
#define SIZE 11
#define DOOR_WIDTH WALL_DIM * 4.0f
// to disable vs warning for fopen function
#pragma warning(disable : 4996)

// @todo auto resize the array
Entity entities[ENTITY_CAP];
static int map_pos[SIZE][GRID_ROWS][GRID_COLS];
static int door_pos[GRID_ROWS][GRID_COLS];
static int room_wall_pos[GRID_ROWS][GRID_COLS];
static unsigned int random_tile_number;
static int rooms_cleared = 0;
static int map_idx = 0;

enum tile_type { FLOOR_TILE, WALL_TILE, MOB_TILE, BOSS_TILE };
static room_state state = loading;

// map of the tiles that the game will draw
static int tilemap[GRID_ROWS][GRID_COLS];
CP_Image tile_list[ROOM_TILE_TYPES];

//for the window width and height
//float W_width = WALL_DIM * GRID_COLS;
//float W_height = WALL_DIM * GRID_ROWS;
Position doors[4];

static void pause_menu(void) {


	CP_Settings_RectMode(CP_POSITION_CENTER);
	CP_Graphics_DrawRect(480, 480, 480, 480);

}

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
				random_tile_number = CP_Random_RangeInt(0, 100);
				tilemap[i][j] = (random_tile_number > 5) ? 0 : 1;
				break;
			case WALL_TILE:
				room_wall_pos[i][j] = WALL_TILE;
				random_tile_number = CP_Random_RangeInt(0, 90);
				tilemap[i][j] = (random_tile_number < 30) ? 2 : (random_tile_number < 60) ? 3 : 4;
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

void draw_door(void) {
	//for (int i = 0; i < GRID_ROWS; i++) {
	//	for (int j = 0; j < GRID_COLS; ++j) {
	//		if (door_pos[i][j]) {
	//			CP_Settings_StrokeWeight(0.0);
	//			CP_Graphics_DrawRect(get_camera_x_pos(j * WALL_DIM), get_camera_y_pos(i * WALL_DIM), WALL_DIM, WALL_DIM);
	//		}
	//	}
	//}
	CP_Settings_StrokeWeight(0.0);

	// top door
	CP_Graphics_DrawRect(get_camera_x_pos(doors[0].x), get_camera_y_pos(doors[0].y), WALL_DIM * 4.0f, WALL_DIM);
	// bottom door
	CP_Graphics_DrawRect(get_camera_x_pos(doors[1].x), get_camera_y_pos(doors[1].y), WALL_DIM * 4.0f, WALL_DIM);
	// left door
	CP_Graphics_DrawRect(get_camera_x_pos(doors[2].x), get_camera_y_pos(doors[2].y), WALL_DIM, WALL_DIM * 4.0f);
	// right door
	CP_Graphics_DrawRect(get_camera_x_pos(doors[3].x), get_camera_y_pos(doors[3].y), WALL_DIM, WALL_DIM * 4.0f);
}

void draw_room_wall(void) {

	CP_Settings_ImageWrapMode(CP_IMAGE_WRAP_CLAMP_EDGE);
	
	/*for (int i = 1; i < GRID_COLS - 1; i += 2) {
		CP_Image_Draw(TopWall, ((i+1) * WALL_DIM), (WALL_DIM), WALL_DIM *2, WALL_DIM *2, 255);
		CP_Image_Draw(BottomWall,((i + 1) * WALL_DIM), CP_System_GetWindowHeight() - (WALL_DIM), WALL_DIM * 2, WALL_DIM * 2, 255);
	}
	
	
	for (int i = 1; i < GRID_ROWS-1; i += 2) {
		CP_Image_Draw(LeftWall, (WALL_DIM), ((i + 1) * WALL_DIM), WALL_DIM * 2, WALL_DIM * 2, 255);
		CP_Image_Draw(RightWall, CP_System_GetWindowWidth() - (WALL_DIM), ((i + 1) * WALL_DIM), WALL_DIM * 2, WALL_DIM * 2, 255);
	}*/
	
	for (int i = 0; i < GRID_ROWS; i++) {
		for (int j = 0; j < GRID_COLS; ++j) {
			if (tilemap[i][j]>1 && room_wall_pos[i][j] == WALL_TILE) {
				CP_Image_Draw(tile_list[tilemap[i][j]], get_camera_x_pos((j * WALL_DIM) + WALL_DIM / 2), get_camera_y_pos((i * WALL_DIM) + WALL_DIM / 2), WALL_DIM, WALL_DIM, 255);
			}
		}
	}

}

void draw_room_floor(void) {
	for (int i = 0; i < GRID_ROWS; i++) {
		for (int j = 0; j < GRID_COLS; ++j) {
			if (tilemap[i][j]==1) { //Draw a flat floor bellow wall/object
				CP_Settings_ImageWrapMode(CP_IMAGE_WRAP_CLAMP_EDGE);
				CP_Image_Draw(tile_list[tilemap[i][j]], get_camera_x_pos((j * WALL_DIM) + WALL_DIM / 2), get_camera_y_pos((i * WALL_DIM) + WALL_DIM / 2), WALL_DIM, WALL_DIM, 255);
			}
		}
	}
}

void game_init(void)
{
	// top door
	doors[0] = (Position){ ((float)CP_System_GetWindowWidth() / 2.0f) - (WALL_DIM * 2.0f), 0.0f};
	// bottom door
	doors[1] = (Position){ ((float)CP_System_GetWindowWidth() / 2.0f) - (WALL_DIM * 2.0f), (float)CP_System_GetWindowHeight() - WALL_DIM };

	//left door
	doors[2] = (Position){ 0.0f, ((float)CP_System_GetWindowHeight() / 2.0f) - (WALL_DIM * 2.0f) };
	//right door
	doors[3] = (Position){(float)CP_System_GetWindowWidth() - WALL_DIM, ((float)CP_System_GetWindowHeight() / 2.0f) - (WALL_DIM * 2.0f) };
	srand(time(0));
	init_sprites();
	rooms_cleared = 0;
	map_idx = 0;
	state = loading;
	//initialized the player as idx 0
	for (int i = 0; i < ENTITY_CAP; ++i) {
		entities[i].type = entity_null;
	}
	entities[PLAYER_IDX] = (Entity){
		entity_player,
		init_player()
	};


	load_maps();
	generate_door();
}

void game_update(void)
{
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
	if (state == room_failed) {

		// draw ur room failed stuff here
		clear_all_entities();
		if (CP_Input_KeyTriggered(KEY_ESCAPE)) exit(EXIT_SUCCESS);
		else if (CP_Input_KeyTriggered(KEY_R)) CP_Engine_SetNextGameState(Main_Menu_Init, Main_Menu_Update, Main_Menu_Exit);
	}
	else if (state == room_pause) {

		//resume game
		if (CP_Input_KeyTriggered(KEY_ESCAPE)) {

			// explicitly goes to room_active state, or itll cause undefined behavior
			// room_active will check if the game can go to room_cleared
			state = room_active;//!room_pause;
		}
		//exit to main menu
		else if (IsAreaClicked(480, 480, 480, 480, CP_Input_GetMouseX(), CP_Input_GetMouseY())) {

			if (CP_Input_MouseClicked(MOUSE_BUTTON_LEFT)) {

				CP_Engine_SetNextGameState(Main_Menu_Init, Main_Menu_Update, Main_Menu_Exit);
				state = loading;
			}
		}
	}
	else {
		if (state == loading) {
			//if it is boss room
			if ((rooms_cleared % BOSS_ROOM_INTERVAL) == (BOSS_ROOM_INTERVAL - 1)) {
				map_idx = 0;
			}
			else {
				map_idx = rand() % 10 + 1; //set map idx to a random range between 0 to 4

			}
			generate_current_map();
			
			state = room_active;
		}
		else {
			//CP_Graphics_ClearBackground(CP_Color_Create(255, 255, 255, 255));
			for (int i = 0; i < ENTITY_CAP; ++i) {
				if (entities[i].type == entity_null) continue;
				switch (entities[i].type) {
				case entity_player: update_player(PLAYER_IDX, entities, room_wall_pos); break;
				case entity_mob: update_mob(i, PLAYER_IDX, entities, room_wall_pos); break;
				case entity_boss: update_boss(i, PLAYER_IDX, entities, room_wall_pos); break;
				case entity_projectile: update_projectile(i, entities, room_wall_pos); break;
				}
			}

			//ends the game
			if (entities[PLAYER_IDX].player.state == dead) state = room_failed;
			if (state == room_active) {
				for (int i = 0; i < ENTITY_CAP; ++i) {
					// if the entities are not player or null
					if (entities[i].type != entity_player && entities[i].type != entity_null && entities[i].type != entity_projectile)
						break;
					if (i == ENTITY_CAP - 1)
						state = room_clear;
					if (CP_Input_KeyTriggered(KEY_ESCAPE)) {

						state = room_pause;
					}
				}
			}
			else if (state == room_clear) {
				clear_all_entities();
				for (int i = 0; i < GRID_ROWS; ++i) {
					for (int j = 0; j < GRID_COLS; ++j) {
						Player* player = &(entities[PLAYER_IDX].player);
						if (state == loading) return;
						if (collisionCircleRect(player->pos, player->diameter / 2.0f, doors[0], DOOR_WIDTH, WALL_DIM)) {
							player->pos.y = (float)CP_System_GetWindowHeight() - (player->diameter / 2.0f);
							rooms_cleared++;
							state = loading;
						}
						else if (collisionCircleRect(player->pos, player->diameter / 2.0f, doors[1], DOOR_WIDTH, WALL_DIM)) {
							player->pos.y = 0.0f + (player->diameter / 2.0f);
							rooms_cleared++;
							state = loading;
						}
						else if (collisionCircleRect(player->pos, player->diameter / 2.0f, doors[2], WALL_DIM, DOOR_WIDTH)) {
							player->pos.x = (float)CP_System_GetWindowWidth() - (player->diameter / 2.0f);
							rooms_cleared++;
							state = loading;
						}
						else if (collisionCircleRect(player->pos, player->diameter / 2.0f, doors[3], WALL_DIM, DOOR_WIDTH)) {
							player->pos.x = 0.0f + (player->diameter / 2.0f);
							rooms_cleared++;
							state = loading;
						}
					}
				}
				if (CP_Input_KeyTriggered(KEY_ESCAPE)) {

					state = room_pause;
				}
			}
		}
	}

	draw_all(entities, tilemap, state);
}

void game_exit(void)
{

}

