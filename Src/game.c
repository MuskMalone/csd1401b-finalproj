
#include "mainmenu.h"
#include "game.h"
#include <stdio.h>
#include <stdlib.h>

#define BOSS_IDX 1
#define BOSS_ROOM_INTERVAL 6
#define SIZE 11
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
typedef enum room_state { room_pause, room_active, room_clear, loading, room_failed } room_state;
enum tile_type { FLOOR_TILE, WALL_TILE, MOB_TILE, BOSS_TILE };
static room_state state = loading;

// map of the tiles that the game will draw
static int tilemap[GRID_ROWS][GRID_COLS];
CP_Image TopWall = NULL;
CP_Image BottomWall = NULL;
CP_Image RightWall = NULL;
CP_Image LeftWall = NULL;
CP_Image Rock_Floor = NULL;
CP_Image grave = NULL;
CP_Image Anvil = NULL;
CP_Image Barrel = NULL;
CP_Image ImageList[10];

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
			if (tilemap[i][j]>1) {
				CP_Image_Draw(ImageList[tilemap[i][j]], (j * WALL_DIM) + WALL_DIM / 2, (i * WALL_DIM) + WALL_DIM / 2, WALL_DIM, WALL_DIM, 255);
			}
		}
	}

}

static void draw_room_floor(void) {
	for (int i = 0; i < GRID_ROWS; i++) {
		for (int j = 0; j < GRID_COLS; ++j) {
			if (tilemap[i][j]==1) { //Draw a flat floor bellow wall/object
				CP_Settings_ImageWrapMode(CP_IMAGE_WRAP_CLAMP_EDGE);
				CP_Image_Draw(ImageList[tilemap[i][j]], (j * WALL_DIM) + WALL_DIM / 2, (i * WALL_DIM) + WALL_DIM / 2, WALL_DIM, WALL_DIM, 255);
			}
		}
	}
}

void game_init(void)
{
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
	
	TopWall = CP_Image_Load("./Assets/Tiles/TopWall.png");
	BottomWall = CP_Image_Load("./Assets/Tiles/BottomWall.png");
	RightWall = CP_Image_Load("./Assets/Tiles/RightWall.png");
	LeftWall = CP_Image_Load("./Assets/Tiles/LeftWall.png");
	Rock_Floor = CP_Image_Load("./Assets/Tiles/tile_0012.png");
	grave = CP_Image_Load("./Assets/Tiles/tile_0065.png");
	Anvil = CP_Image_Load("./Assets/Tiles/tile_0074.png");
	Barrel = CP_Image_Load("./Assets/Tiles/tile_0082.png");
	ImageList[1] = Rock_Floor;
	ImageList[2] = grave;
	ImageList[3] = Anvil;
	ImageList[4] = Barrel;

	
}

void game_update(void)
{
	CP_Settings_Fill(CP_Color_Create(0, 0, 0, 255));

	CP_Graphics_ClearBackground(CP_Color_Create(88, 88, 88, 255));
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
		CP_Settings_Fill(CP_Color_Create(0, 0, 0, 255));
		CP_Settings_TextSize(50.0f);

		char buffer[500] = { 0 };
		sprintf_s(buffer, _countof(buffer), "YOU DIED\nR to main menu, ESC to quit");
		CP_Font_DrawText(buffer, 200, 200);
		if (CP_Input_KeyTriggered(KEY_ESCAPE)) exit(EXIT_SUCCESS);
		else if (CP_Input_KeyTriggered(KEY_R)) CP_Engine_SetNextGameState(Main_Menu_Init, Main_Menu_Update, Main_Menu_Exit);
	}
	else if (state == room_pause) {

		//draw the stuff here
		pause_menu();

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
			draw_room_floor();
			draw_room_wall();
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
					if (entities[i].type != entity_player && entities[i].type != entity_null)
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
				draw_door();
				for (int i = 0; i < GRID_ROWS; ++i) {
					for (int j = 0; j < GRID_COLS; ++j) {
						if (door_pos[i][j]) {
							if (collisionCircleRect(entities[PLAYER_IDX].player.pos, entities[PLAYER_IDX].player.diameter / 2.0f, (Position) { WALL_DIM* (float)j, WALL_DIM* (float)i }, WALL_DIM, WALL_DIM)) { //when touch door
								if (state != loading) rooms_cleared++;
								state = loading;
							}
							if (CP_Input_KeyTriggered(KEY_ESCAPE)) {

								state = room_pause;
							}
						}
					}
				}

			}
		}
	}
	
	CP_Settings_Fill(CP_Color_Create(0, 0, 0, 255));
	CP_Settings_TextSize(20.0f);

	char buffer[500] = { 0 };
	sprintf_s(buffer, _countof(buffer), "room state: %d, cleared rooms: %d", state, rooms_cleared);
	CP_Font_DrawText(buffer, 30, 50);	CP_Font_DrawText(buffer, 30, 50);

}

void game_exit(void)
{

}

