
#include "gamestates.h"
#include "camera.h"
#include "game.h"
#include "button.h"
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
CP_Image game_button_sprites[BUTTON_SPRITE_COUNT];
Game_Button fail_menu_btns[2];
Game_Button pause_menu_btns[2];
int transition_side;
Entity entities[ENTITY_CAP];
static int map_pos[SIZE][GRID_ROWS][GRID_COLS];
static int door_pos[GRID_ROWS][GRID_COLS];
static int room_wall_pos[GRID_ROWS][GRID_COLS];
static unsigned int random_tile_number;
static int rooms_cleared = 0;
static int map_idx = 0;
int isplaying = 0;
CP_Sound bgm;
CP_Sound bossbgm;
CP_Sound defeat;
CP_Image DoorTop;
CP_Image DoorLeft;
CP_Image DoorRight;
CP_Image DoorBot;

static room_state state = loading;

// map of the tiles that the game will draw
static int tilemap[GRID_ROWS][GRID_COLS];
CP_Image tile_list[ROOM_TILE_TYPES];

//for the window width and height
//float W_width = WALL_DIM * GRID_COLS;
//float W_height = WALL_DIM * GRID_ROWS;
Position doors[DOOR_COUNT];

void back_to_mainmenu(void){
	CP_Engine_SetNextGameState(Main_Menu_Init, Main_Menu_Update, Main_Menu_Exit);
}
void resume_game(void) {
	state = room_active;
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
void generate_current_map(void) {
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
void draw_room_failed_buttons(void) {
	for (int i = 0; i < 2; ++i) {
		draw_button(&(fail_menu_btns[i]));
	}
}

void draw_pause_menu_btns(void) {
	for (int i = 0; i < 2; ++i) {
		draw_button(&(pause_menu_btns[i]));
	}
}
void draw_door(float timer, CP_Image sprites2d[DOOR_COUNT][DOOR_SPRITE_COUNT]) {
	//for (int i = 0; i < GRID_ROWS; i++) {
	//	for (int j = 0; j < GRID_COLS; ++j) {
	//		if (door_pos[i][j]) {
	//			CP_Settings_StrokeWeight(0.0);
	//			CP_Graphics_DrawRect(get_camera_x_pos(j * WALL_DIM), get_camera_y_pos(i * WALL_DIM), WALL_DIM, WALL_DIM);
	//		}
	//	}
	//}
	// top door
	//CP_Graphics_DrawRect(get_camera_x_pos(doors[0].x), get_camera_y_pos(doors[0].y), WALL_DIM * 4.0f, WALL_DIM);
	for (int i = 0; i < DOOR_COUNT; ++i) {
		CP_Image_Draw(sprites2d[i][(int) (timer / (DOOR_MAX_TIMER / (float)DOOR_SPRITE_COUNT))], get_camera_x_pos(doors[i].x) + 80, get_camera_y_pos(doors[i].y) + 20, WALL_DIM * 4.0f, WALL_DIM, 255);

	}
	//CP_Image_Draw(DoorTop, get_camera_x_pos(doors[0].x) + 80, get_camera_y_pos(doors[0].y) + 20, WALL_DIM * 4.0f, WALL_DIM, 255);
	//// bottom door
	//CP_Image_Draw(DoorBot, get_camera_x_pos(doors[1].x) + 80, get_camera_y_pos(doors[1].y) + 20, WALL_DIM * 4.0f, WALL_DIM, 255);
	//// left door
	//CP_Image_Draw(DoorLeft, get_camera_x_pos(doors[2].x) + 20, get_camera_y_pos(doors[2].y) + 80, WALL_DIM, WALL_DIM * 4.0f, 255);
	//// right door
	//CP_Image_Draw(DoorRight, get_camera_x_pos(doors[3].x) + 20, get_camera_y_pos(doors[3].y) + 80, WALL_DIM, WALL_DIM * 4.0f, 255);
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
	isplaying = 0;
	// top door
	doors[0] = (Position){ ((float)CP_System_GetWindowWidth() / 2.0f) - (WALL_DIM * 2.0f), 0.0f};
	// bottom door
	doors[1] = (Position){ ((float)CP_System_GetWindowWidth() / 2.0f) - (WALL_DIM * 2.0f), (float)CP_System_GetWindowHeight() - WALL_DIM };

	//left door
	doors[2] = (Position){ 0.0f, ((float)CP_System_GetWindowHeight() / 2.0f) - (WALL_DIM * 2.0f) };
	//right door
	doors[3] = (Position){(float)CP_System_GetWindowWidth() - WALL_DIM, ((float)CP_System_GetWindowHeight() / 2.0f) - (WALL_DIM * 2.0f) };
	
	fail_menu_btns[0].image = &(game_button_sprites[MENU_BUTTON]);
	fail_menu_btns[0].on_click_func = back_to_mainmenu;
	fail_menu_btns[1].image = &(game_button_sprites[EXIT_BUTTON]);
	fail_menu_btns[1].on_click_func = CP_Engine_Terminate;

	pause_menu_btns[0].image = &(game_button_sprites[RESUME_BUTTON]);
	pause_menu_btns[0].on_click_func = resume_game;
	pause_menu_btns[1].image = &(game_button_sprites[EXIT_BUTTON]);
	pause_menu_btns[1].on_click_func = CP_Engine_Terminate;

	for (int i = 0; i < 2; ++i) {
		fail_menu_btns[i].pos = (Position){
			((float)CP_System_GetWindowWidth() * ((float)i + 1.0f)) / 3.0f,
			(float)CP_System_GetWindowHeight() * 3.0f / 4.0f
		};
		fail_menu_btns[i].size = (Position){ WALL_DIM * 6.0f, WALL_DIM * 1.0f };
		fail_menu_btns[i].scale = 1.0f;
		fail_menu_btns[i].timer = 0.0f;

		pause_menu_btns[i].pos = (Position){
			(float)CP_System_GetWindowWidth() / 2.0f,
			(float)CP_System_GetWindowHeight() * ((float)i + 5.0f) / 10.0f
		};
		pause_menu_btns[i].size = (Position){ WALL_DIM * 6.0f, WALL_DIM * 1.0f };
		pause_menu_btns[i].scale = 1.0f;
		pause_menu_btns[i].timer = 0.0f;
	}

	srand((unsigned int)time(0));
	init_sprites();
	rooms_cleared = 0;
	map_idx = 0;
	state = loading;
	bossbgm = CP_Sound_Load("./Assets/SFX/Boss.wav");
	bgm = CP_Sound_Load("./Assets/SFX/BGM1.wav");
	//defeat = CP_Sound_Load("./Assets/SFX/Defeat.wav");
	defeat = CP_Sound_Load("./Assets/SFX/Comedy.wav");

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
	if (CP_Input_KeyTriggered(KEY_F)) {
		clear_all_entities();
	}
	if (state == room_failed) {
		if (isplaying == 1 || isplaying == 0)
		{
			CP_Sound_StopAll();
			CP_Sound_PlayMusic(defeat);
			//play defeat music
			isplaying = 2;
		}
		clear_all_entities();

		for (int i = 0; i < 2; ++i) {
			update_button(&(fail_menu_btns[i]));
		}
	}
	else if (state == room_pause) {
		for (int i = 0; i < 2; ++i) {
			update_button(&(pause_menu_btns[i]));
		}
	}
	else {
		if (state == loading) {
			//if it is boss room
			if ((rooms_cleared % BOSS_ROOM_INTERVAL) == (BOSS_ROOM_INTERVAL - 1)) {
				map_idx = 0;
				if (isplaying == 1)
				{
					CP_Sound_StopAll();
					CP_Sound_PlayMusic(bossbgm);
					//play boss scene music
					isplaying = 0;
				}
			}
			else {
				map_idx = rand() % 10 + 1; //set map idx to a random range between 0 to 4
				if (isplaying == 0) 
				{
					//play bgm
					CP_Sound_StopAll();
					CP_Sound_PlayMusic(bgm);
					isplaying = 1;
				}
			}
			generate_current_map();
			state = room_active;
		}
		else {
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

				Player* player = &(entities[PLAYER_IDX].player);
				if (state == loading) return;
				if (collisionCircleRect(player->pos, player->diameter / 2.0f, doors[0], DOOR_WIDTH, WALL_DIM)) {
					player->pos.y = (float)CP_System_GetWindowHeight() - (player->diameter / 2.0f);
					rooms_cleared++;
					transition_side = 0;
					state = loading;
				}
				else if (collisionCircleRect(player->pos, player->diameter / 2.0f, doors[1], DOOR_WIDTH, WALL_DIM)) {
					player->pos.y = 0.0f + (player->diameter / 2.0f);
					rooms_cleared++;
					transition_side = 1;
					state = loading;
				}
				else if (collisionCircleRect(player->pos, player->diameter / 2.0f, doors[2], WALL_DIM, DOOR_WIDTH)) {
					player->pos.x = (float)CP_System_GetWindowWidth() - (player->diameter / 2.0f);
					rooms_cleared++;
					transition_side = 2;
					state = loading;
				}
				else if (collisionCircleRect(player->pos, player->diameter / 2.0f, doors[3], WALL_DIM, DOOR_WIDTH)) {
					player->pos.x = 0.0f + (player->diameter / 2.0f);
					rooms_cleared++;
					transition_side = 3;
					state = loading;
				}
				if (CP_Input_KeyTriggered(KEY_ESCAPE)) {

					state = room_pause;
				}
			}
		}
	}

	draw_all(entities, tilemap,room_wall_pos, state);
}

void game_exit(void)
{
	CP_Sound_StopAll();
	CP_Sound_Free(&defeat);
	CP_Sound_Free(&bgm);
	CP_Sound_Free(&bossbgm);

}

void load_room_done(void) {
	
}

