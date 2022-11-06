
#include "game.h"
#include <stdio.h>
#include <stdlib.h>
#define PLAYER_IDX 0
#define BOSS_IDX 1
#define ENTITY_CAP 100
#define SIZE 6
// to disable vs warning for fopen function
#pragma warning(disable : 4996)

// @todo auto resize the array
Entity entities[ENTITY_CAP];
int wall_pos[SIZE][GRID_ROWS][GRID_COLS];

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
	
}

void game_update(void)
{
	int num = 1;
	CP_Graphics_ClearBackground(CP_Color_Create(150, 150, 150, 255));

	// calls each entity's update function
	for (int i = 0; i < ENTITY_CAP; ++i) {
		if (entities[i].type == entity_null) continue;
		switch (entities[i].type) {
		case entity_player: update_player(PLAYER_IDX, entities, wall_pos[num]); break;
		case entity_mob: update_mob(i, PLAYER_IDX, entities); break;
		case entity_boss: break;
		case entity_projectile: break;
		}
	}
	for (int i = 0; i < GRID_ROWS; i++) {
		for (int j = 0; j < GRID_COLS; ++j) {
			if (wall_pos[num][i][j]) {
				CP_Settings_StrokeWeight(0.0);
				CP_Graphics_DrawRect(j * WALL_DIM, i * WALL_DIM, WALL_DIM, WALL_DIM);
			}
		}
	}

}

void game_exit(void)
{

}

