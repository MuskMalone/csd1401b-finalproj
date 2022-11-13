#include "player.h"
#include "boss.h"
#include "mobs.h"
#include "projectiles.h"
#include "camera.h"
#include "game.h"
#include <stdio.h>
#include <stdlib.h>

CP_Image player_heart;

CP_Image Player_Barrier_Img;

CP_Image Cannon_Img;
CP_Image boss_def;
CP_Image Boss_Barrier_Img;
CP_Image Boss_Atk_Right[BOSS_SPRITE_COUNT];
CP_Image Boss_Atk_Left[BOSS_SPRITE_COUNT];

CP_Image Mobile_Proj_E;
CP_Image Mobile_Proj_P;
CP_Image player_projectile_sprites[MELEE_PROJECTILE_SPRITE_COUNT];
CP_Image enemy_projectile_sprites[MELEE_PROJECTILE_SPRITE_COUNT];

CP_Image melee_mob[MELEE_MOB_SPRITE_COUNT];
CP_Image explode_mob[MELEE_MOB_SPRITE_COUNT];
CP_Image range_mob[RANGE_MOB_SPRITE_COUNT];



void init_sprites(void) {
	player_heart = CP_Image_Load("./Assets/PlayerLife.png");
	Player_Barrier_Img = CP_Image_Load("./Assets/Tiles/Player/Player_Barrier4.png");

	Boss_Barrier_Img = CP_Image_Load("./Assets/Tiles/Boss/Boss_Barrier.png");
	Cannon_Img = CP_Image_Load("./Assets/Tiles/Boss/Cannon.png");
	boss_def = CP_Image_Load("./Assets/Tiles/Boss/Boss_Base3.png");

	Boss_Atk_Right[0] = CP_Image_Load("./Assets/Tiles/Boss/BossAtk1.png");
	Boss_Atk_Right[1] = CP_Image_Load("./Assets/Tiles/Boss/BossAtk2.png");
	Boss_Atk_Right[2] = CP_Image_Load("./Assets/Tiles/Boss/BossAtk3.png");
	Boss_Atk_Right[3] = CP_Image_Load("./Assets/Tiles/Boss/BossAtk4.png");
	Boss_Atk_Right[4] = CP_Image_Load("./Assets/Tiles/Boss/BossAtk5.png");
	Boss_Atk_Right[5] = CP_Image_Load("./Assets/Tiles/Boss/BossAtk6.png");
	Boss_Atk_Right[6] = CP_Image_Load("./Assets/Tiles/Boss/BossAtk7.png");

	Boss_Atk_Left[0] = CP_Image_Load("./Assets/Tiles/Boss/BossAtk1Left.png");
	Boss_Atk_Left[1] = CP_Image_Load("./Assets/Tiles/Boss/BossAtk2Left.png");
	Boss_Atk_Left[2] = CP_Image_Load("./Assets/Tiles/Boss/BossAtk3Left.png");
	Boss_Atk_Left[3] = CP_Image_Load("./Assets/Tiles/Boss/BossAtk4Left.png");
	Boss_Atk_Left[4] = CP_Image_Load("./Assets/Tiles/Boss/BossAtk5Left.png");
	Boss_Atk_Left[5] = CP_Image_Load("./Assets/Tiles/Boss/BossAtk6Left.png");
	Boss_Atk_Left[6] = CP_Image_Load("./Assets/Tiles/Boss/BossAtk7Left.png");

	Mobile_Proj_E = CP_Image_Load("./Assets/Tiles/Projectiles/Mobile_Proj_E.png");
	Mobile_Proj_P = CP_Image_Load("./Assets/Tiles/Projectiles/Mobile_Proj_P.png");
	player_projectile_sprites[0] = CP_Image_Load("./Assets/Tiles/Projectiles/Static_Proj_P1.png");
	player_projectile_sprites[1] = CP_Image_Load("./Assets/Tiles/Projectiles/Static_Proj_P2.png");
	player_projectile_sprites[2] = CP_Image_Load("./Assets/Tiles/Projectiles/Static_Proj_P3.png");
	player_projectile_sprites[3] = CP_Image_Load("./Assets/Tiles/Projectiles/Static_Proj_P4.png");
	player_projectile_sprites[4] = CP_Image_Load("./Assets/Tiles/Projectiles/Static_Proj_P5.png");
	player_projectile_sprites[5] = CP_Image_Load("./Assets/Tiles/Projectiles/Static_Proj_P6.png");

	enemy_projectile_sprites[0] = CP_Image_Load("./Assets/Tiles/Projectiles/Static_Proj_E1.png");
	enemy_projectile_sprites[1] = CP_Image_Load("./Assets/Tiles/Projectiles/Static_Proj_E2.png");
	enemy_projectile_sprites[2] = CP_Image_Load("./Assets/Tiles/Projectiles/Static_Proj_E3.png");
	enemy_projectile_sprites[3] = CP_Image_Load("./Assets/Tiles/Projectiles/Static_Proj_E4.png");
	enemy_projectile_sprites[4] = CP_Image_Load("./Assets/Tiles/Projectiles/Static_Proj_E5.png");
	enemy_projectile_sprites[5] = CP_Image_Load("./Assets/Tiles/Projectiles/Static_Proj_E6.png");

	melee_mob[0] = CP_Image_Load("./Assets/Tiles/Mobs/Melee/Slime_Idle1.png");
	melee_mob[1] = CP_Image_Load("./Assets/Tiles/Mobs/Melee/Slime_Idle2.png");

	explode_mob[0] = CP_Image_Load("./Assets/Tiles/Mobs/Explode/ExplodeSlime_Idle1.png");
	explode_mob[1] = CP_Image_Load("./Assets/Tiles/Mobs/Explode/ExplodeSlime_Idle2.png");

	//room tiles
	tile_list[1] = CP_Image_Load("./Assets/Tiles/tile_0012.png"); // rock floor
	tile_list[2] = CP_Image_Load("./Assets/Tiles/tile_0065.png"); // grave
	tile_list[3] = CP_Image_Load("./Assets/Tiles/tile_0074.png"); // anvil
	tile_list[4] = CP_Image_Load("./Assets/Tiles/tile_0082.png"); // barrel
	tile_list[5] = CP_Image_Load("./Assets/Tiles/TopWall.png");
	tile_list[6] = CP_Image_Load("./Assets/Tiles/BottomWall.png");
	tile_list[7] = CP_Image_Load("./Assets/Tiles/RightWall.png");
	tile_list[8] = CP_Image_Load("./Assets/Tiles/LeftWall.png");
}

void draw_all(Entity entities[], int tile_map[GRID_ROWS][GRID_COLS], room_state state) {

	CP_Settings_Fill(CP_Color_Create(0, 0, 0, 255));

	CP_Graphics_ClearBackground(CP_Color_Create(88, 88, 88, 255));

	if (state == room_failed) {

		// draw ur room failed stuff here
		CP_Settings_Fill(CP_Color_Create(0, 0, 0, 255));
		CP_Settings_TextSize(50.0f);

		char buffer[500] = { 0 };
		sprintf_s(buffer, _countof(buffer), "YOU DIED\nR to main menu, ESC to quit");
		CP_Font_DrawText(buffer, 200, 200);
	}
	else if (state == room_pause) {

		//draw the stuff here
		CP_Settings_RectMode(CP_POSITION_CENTER);
		CP_Graphics_DrawRect(480, 480, 480, 480);
	}
	else {
		if (state == loading) {

		}
		else {
			//CP_Graphics_ClearBackground(CP_Color_Create(255, 255, 255, 255));
			draw_room_floor();
			draw_room_wall();
			for (int i = 0; i < ENTITY_CAP; ++i) {
				if (entities[i].type == entity_null) continue;
				switch (entities[i].type) {
				case entity_player: draw_player(&(entities[i].player)); break;
				case entity_mob: draw_mob(&(entities[i].mob)); break;
				case entity_boss: draw_boss(&(entities[i].boss)); break;
				case entity_projectile: draw_projectile(&(entities[i].projectile)); break;
				}
			}

			//ends the game
			if (state == room_active) {
			}
			else if (state == room_clear) {
				draw_door();

			}
		}
	}

	draw_hud(&(entities[PLAYER_IDX].player));
}

void draw_hud(Player* player) {
	for (int i = 1; i <= player->health; i++)
	{
		CP_Image_Draw(player_heart, 32 * i, 64, 32, 32, 255);
	}
}