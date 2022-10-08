#include "cprocessing.h"
#include "utils.h"
#include "game.h"
#define ENTITY_CAP 100

Entity entities[ENTITY_CAP];

void game_init(void)
{
	for (int i = 0; i < ENTITY_CAP; ++i) {
		entities[i].type = entity_null;
	}
	Player p = init_player();
	entities[0].type = entity_player;
	entities[0].player = p;
}

void game_update(void)
{
	CP_Graphics_ClearBackground(CP_Color_Create(150,150,150,255));
	for (int i = 0; i < ENTITY_CAP; ++i) {
		if (entities[i].type == entity_null) continue;
		switch (entities[i].type) {
		case entity_player: update_player(); break;
		case entity_mob: break;
		case entity_boss: break;
		case entity_projectile: break;
		}
	}
}

void game_exit(void)
{
	
}

