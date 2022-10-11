
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
	Position test, a_min, a_max, b_min, b_max;
	test.x = 100.0f; test.y = 100.f;
	float testradius = 50.0f;
	a_min.x = test.x - testradius / 2.0f; a_min.y = test.y - testradius / 2.0f;
	b_min.x = entities[PLAYER_IDX].player.pos.x - entities[PLAYER_IDX].player.diameter / 2.0f;
	b_min.y = entities[PLAYER_IDX].player.pos.y - entities[PLAYER_IDX].player.diameter / 2.0f;

	a_max.x = test.x + testradius / 2.0f; a_max.y = test.y + testradius / 2.0f;
	b_max.x = entities[PLAYER_IDX].player.pos.x + entities[PLAYER_IDX].player.diameter / 2.0f;
	b_max.y = entities[PLAYER_IDX].player.pos.y + entities[PLAYER_IDX].player.diameter / 2.0f;
	CP_Settings_StrokeWeight(0.0f);
	if (collisionAABB(a_min, a_max, b_min, b_max))
		CP_Settings_Fill(CP_Color_Create(51, 255, 173, 255));
	else CP_Settings_Fill(CP_Color_Create(255, 255, 255, 255));
	
	CP_Settings_RectMode(CP_POSITION_CENTER);
	CP_Graphics_DrawRect(test.x, test.y, testradius, testradius);
}

void game_exit(void)
{
	
}

