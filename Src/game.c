/*
@file game.c
@author Shannon Teo
@course CSD1401
@section B
@brief where the main game runs
*/
/*______________________________________________________________________*/
#include "gamestates.h"
#include "camera.h"
#include "game.h"
#include "button.h"
#include "sounds.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define BOSS_IDX 1
#define BOSS_ROOM_INTERVAL 6
#define SIZE 11
#define DOOR_WIDTH WALL_DIM * 4.0f

// to disable vs warning for fopen function
#pragma warning(disable : 4996)

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
int isplaying;
CP_Sound bgm;
CP_Sound bossbgm;
CP_Sound defeat;
CP_Image DoorTop;
CP_Image DoorLeft;
CP_Image DoorRight;
CP_Image DoorBot;

static room_state state = loading;

static int tilemap[GRID_ROWS][GRID_COLS];
CP_Image tile_list[ROOM_TILE_TYPES];

Position doors[DOOR_COUNT];

/*
This function set the next game state back to the main menu from the pause screen.
*/
void back_to_mainmenu(void){
	CP_Engine_SetNextGameState(Main_Menu_Init, Main_Menu_Update, Main_Menu_Exit);
}

/*
This function sets the game state to room_active.
*/
void resume_game(void) {
	state = room_active;
}

/*
This function loads in the different rooms for the levels from a text file using the
function atoi. Its reads a series of 1s and 0s and set the value in the array to 1(wall)
or 0(floor).
*/
static void load_maps(void) {

	int ch, idx;

	FILE* map = fopen("Assets/maps.txt", "r");
	for (idx = 0; idx < SIZE; ++idx) {
		for (int i = 0; i < GRID_ROWS; i++) {
			for (int j = 0; j < GRID_COLS; ++j) {
				ch = fgetc(map);

				map_pos[idx][i][j] = atoi((char*)&ch);
			}
		}

		ch = fgetc(map);
	}

	fclose(map);
}

/*
This function clears the map all entities except player. 
*/
static void clear_all_entities(void) {
	for (int i = 0; i < ENTITY_CAP; ++i) {
		if (entities[i].type != entity_player)
			entities[i].type = entity_null;
	}
}

/*
This function generates the current map and uses the array from room_wall_pos to
determine which tile is drawn.
*/
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
			
		}
	}
}

/*
This functions draws the main menu and exit buttons on the game over screen.
*/
void draw_room_failed_buttons(void) {
	for (int i = 0; i < 2; ++i) {
		draw_button(&(fail_menu_btns[i]));
	}
}

/*
This functions draws the main menu and exit buttons on the pause screen.
*/
void draw_pause_menu_btns(void) {
	for (int i = 0; i < 2; ++i) {
		draw_button(&(pause_menu_btns[i]));
	}
}

/*
This functions draws the door animation.
*/
void draw_door(float timer, CP_Image sprites2d[DOOR_COUNT][DOOR_SPRITE_COUNT]) {


	int idx = (int)(timer / (DOOR_MAX_TIMER / (float)DOOR_SPRITE_COUNT));
	if (idx == DOOR_SPRITE_COUNT) --idx;
	CP_Image_Draw(
		sprites2d[0][idx], 
		get_camera_x_pos(doors[0].x + (WALL_DIM * 2.0f)), 
		get_camera_y_pos(doors[0].y + (WALL_DIM)), 
		WALL_DIM * 4.0f, WALL_DIM * 2.0f, 255);
	// bottom door
	CP_Image_Draw(
		sprites2d[1][idx], 
		get_camera_x_pos(doors[1].x + (WALL_DIM * 2.0f)),
		get_camera_y_pos(doors[1].y),
		WALL_DIM * 4.0f, WALL_DIM * 2.0f, 255);
	// left door
	CP_Image_Draw(
		sprites2d[2][idx], 
		get_camera_x_pos(doors[2].x + (WALL_DIM)),
		get_camera_y_pos(doors[2].y + (WALL_DIM * 2.0f)),
		WALL_DIM * 2.0f, WALL_DIM * 4.0f, 255);
	// right door
	CP_Image_Draw(
		sprites2d[3][idx], 
		get_camera_x_pos(doors[3].x),
		get_camera_y_pos(doors[3].y + (WALL_DIM * 2.0f)),
		WALL_DIM * 2.0f, WALL_DIM * 4.0f, 255);
}

/*
This function draws the obstacles in the room. 
*/
void draw_room_wall(void) {

	CP_Settings_ImageWrapMode(CP_IMAGE_WRAP_CLAMP_EDGE);
	
	for (int i = 0; i < GRID_ROWS; i++) {
		for (int j = 0; j < GRID_COLS; ++j) {
			if (tilemap[i][j]>1 && room_wall_pos[i][j] == WALL_TILE) {
				CP_Image_Draw(tile_list[tilemap[i][j]], get_camera_x_pos((j * WALL_DIM) + WALL_DIM / 2), get_camera_y_pos((i * WALL_DIM) + WALL_DIM / 2), WALL_DIM, WALL_DIM, 255);
			}
		}
	}

}

/*
This function draws the floor tiles.
*/
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

/*
This function is to initialise the game state.
*/
void game_init(void)
{
	isplaying = 0;

	//sets the door positions in the room
	doors[0] = (Position){ ((float)CP_System_GetWindowWidth() / 2.0f) - (WALL_DIM * 2.0f), 0.0f};
	doors[1] = (Position){ ((float)CP_System_GetWindowWidth() / 2.0f) - (WALL_DIM * 2.0f), (float)CP_System_GetWindowHeight() - WALL_DIM};
	doors[2] = (Position){ 0.0f, ((float)CP_System_GetWindowHeight() / 2.0f) - (WALL_DIM * 2.0f) };
	doors[3] = (Position){(float)CP_System_GetWindowWidth() - WALL_DIM, ((float)CP_System_GetWindowHeight() / 2.0f) - (WALL_DIM * 2.0f) };
	
	//draws the buttons for game over state and when button is clicked, set to next game state.
	fail_menu_btns[0].image = &(game_button_sprites[MENU_BUTTON]);
	fail_menu_btns[0].on_click_func = back_to_mainmenu;
	fail_menu_btns[1].image = &(game_button_sprites[EXIT_BUTTON]);
	fail_menu_btns[1].on_click_func = CP_Engine_Terminate;

	//draws the buttons for pause menu state and when button is clicked, set to next game state.
	pause_menu_btns[0].image = &(game_button_sprites[RESUME_BUTTON]);
	pause_menu_btns[0].on_click_func = resume_game;
	pause_menu_btns[1].image = &(game_button_sprites[EXIT_BUTTON]);
	pause_menu_btns[1].on_click_func = CP_Engine_Terminate;

	//sets the postion of the buttons.
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

	//to set rng
	srand((unsigned int)time(0));

	//initialise the sprites and sounds.
	init_sprites();
	init_sounds();
	rooms_cleared = 0;
	map_idx = 0;
	state = loading;
	bossbgm = CP_Sound_Load("./Assets/SFX/Boss.ogg");
	bgm = CP_Sound_Load("./Assets/SFX/BGM1.ogg");
	defeat = CP_Sound_Load("./Assets/SFX/Comedy.ogg");


	for (int i = 0; i < ENTITY_CAP; ++i) {
		entities[i].type = entity_null;
	}
	entities[PLAYER_IDX] = (Entity){
		entity_player,
		init_player()
	};

	load_maps();
}

/*
This function is to initialise the game state.
*/
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
		
			if ((rooms_cleared % BOSS_ROOM_INTERVAL) == (BOSS_ROOM_INTERVAL - 1)) {
				map_idx = 0;
				if (isplaying == 1)
				{
					CP_Sound_StopAll();
					CP_Sound_PlayMusic(bossbgm);
					isplaying = 0;
				}
			}
			else {
				map_idx = rand() % 10 + 1;
				if (isplaying == 0) 
				{
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

			if (entities[PLAYER_IDX].player.state == dead) state = room_failed;
			if (state == room_active) {
				for (int i = 0; i < ENTITY_CAP; ++i) {

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
	update_sounds();
	draw_all(entities, tilemap,room_wall_pos, state);
}

/*
This function exit the game state.
*/
void game_exit(void)
{
	CP_Sound_StopAll();
	CP_Sound_Free(&defeat);
	CP_Sound_Free(&bgm);
	CP_Sound_Free(&bossbgm);
	free_sounds();
}

