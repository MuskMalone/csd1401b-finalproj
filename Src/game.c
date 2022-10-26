
#include "game.h"
#define PLAYER_IDX 0
#define BOSS_IDX 1
#define ENTITY_CAP 100

// @todo auto resize the array
Entity entities[ENTITY_CAP];

void game_init(void)
{
	for (int i = 0; i < ENTITY_CAP; ++i) {
		entities[i].type = entity_null;
	}
	Player p = init_player();
	Boss b = init_boss();
	entities[PLAYER_IDX].type = entity_player;
	entities[PLAYER_IDX].player = p;
	entities[BOSS_IDX].type = entity_boss;
	entities[BOSS_IDX].boss = b;
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
			case entity_boss: update_boss(BOSS_IDX, PLAYER_IDX, entities); break;
			case entity_projectile: update_projectile(i, entities); break;
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
	/* // spawn projectiles aiming at boss (For testing only)
	if (CP_Input_KeyTriggered(KEY_Q)) {
		for (int i = 0; i < ENTITY_CAP; ++i) {
			if (entities[i].type == entity_null) {
				Position startpos;
				startpos.x = ((float)CP_System_GetWindowWidth() *1/ 4) - (10.0f);
				startpos.y = ((float)CP_System_GetWindowHeight() /2) - (10.0f);
				Projectile proj = init_projectile('p',startpos, getVectorBetweenPositions(&(startpos), &(entities[BOSS_IDX].boss.pos)));
				entities[i].type = entity_projectile;
				entities[i].projectile = proj;
				Position startposb;
				startposb.x = ((float)CP_System_GetWindowWidth() * 3 / 4) - (10.0f);
				startposb.y = ((float)CP_System_GetWindowHeight() / 2) - (10.0f);
				Projectile projb = init_projectile('p', startposb, getVectorBetweenPositions(&(startposb), &(entities[BOSS_IDX].boss.pos)));
				entities[i+1].type = entity_projectile;
				entities[i+1].projectile = projb;
				break;
			}
		}
	}
	*/
}

void game_exit(void)
{
	
}

