
#include "game.h"
#include <stdio.h>
#include <stdlib.h>
#define PLAYER_IDX 0
#define ENTITY_CAP 100
// to disable vs warning for fopen function
#pragma warning(disable : 4996)

// @todo auto resize the array
Entity entities[ENTITY_CAP];
int wall_pos[GRID_ROWS][GRID_COLS];

void game_init(void)
{
	for (int i = 0; i < ENTITY_CAP; ++i) {
		entities[i].type = entity_null;
	}
	Player p = init_player();
	entities[PLAYER_IDX].type = entity_player;
	entities[PLAYER_IDX].player = p;
	Mob m = init_mob();
	entities[1].type = entity_mob;
	entities[1].mob = m;

	int ch, newline = 0;
	// loads the map delimited by \n into the wall_pos array
	FILE* map = fopen("Assets/maps.txt", "r");
	for (int i = 0; i < GRID_ROWS && !newline; ++i) {
		for (int j = 0; j < GRID_COLS; ++j) {
			ch = fgetc(map);
			// if encounter a \n char, break out of the entire loop
			if (ch == '\n') {
				newline = 1;
				break;
			}
			wall_pos[i][j] = atoi((char*)&ch);
		}
	}
	fclose(map);
}

void game_update(void)
{
	
	CP_Graphics_ClearBackground(CP_Color_Create(150,150,150,255));

	// calls each entity's update function
	for (int i = 0; i < ENTITY_CAP; ++i) {
		if (entities[i].type == entity_null) continue;
		switch (entities[i].type) {
			case entity_player: update_player(PLAYER_IDX, entities); break;
			case entity_mob: update_mob(i, PLAYER_IDX, entities); break;
			case entity_boss: break;
			case entity_projectile: break;
		}
	}

}

void game_exit(void)
{
	
}

