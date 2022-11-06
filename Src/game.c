
#include "game.h"
#include <stdio.h>
#include <stdlib.h>

#define BOSS_IDX 1
// to disable vs warning for fopen function
#pragma warning(disable : 4996)

// @todo auto resize the array
Entity entities[ENTITY_CAP];
int wall_pos[GRID_ROWS][GRID_COLS];

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

	Boss b = init_boss();
	entities[BOSS_IDX].type = entity_boss;
	entities[BOSS_IDX].boss = b;
	Mob m = init_mob();
	entities[2].type = entity_mob;
	entities[2].mob = m;
	
}

void game_update(void)
{
	
	CP_Graphics_ClearBackground(CP_Color_Create(150,150,150,255));
	// calls each entity's update function
	for (int i = 0; i < ENTITY_CAP; ++i) {
		if (entities[i].type == entity_null) continue;
		switch (entities[i].type) {
			case entity_player: update_player(PLAYER_IDX, entities, wall_pos); break;
			case entity_mob: update_mob(i, PLAYER_IDX, entities); break;
			case entity_boss: update_boss(BOSS_IDX, PLAYER_IDX, entities, wall_pos); break;
			case entity_projectile: update_projectile(i, entities , wall_pos);break;
		}
	}
	for (int i = 0; i < GRID_ROWS; ++i) {
		for (int j = 0; j < GRID_COLS; ++j) {
			if (wall_pos[i][j]) {
				CP_Settings_StrokeWeight(0.0);
				CP_Graphics_DrawRect(WALL_DIM * (float)j, WALL_DIM * (float)i, WALL_DIM, WALL_DIM);
			}
		}
	}
	Position test;
	test.x = 100.0f; test.y = 100.0f;
	float diameter = 50.0f;
	CP_Settings_StrokeWeight(0.0f);

	if (collisionCircle(test, diameter/2.0f, entities[PLAYER_IDX].player.pos, entities[PLAYER_IDX].player.diameter/2.0f))
		CP_Settings_Fill(CP_Color_Create(51, 255, 173, 255));
	else CP_Settings_Fill(CP_Color_Create(255, 255, 255, 255));
	CP_Graphics_DrawCircle(test.x, test.y, diameter);
	 // spawn projectiles aiming at boss (For testing only)
	if (CP_Input_KeyTriggered(KEY_Q)) {
		for (int i = 0; i < ENTITY_CAP; ++i) {
			if (entities[i].type == entity_null) {
				//entities[BOSS_IDX].boss.health--;
				Position Mousepos = (Position){ CP_Input_GetMouseX(),CP_Input_GetMouseY() };
				Position startposb;
				startposb.x = ((float)CP_System_GetWindowWidth() * 3 / 4) - (10.0f);
				startposb.y = ((float)CP_System_GetWindowHeight() / 2) - (10.0f);
				Projectile projb = init_projectile('p', startposb, getVectorBetweenPositions(&(startposb), &(Mousepos)),'r');
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

