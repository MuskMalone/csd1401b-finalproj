
#include "game.h"
#define PLAYER_IDX 0
#define ENTITY_CAP 100

// @todo auto resize the array
Entity entities[ENTITY_CAP];

void game_init(void)
{
	for (int i = 0; i < ENTITY_CAP; ++i) {
		entities[i].type = entity_null;
	}
	Player p = init_player();
	entities[PLAYER_IDX].type = entity_player;
	entities[PLAYER_IDX].player = p;
	//Mob m = init_mob();
	//entities[1].type = entity_mob;
	//entities[1].mob = m;
}

void game_update(void)
{
	CP_Graphics_ClearBackground(CP_Color_Create(150,150,150,255));
	for (int i = 0; i < ENTITY_CAP; ++i) {
		if (entities[i].type == entity_null) continue;
		switch (entities[i].type) {
			case entity_player: update_player(PLAYER_IDX, entities); break;
			case entity_mob: update_mob(i, PLAYER_IDX, entities); break;
			case entity_boss: break;
			case entity_projectile: break;
		}
	}

	Position test;
	float dim = 50.0f;
	test.x = 100.0f; test.y = 100.f;
	CP_Settings_StrokeWeight(0.0f);
	
	if (collisionCircleRect(entities[PLAYER_IDX].player.pos, entities[PLAYER_IDX].player.diameter/2.0f, test, dim, dim))
		CP_Settings_Fill(CP_Color_Create(51, 255, 173, 255));
	else CP_Settings_Fill(CP_Color_Create(255, 255, 255, 255));
	
	CP_Graphics_DrawRect(test.x, test.y, dim, dim);
}

void game_exit(void)
{
	
}

