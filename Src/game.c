
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
int tilemap[GRID_ROWS][GRID_COLS];
CP_Image Flat_Floor = NULL;
CP_Image Rock_Floor = NULL;
CP_Image grave = NULL;
CP_Image Anvil = NULL;
CP_Image Barrel = NULL;
CP_Image ImageList[10];

void game_init(void)
{
	//int max_mobs = 20;
	//int mob_idx = 1;
	for (int i = 0; i < ENTITY_CAP; ++i) {
		entities[i].type = entity_null;
	}
	entity_struct p = init_player();
	entities[PLAYER_IDX] = (Entity){
		entity_player,
		p
	};

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

	insert_to_entity_array(entity_boss, entities, init_boss);
	//Mob m = init_mob();
	//entities[2].type = entity_mob;
	//entities[2].mob = m;
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



	int num = 1;
	CP_Graphics_ClearBackground(CP_Color_Create(255, 255, 255, 255));
	for (int i = 0; i < GRID_ROWS; i++) {
		for (int j = 0; j < GRID_COLS; ++j) {
			if (wall_pos[num][i][j]) {
				unsigned int random_int = CP_Random_RangeInt(0, 90);
				tilemap[i][j] = (random_int < 30) ? 2 : (random_int < 60) ? 3 : 4;
			}
			else {
				unsigned int random_int = CP_Random_RangeInt(0, 100);
				tilemap[i][j] = (random_int > 5) ? 0 :1;
			}
		}
	}

	//Wall = CP_Image_Load("./Assets/Justins_face.png");
	
}

void game_update(void)
{
	//Floor = CP_Image_Load("./Assets/Tiles/tile_0000.png");
	int num = 1;
	CP_Settings_ImageWrapMode(CP_IMAGE_WRAP_CLAMP_EDGE);
	CP_Graphics_ClearBackground(CP_Color_Create(255,255,255, 255));
	//CP_Settings_ImageWrapMode(CP_IMAGE_WRAP_CLAMP_EDGE);
	for (int i = 0; i < GRID_ROWS; i++) {
		for (int j = 0; j < GRID_COLS; ++j) {
			if (tilemap[i][j] > 1)
				CP_Image_Draw(Flat_Floor, (j * WALL_DIM) + WALL_DIM / 2, (i * WALL_DIM) + WALL_DIM / 2, WALL_DIM, WALL_DIM, 255);
			else {
				CP_Image_Draw(ImageList[tilemap[i][j]], (j * WALL_DIM) + WALL_DIM / 2, (i * WALL_DIM) + WALL_DIM / 2, WALL_DIM, WALL_DIM, 255);
			}
		}
	}
	for (int i = 0; i < GRID_ROWS; i++) {
		for (int j = 0; j < GRID_COLS; ++j) {
			if (tilemap[i][j] > 1) {
				CP_Image_Draw(ImageList[tilemap[i][j]], (j * WALL_DIM) + WALL_DIM / 2, (i * WALL_DIM) + WALL_DIM / 2, WALL_DIM*1.2 , WALL_DIM*1.2, 255);
			}
		}
	}

	// calls each entity's update function
	for (int i = 0; i < ENTITY_CAP; ++i) {
		if (entities[i].type == entity_null) continue;
		switch (entities[i].type) {
		case entity_player: update_player(PLAYER_IDX, entities, wall_pos[num]); break;
		case entity_mob: update_mob(i, PLAYER_IDX, entities); break;
		case entity_boss: update_boss(BOSS_IDX, PLAYER_IDX, entities, wall_pos[num]); break;
		case entity_projectile: update_projectile(i, entities, wall_pos[num]); break;
		}
	}
	if (CP_Input_KeyTriggered(KEY_Q)) {
		Position Mousepos = (Position){ CP_Input_GetMouseX(),CP_Input_GetMouseY() };
		Position startposb;
		startposb.x = ((float)CP_System_GetWindowWidth() / 2);
		startposb.y = ((float)CP_System_GetWindowHeight() * 1 / 4);
		int p_idx = insert_to_entity_array(entity_projectile, entities, init_projectile);
		if (p_idx > -1) {
			set_projectile_values(&(entities[p_idx].projectile), 'p', 'm', 10, startposb, getVectorBetweenPositions(&(startposb), &(Mousepos)));
		}
	}

	/*if (CP_Input_KeyTriggered(KEY_1))
		CP_Settings_ImageWrapMode(CP_IMAGE_WRAP_CLAMP);
	else if (CP_Input_KeyTriggered(KEY_2))
		CP_Settings_ImageWrapMode(CP_IMAGE_WRAP_CLAMP_EDGE);
	else if (CP_Input_KeyTriggered(KEY_3))
		CP_Settings_ImageWrapMode(CP_IMAGE_WRAP_REPEAT);
	else if (CP_Input_KeyTriggered(KEY_4))
	CP_Settings_ImageWrapMode(CP_IMAGE_WRAP_MIRROR);*/
}

void game_exit(void)
{

}

