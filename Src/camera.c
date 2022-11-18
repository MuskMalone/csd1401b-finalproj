#include "player.h"
#include "boss.h"
#include "mobs.h"
#include "projectiles.h"
#include "camera.h"
#include "game.h"
#include "utils.h"
#include "easing.h"
#include "gametypes.h"
#include <stdio.h>
#include <stdlib.h>
#define TRANSITION_TIMER .5f

CP_Image player_heart;
CP_Image Pause_Menu;

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

Position world_offset;

int shaking = 0;
float shaking_scale = 1.0f;

int tilemap_copied = 0;
int transitioning = 0;
int transition_side = -1;
float transition_timer = 0.0f;
int prev_room_tilemap[GRID_ROWS][GRID_COLS];

void init_sprites(void) {
	world_offset = (Position){ 0.0f, 0.0f };
	player_heart = CP_Image_Load("./Assets/PlayerLife.png");
	Player_Barrier_Img = CP_Image_Load("./Assets/Tiles/Player/Player_Barrier4.png");

	Boss_Barrier_Img = CP_Image_Load("./Assets/Tiles/Boss/Boss_Barrier.png");
	Cannon_Img = CP_Image_Load("./Assets/Tiles/Boss/Cannon.png");
	boss_def = CP_Image_Load("./Assets/Tiles/Boss/Boss_Base3.png");
	Pause_Menu = CP_Image_Load("./Assets/PauseMenu.png");
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

	range_mob[7] = CP_Image_Load("./Assets/Tiles/Mobs/Range/1_right.png");
	range_mob[6] = CP_Image_Load("./Assets/Tiles/Mobs/Range/1_rightup.png");
	range_mob[5] = CP_Image_Load("./Assets/Tiles/Mobs/Range/1_up.png");
	range_mob[4] = CP_Image_Load("./Assets/Tiles/Mobs/Range/1_leftup.png");
	range_mob[3] = CP_Image_Load("./Assets/Tiles/Mobs/Range/1_left.png");
	range_mob[2] = CP_Image_Load("./Assets/Tiles/Mobs/Range/1_leftdown.png");
	range_mob[1] = CP_Image_Load("./Assets/Tiles/Mobs/Range/1_down.png");
	range_mob[0] = CP_Image_Load("./Assets/Tiles/Mobs/Range/1_rightdown.png");

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

void draw_all(Entity entities[], int tile_map[GRID_ROWS][GRID_COLS], int room_wall_pos[GRID_ROWS][GRID_COLS], room_state state) {
	if (CP_Input_KeyDown(KEY_UP)) shake_camera(100.0f, 0);//world_offset.y += 10.0f;
	//else if (CP_Input_KeyDown(KEY_DOWN)) world_offset.y -= 10.0f;
	//if (CP_Input_KeyDown(KEY_RIGHT)) world_offset.x -= 10.0f;
	//else if (CP_Input_KeyDown(KEY_LEFT)) world_offset.x += 10.0f;

	CP_Settings_Fill(CP_Color_Create(0, 0, 0, 255));
	CP_Graphics_ClearBackground(CP_Color_Create(88, 88, 88, 255));
	if (shaking) {
		world_offset.x = CP_Random_RangeFloat(-shaking_scale, shaking_scale);
		world_offset.y = CP_Random_RangeFloat(-shaking_scale, shaking_scale);
		shaking_scale *= .9f;
		if (shaking_scale < 1.0f) {
			shaking = 0;
			shaking_scale = 0.0f;
			world_offset.x = 0.0f;
			world_offset.y = 0.0f;
		}
	}
	if (transitioning) {
		transition_timer += CP_System_GetDt();
		int multiplierx, multipliery;
		if (transition_timer >= TRANSITION_TIMER){
			transitioning = 0;
			world_offset.y = 0.0f; world_offset.x = 0.0f;
			transition_timer = 0.0f;
		}
		else {
			switch (transition_side) {
			case 0:
				world_offset.y = EaseOutExpo(-CP_System_GetWindowHeight(), 0.0f, transition_timer / TRANSITION_TIMER);
				for (int i = 0; i < GRID_ROWS; i++) {
					for (int j = 0; j < GRID_COLS; ++j) {
						if (prev_room_tilemap[i][j] == 1) { //Draw a flat floor bellow wall/object
							CP_Settings_ImageWrapMode(CP_IMAGE_WRAP_CLAMP_EDGE);
							CP_Image_Draw(tile_list[prev_room_tilemap[i][j]], get_camera_x_pos((j * WALL_DIM) + WALL_DIM / 2), get_camera_y_pos((i * WALL_DIM) + WALL_DIM / 2) + CP_System_GetWindowHeight(), WALL_DIM, WALL_DIM, 255);
						}
					}
				}
				for (int i = 0; i < GRID_ROWS; i++) {
					for (int j = 0; j < GRID_COLS; ++j) {
						if (prev_room_tilemap[i][j] > 1 && room_wall_pos[i][j] == WALL_TILE) {
							CP_Image_Draw(tile_list[prev_room_tilemap[i][j]], get_camera_x_pos((j * WALL_DIM) + WALL_DIM / 2), get_camera_y_pos((i * WALL_DIM) + WALL_DIM / 2) + CP_System_GetWindowHeight(), WALL_DIM, WALL_DIM, 255);
						}
					}
				}
				break;
			case 1:
				world_offset.y = EaseOutExpo(CP_System_GetWindowHeight(), 0.0f, transition_timer / TRANSITION_TIMER);
				for (int i = 0; i < GRID_ROWS; i++) {
					for (int j = 0; j < GRID_COLS; ++j) {
						if (prev_room_tilemap[i][j] == 1) { //Draw a flat floor bellow wall/object
							CP_Settings_ImageWrapMode(CP_IMAGE_WRAP_CLAMP_EDGE);
							CP_Image_Draw(tile_list[prev_room_tilemap[i][j]], get_camera_x_pos((j * WALL_DIM) + WALL_DIM / 2), get_camera_y_pos((i * WALL_DIM) + WALL_DIM / 2) - CP_System_GetWindowHeight(), WALL_DIM, WALL_DIM, 255);
						}
					}
				}
				for (int i = 0; i < GRID_ROWS; i++) {
					for (int j = 0; j < GRID_COLS; ++j) {
						if (prev_room_tilemap[i][j] > 1 && room_wall_pos[i][j] == WALL_TILE) {
							CP_Image_Draw(tile_list[prev_room_tilemap[i][j]], get_camera_x_pos((j * WALL_DIM) + WALL_DIM / 2), get_camera_y_pos((i * WALL_DIM) + WALL_DIM / 2) - CP_System_GetWindowHeight(), WALL_DIM, WALL_DIM, 255);
						}
					}
				}
				break;
			case 2:
				world_offset.x = EaseOutExpo(-CP_System_GetWindowWidth(), 0.0f, transition_timer / TRANSITION_TIMER);
				for (int i = 0; i < GRID_ROWS; i++) {
					for (int j = 0; j < GRID_COLS; ++j) {
						if (prev_room_tilemap[i][j] == 1) { //Draw a flat floor bellow wall/object
							CP_Settings_ImageWrapMode(CP_IMAGE_WRAP_CLAMP_EDGE);
							CP_Image_Draw(tile_list[prev_room_tilemap[i][j]], get_camera_x_pos((j * WALL_DIM) + WALL_DIM / 2) + CP_System_GetWindowWidth(), get_camera_y_pos((i * WALL_DIM) + WALL_DIM / 2), WALL_DIM, WALL_DIM, 255);
						}
					}
				}
				for (int i = 0; i < GRID_ROWS; i++) {
					for (int j = 0; j < GRID_COLS; ++j) {
						if (prev_room_tilemap[i][j] > 1 && room_wall_pos[i][j] == WALL_TILE) {
							CP_Image_Draw(tile_list[prev_room_tilemap[i][j]], get_camera_x_pos((j * WALL_DIM) + WALL_DIM / 2) + CP_System_GetWindowWidth(), get_camera_y_pos((i * WALL_DIM) + WALL_DIM / 2), WALL_DIM, WALL_DIM, 255);
						}
					}
				}
				break;
			case 3:
				world_offset.x = EaseOutExpo(CP_System_GetWindowWidth(), 0.0f, transition_timer / TRANSITION_TIMER);
				for (int i = 0; i < GRID_ROWS; i++) {
					for (int j = 0; j < GRID_COLS; ++j) {
						if (prev_room_tilemap[i][j] == 1) { //Draw a flat floor bellow wall/object
							CP_Settings_ImageWrapMode(CP_IMAGE_WRAP_CLAMP_EDGE);
							CP_Image_Draw(tile_list[prev_room_tilemap[i][j]], get_camera_x_pos((j * WALL_DIM) + WALL_DIM / 2) - CP_System_GetWindowWidth(), get_camera_y_pos((i * WALL_DIM) + WALL_DIM / 2), WALL_DIM, WALL_DIM, 255);
						}
					}
				}
				for (int i = 0; i < GRID_ROWS; i++) {
					for (int j = 0; j < GRID_COLS; ++j) {
						if (prev_room_tilemap[i][j] > 1 && room_wall_pos[i][j] == WALL_TILE) {
							CP_Image_Draw(tile_list[prev_room_tilemap[i][j]], get_camera_x_pos((j * WALL_DIM) + WALL_DIM / 2) - CP_System_GetWindowWidth(), get_camera_y_pos((i * WALL_DIM) + WALL_DIM / 2), WALL_DIM, WALL_DIM, 255);
						}
					}
				}
				break;
			}

		}

	}

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
		CP_Image_Draw(Pause_Menu, CP_System_GetWindowWidth() / 2, CP_System_GetWindowHeight() / 2, 550,590,255);
	}
	else {
		if (state == loading) {
			switch(transition_side){
			case 0: 
				world_offset.y = -CP_System_GetWindowHeight();
				transitioning = 1;
				break;
			case 1:
				world_offset.y = CP_System_GetWindowHeight();
				transitioning = 1;
				break;
			case 2:
				world_offset.x = -CP_System_GetWindowWidth();
				transitioning = 1;
				break;
			case 3:
				world_offset.x = CP_System_GetWindowWidth();
				transitioning = 1;
				break;
			}

		}
		else {
			//CP_Graphics_ClearBackground(CP_Color_Create(255, 255, 255, 255));
			draw_room_floor(world_offset);
			draw_room_wall(world_offset);
			for (int i = 0; i < ENTITY_CAP; ++i) {
				if (entities[i].type == entity_null) continue;
				switch (entities[i].type) {
				case entity_player: draw_player(&(entities[i].player), world_offset); break;
				case entity_mob: draw_mob(&(entities[i].mob), world_offset); break;
				case entity_boss: draw_boss(&(entities[i].boss), world_offset); break;
				case entity_projectile: draw_projectile(&(entities[i].projectile), world_offset); break;
				}
			}

			//ends the game
			if (state == room_active) {
			}
			else if (state == room_clear) {
				if (!tilemap_copied) {
					for (int i = 0; i < GRID_ROWS; ++i) {
						for (int j = 0; j < GRID_COLS; ++j) {
							prev_room_tilemap[i][j] = tile_map[i][j];
						}
					}
					tilemap_copied = 1;
				}
				draw_door(world_offset);

			}
		}
	}

	if (state != room_pause && state != room_failed)
		draw_hud(&(entities[PLAYER_IDX].player));
}

void draw_hud(Player* player) {
	for (int i = 1; i <= player->health; i++)
	{
		CP_Image_Draw(player_heart, 32 * i, 64, 32, 32, 255);
	}
}

float get_camera_x_pos(float x) {
	return x + world_offset.x;
}
float get_camera_y_pos(float y) {
	return y + world_offset.y;
}
void shake_camera(float scale, int override) {
	if (override) {
		shaking = 1;
		shaking_scale = scale;
	}else{
		if (shaking) {
			return;
		}
		else {
			shaking = 1;
			shaking_scale = scale;
		}
	}
}

