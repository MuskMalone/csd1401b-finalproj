/*
@file camera.c
@author Tan Cheng Hian
@course CSD1401
@section B
@brief code for game camera
*/
/*______________________________________________________________________*/
#include "player.h"
#include "boss.h"
#include "mobs.h"
#include "projectiles.h"
#include "camera.h"
#include "game.h"
#include "utils.h"
#include "easing.h"
#include "gametypes.h"
#include "particle.h"
#include <stdio.h>
#include <stdlib.h>
#define TRANSITION_TIMER 1.25f
#define PLAYER_HEART_SPEED NORMAL_SPEED * 4.0f

static is_loaded = 0; // doesnt load the sprites more than once
CP_Image game_button_sprites[BUTTON_SPRITE_COUNT];

static Position player_heart_pos;
static float player_heart_speed;

CP_Image tutorial[TUTORIAL_SPRITE_COUNT];

CP_Image player_heart[PLAYER_HEALTH_SPRITE_COUNT];
CP_Image Pause_Menu;
CP_Image GameOverMenu; 
CP_Image player_deflect_arrow;
CP_Image Player_Barrier_Img;
CP_Image player_front[PLAYER_SPRITE_COUNT];
CP_Image player_frontdiagleft[PLAYER_SPRITE_COUNT];
CP_Image player_frontdiagright[PLAYER_SPRITE_COUNT];
CP_Image player_back[PLAYER_SPRITE_COUNT];
CP_Image player_backdiagleft[PLAYER_SPRITE_COUNT];
CP_Image player_backdiagright[PLAYER_SPRITE_COUNT];
CP_Image player_left[PLAYER_SPRITE_COUNT];
CP_Image player_right[PLAYER_SPRITE_COUNT];
CP_Image BackToMenuBut;
CP_Image Cannon_Img;
CP_Image boss_def;
CP_Image Boss_Stun[2];
CP_Image Boss_Barrier_Img;
CP_Image Boss_Atk_Right[BOSS_SPRITE_COUNT];
CP_Image Boss_Atk_Left[BOSS_SPRITE_COUNT];

CP_Image Mobile_Proj_E;
CP_Image Mobile_Proj_P;
CP_Image player_projectile_sprites[EXPLOSION_PROJECTILE_SPRITE_COUNT];
CP_Image enemy_projectile_sprites[EXPLOSION_PROJECTILE_SPRITE_COUNT];
CP_Image sword_right[WEAPON_PROJECTILE_SPRITE_COUNT];
CP_Image sword_left[WEAPON_PROJECTILE_SPRITE_COUNT];

CP_Image melee_mob_left[MELEE_MOB_SPRITE_COUNT];
CP_Image explode_mob_left[MELEE_MOB_SPRITE_COUNT];
CP_Image melee_mob_right[MELEE_MOB_SPRITE_COUNT];
CP_Image explode_mob_right[MELEE_MOB_SPRITE_COUNT];
CP_Image range_mob[RANGE_MOB_SPRITE_COUNT];

CP_Image door_sprites[DOOR_COUNT][DOOR_SPRITE_COUNT];
Position world_offset;

Particle particle_array[PARTICLE_CAP];
unsigned int particle_count = 0;

int hue_flashing = 0;
float hue_max_flashing_timer = 0.0f;
int hue_min_alpha, hue_max_alpha;
float hue_flashing_timer = 0.0f;
CP_Color hue_color;

int shaking = 0;
float shaking_scale = 1.0f;

int tilemap_copied = 0;
int transitioning = 0;
int transition_side = -1;
float transition_timer = 0.0f;
int prev_room_tilemap[GRID_ROWS][GRID_COLS];

float door_timer = 0.0f;

void init_sprites(void) {
	player_heart_pos = (Position){
		0.0f,
		0.0f
	};
	world_offset = (Position){ 0.0f, 0.0f };
	game_button_sprites[START_BUTTON] = CP_Image_Load("./Assets/STARTGAME.png");
	game_button_sprites[EXIT_BUTTON] = CP_Image_Load("./Assets/EXIT.png");
	game_button_sprites[TUTORIAL_BUTTON] = CP_Image_Load("./Assets/HOWTOPLAY.png");
	game_button_sprites[MENU_BUTTON] = CP_Image_Load("./Assets/BACKTOMENU.png");
	game_button_sprites[RESUME_BUTTON] = CP_Image_Load("./Assets/RESUME.png");
	if (is_loaded) return;
	GameOverMenu = CP_Image_Load("./Assets/gameover.png");
	Boss_Barrier_Img = CP_Image_Load("./Assets/Tiles/Boss/Boss_Barrier.png");
	Cannon_Img = CP_Image_Load("./Assets/Tiles/Boss/Cannon.png");
	boss_def = CP_Image_Load("./Assets/Tiles/Boss/Boss_Base.png");
	Pause_Menu = CP_Image_Load("./Assets/PauseMenu.png");
	BackToMenuBut = CP_Image_Load("./Assets/BACKTOMENU.png");

	tutorial[0] = CP_Image_Load("./Assets/Tiles/Tutorial/Tut_1_inGame.png");
	tutorial[1] = CP_Image_Load("./Assets/Tiles/Tutorial/Tut_2_inGame.png");
	tutorial[2] = CP_Image_Load("./Assets/Tiles/Tutorial/Tut_3_inGame.png");
	tutorial[3] = CP_Image_Load("./Assets/Tiles/Tutorial/Tut_4_inGame.png");
	tutorial[4] = CP_Image_Load("./Assets/Tiles/Tutorial/Tut_5_inGame.png");

	player_heart[0] = CP_Image_Load("./Assets/Tiles/Player/heart7.png");
	player_heart[1] = CP_Image_Load("./Assets/Tiles/Player/heart6.png");
	player_heart[2] = CP_Image_Load("./Assets/Tiles/Player/heart5.png");
	player_heart[3] = CP_Image_Load("./Assets/Tiles/Player/heart4.png");
	player_heart[4] = CP_Image_Load("./Assets/Tiles/Player/heart3.png");
	player_heart[5] = CP_Image_Load("./Assets/Tiles/Player/heart2.png");
	player_heart[6] = CP_Image_Load("./Assets/Tiles/Player/heart1.png");
	player_heart[7] = CP_Image_Load("./Assets/Tiles/Player/heart0.png");

	Player_Barrier_Img = CP_Image_Load("./Assets/Tiles/Player/Player_Barrier4.png");
	player_deflect_arrow = CP_Image_Load("./Assets/Tiles/Player/up.png");
	player_front[0] = CP_Image_Load("./Assets/Tiles/Player/front1.png");
	player_front[1] = CP_Image_Load("./Assets/Tiles/Player/front0.png");
	player_front[2] = CP_Image_Load("./Assets/Tiles/Player/front2.png");
	player_front[3] = CP_Image_Load("./Assets/Tiles/Player/front0.png");
	player_front[4] = CP_Image_Load("./Assets/Tiles/Player/front3.png");
	player_frontdiagright[0] = CP_Image_Load("./Assets/Tiles/Player/45front1.png");
	player_frontdiagright[1] = CP_Image_Load("./Assets/Tiles/Player/45front0.png");
	player_frontdiagright[2] = CP_Image_Load("./Assets/Tiles/Player/45front2.png");
	player_frontdiagright[3] = CP_Image_Load("./Assets/Tiles/Player/45front0.png");
	player_frontdiagright[4] = CP_Image_Load("./Assets/Tiles/Player/45front3.png");
	player_frontdiagleft[0] = CP_Image_Load("./Assets/Tiles/Player/45front21.png");
	player_frontdiagleft[1] = CP_Image_Load("./Assets/Tiles/Player/45front20.png");
	player_frontdiagleft[2] = CP_Image_Load("./Assets/Tiles/Player/45front22.png");
	player_frontdiagleft[3] = CP_Image_Load("./Assets/Tiles/Player/45front20.png");
	player_frontdiagleft[4] = CP_Image_Load("./Assets/Tiles/Player/45front23.png");
	player_back[0] = CP_Image_Load("./Assets/Tiles/Player/back1.png");
	player_back[1] = CP_Image_Load("./Assets/Tiles/Player/back0.png");
	player_back[2] = CP_Image_Load("./Assets/Tiles/Player/back2.png");
	player_back[3] = CP_Image_Load("./Assets/Tiles/Player/back0.png");
	player_back[4] = CP_Image_Load("./Assets/Tiles/Player/back3.png");
	player_backdiagright[0] = CP_Image_Load("./Assets/Tiles/Player/45back21.png");
	player_backdiagright[1] = CP_Image_Load("./Assets/Tiles/Player/45back20.png");
	player_backdiagright[2] = CP_Image_Load("./Assets/Tiles/Player/45back22.png");
	player_backdiagright[3] = CP_Image_Load("./Assets/Tiles/Player/45back20.png");
	player_backdiagright[4] = CP_Image_Load("./Assets/Tiles/Player/45back23.png");
	player_backdiagleft[0] = CP_Image_Load("./Assets/Tiles/Player/45back1.png");
	player_backdiagleft[1] = CP_Image_Load("./Assets/Tiles/Player/45back0.png");
	player_backdiagleft[2] = CP_Image_Load("./Assets/Tiles/Player/45back2.png");
	player_backdiagleft[3] = CP_Image_Load("./Assets/Tiles/Player/45back0.png");
	player_backdiagleft[4] = CP_Image_Load("./Assets/Tiles/Player/45back3.png");
	player_left[0] = CP_Image_Load("./Assets/Tiles/Player/side1.png");
	player_left[1] = CP_Image_Load("./Assets/Tiles/Player/side0.png");
	player_left[2] = CP_Image_Load("./Assets/Tiles/Player/side2.png");
	player_left[3] = CP_Image_Load("./Assets/Tiles/Player/side0.png");
	player_left[4] = CP_Image_Load("./Assets/Tiles/Player/side3.png");
	player_right[0] = CP_Image_Load("./Assets/Tiles/Player/side21.png");
	player_right[1] = CP_Image_Load("./Assets/Tiles/Player/side20.png");
	player_right[2] = CP_Image_Load("./Assets/Tiles/Player/side22.png");
	player_right[3] = CP_Image_Load("./Assets/Tiles/Player/side20.png");
	player_right[4] = CP_Image_Load("./Assets/Tiles/Player/side23.png");

	
	Boss_Stun[0] = CP_Image_Load("./Assets/Tiles/Boss/BossStunLeft.png");
	Boss_Stun[1] = CP_Image_Load("./Assets/Tiles/Boss/BossStunRight.png");
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
	player_projectile_sprites[6] = CP_Image_Load("./Assets/Tiles/Projectiles/Static_Proj_P7.png");

	enemy_projectile_sprites[0] = CP_Image_Load("./Assets/Tiles/Projectiles/Static_Proj_E1.png");
	enemy_projectile_sprites[1] = CP_Image_Load("./Assets/Tiles/Projectiles/Static_Proj_E2.png");
	enemy_projectile_sprites[2] = CP_Image_Load("./Assets/Tiles/Projectiles/Static_Proj_E3.png");
	enemy_projectile_sprites[3] = CP_Image_Load("./Assets/Tiles/Projectiles/Static_Proj_E4.png");
	enemy_projectile_sprites[4] = CP_Image_Load("./Assets/Tiles/Projectiles/Static_Proj_E5.png");
	enemy_projectile_sprites[5] = CP_Image_Load("./Assets/Tiles/Projectiles/Static_Proj_E6.png");
	enemy_projectile_sprites[6] = CP_Image_Load("./Assets/Tiles/Projectiles/Static_Proj_E7.png");

	sword_left[0] = CP_Image_Load("./Assets/Tiles/Projectiles/sword0.png");
	sword_left[1] = CP_Image_Load("./Assets/Tiles/Projectiles/sword1.png");
	sword_left[2] = CP_Image_Load("./Assets/Tiles/Projectiles/sword2.png");
	sword_left[3] = CP_Image_Load("./Assets/Tiles/Projectiles/sword3.png");

	melee_mob_left[0] = CP_Image_Load("./Assets/Tiles/Mobs/Melee/skeletonleft0.png");
	melee_mob_left[1] = CP_Image_Load("./Assets/Tiles/Mobs/Melee/skeletonleft1.png");
	melee_mob_right[0] = CP_Image_Load("./Assets/Tiles/Mobs/Melee/skeletonright0.png");
	melee_mob_right[1] = CP_Image_Load("./Assets/Tiles/Mobs/Melee/skeletonright1.png");

	explode_mob_left[0] = CP_Image_Load("./Assets/Tiles/Mobs/Explode/boomberleft0.png");
	explode_mob_left[1] = CP_Image_Load("./Assets/Tiles/Mobs/Explode/boomberleft1.png");
	explode_mob_right[0] = CP_Image_Load("./Assets/Tiles/Mobs/Explode/boomberright0.png");
	explode_mob_right[1] = CP_Image_Load("./Assets/Tiles/Mobs/Explode/boomberright1.png");

	range_mob[7] = CP_Image_Load("./Assets/Tiles/Mobs/Range/flowerR.png");
	range_mob[6] = CP_Image_Load("./Assets/Tiles/Mobs/Range/flowerRB45.png");
	range_mob[5] = CP_Image_Load("./Assets/Tiles/Mobs/Range/flowerB.png");
	range_mob[4] = CP_Image_Load("./Assets/Tiles/Mobs/Range/flowerLB45.png");
	range_mob[3] = CP_Image_Load("./Assets/Tiles/Mobs/Range/flowerL.png");
	range_mob[2] = CP_Image_Load("./Assets/Tiles/Mobs/Range/flowerLF45.png");
	range_mob[1] = CP_Image_Load("./Assets/Tiles/Mobs/Range/flowerF.png");
	range_mob[0] = CP_Image_Load("./Assets/Tiles/Mobs/Range/flowerRF45.png");

	//room tiles
	
	tile_list[1] = CP_Image_Load("./Assets/Tiles/test.png"); // rock floor
	tile_list[2] = CP_Image_Load("./Assets/Tiles/walltest1.png"); // grave
	tile_list[3] = CP_Image_Load("./Assets/Tiles/Wall2.png"); // anvil
	tile_list[4] = CP_Image_Load("./Assets/Tiles/walltest2.png"); // barrel
	tile_list[5] = CP_Image_Load("./Assets/Tiles/TopWall.png");
	tile_list[6] = CP_Image_Load("./Assets/Tiles/BottomWall.png");
	tile_list[7] = CP_Image_Load("./Assets/Tiles/RightWall.png");
	tile_list[8] = CP_Image_Load("./Assets/Tiles/LeftWall.png");

	door_sprites[0][0] = CP_Image_Load("./Assets/door/topdoor/doorC.png");
	door_sprites[0][1] = CP_Image_Load("./Assets/door/topdoor/doorO22_5.png");
	door_sprites[0][2] = CP_Image_Load("./Assets/door/topdoor/doorO45.png");
	door_sprites[0][3] = CP_Image_Load("./Assets/door/topdoor/doorO67_5.png");
	door_sprites[0][4] = CP_Image_Load("./Assets/door/topdoor/doorO.png");

	door_sprites[1][0] = CP_Image_Load("./Assets/door/botdoor/doorC.png");
	door_sprites[1][1] = CP_Image_Load("./Assets/door/botdoor/doorO22_5.png");
	door_sprites[1][2] = CP_Image_Load("./Assets/door/botdoor/doorO45.png");
	door_sprites[1][3] = CP_Image_Load("./Assets/door/botdoor/doorO67_5.png");
	door_sprites[1][4] = CP_Image_Load("./Assets/door/botdoor/doorO.png");

	door_sprites[2][0] = CP_Image_Load("./Assets/door/leftdoor/doorC.png");
	door_sprites[2][1] = CP_Image_Load("./Assets/door/leftdoor/doorO22_5.png");
	door_sprites[2][2] = CP_Image_Load("./Assets/door/leftdoor/doorO45.png");
	door_sprites[2][3] = CP_Image_Load("./Assets/door/leftdoor/doorO67_5.png");
	door_sprites[2][4] = CP_Image_Load("./Assets/door/leftdoor/doorO.png");

	door_sprites[3][0] = CP_Image_Load("./Assets/door/rightdoor/doorC.png");
	door_sprites[3][1] = CP_Image_Load("./Assets/door/rightdoor/doorO22_5.png");
	door_sprites[3][2] = CP_Image_Load("./Assets/door/rightdoor/doorO45.png");
	door_sprites[3][3] = CP_Image_Load("./Assets/door/rightdoor/doorO67_5.png");
	door_sprites[3][4] = CP_Image_Load("./Assets/door/rightdoor/doorO.png");

	is_loaded = 1;
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
		if (transition_timer >= TRANSITION_TIMER){
			transitioning = 0;
			world_offset.y = 0.0f; world_offset.x = 0.0f;
			transition_timer = 0.0f;
		}
		else {
			switch (transition_side) {
			case 0:
				world_offset.y = EaseOutExpo((float) - CP_System_GetWindowHeight(), 0.0f, transition_timer / TRANSITION_TIMER);
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
				world_offset.y = EaseOutExpo((float)CP_System_GetWindowHeight(), 0.0f, transition_timer / TRANSITION_TIMER);
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
				world_offset.x = EaseOutExpo((float) - CP_System_GetWindowWidth(), 0.0f, transition_timer / TRANSITION_TIMER);
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
				world_offset.x = EaseOutExpo((float)CP_System_GetWindowWidth(), 0.0f, transition_timer / TRANSITION_TIMER);
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
		//CP_Graphics_ClearBackground(CP_Color_Create(0, 0, 0, 255));

		
		
		CP_Image_Draw(GameOverMenu, (float) CP_System_GetWindowWidth() / 2.0f, (float)CP_System_GetWindowHeight() / 2.0f, (float)CP_System_GetWindowWidth(), (float)CP_System_GetWindowHeight(), 255);
		draw_room_failed_buttons();
		//creditPosY--;
		//CP_Settings_Fill(CP_Color_Create(255, 255, 255, 255));
		//CP_Settings_TextSize(50.0f);
		//char buffer[500] = { 0 };
		//sprintf_s(buffer, _countof(buffer), "R to main menu, ESC to quit");
		//CP_Font_DrawText(buffer, 300, 800);
	}
	else if (state == room_pause) {

		//draw the stuff here
		CP_Image_Draw(Pause_Menu, (float)CP_System_GetWindowWidth() / 2.0f, (float)CP_System_GetWindowHeight() / 2.0f, 550.0f,590.0f,255);
		draw_pause_menu_btns();
		//CP_Image_Draw(BackToMenuBut, CP_System_GetWindowWidth() / 2, CP_System_GetWindowHeight() *3/5, 440, 90, 255);
	}
	else {
		if (state == loading) {
			switch(transition_side){
			case 0: 
				world_offset.y = (float) - CP_System_GetWindowHeight();
				transitioning = 1;
				break;
			case 1:
				world_offset.y = (float)CP_System_GetWindowHeight();
				transitioning = 1;
				break;
			case 2:
				world_offset.x = (float) - CP_System_GetWindowWidth();
				transitioning = 1;
				break;
			case 3:
				world_offset.x = (float)CP_System_GetWindowWidth();
				transitioning = 1;
				break;
			}

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
				door_timer = 0.0f;
			}
			else if (state == room_clear) {
				if (door_timer <= DOOR_MAX_TIMER) {
					door_timer += CP_System_GetDt();
				}
				if (!tilemap_copied) {
					for (int i = 0; i < GRID_ROWS; ++i) {
						for (int j = 0; j < GRID_COLS; ++j) {
							prev_room_tilemap[i][j] = tile_map[i][j];
						}
					}
					tilemap_copied = 1;
				}
				draw_door(door_timer, door_sprites);

			}
		}
	}

	if (state != room_pause && state != room_failed)
		draw_hud(&(entities[PLAYER_IDX].player));
	for (int i = 0; i < PARTICLE_CAP; ++i) {
		if (particle_array[i].running) {
			update_particle(&(particle_array[i]));
			draw_particle(&(particle_array[i]));
			//CP_Graphics_DrawCircle(100, 100, 20.0f);
		}
	}
	if (hue_flashing) {
		hue_flashing_timer += CP_System_GetDt();
		if (hue_flashing_timer >= hue_max_flashing_timer) {
			hue_flashing = 0;
		}
		float alpha = QuickSpikeEaseOut(
			(float)hue_min_alpha, 
			(float)hue_max_alpha, 
			(float)hue_flashing_timer / hue_max_flashing_timer
		);
		if (alpha >= 0.0f) {
			hue_color.a = (int)alpha;
			CP_Settings_Fill(hue_color);
			CP_Graphics_DrawRect(0.0f, 0.0f, (float)CP_System_GetWindowWidth(), (float)CP_System_GetWindowHeight());
		}
	}

}

void insert_to_particle_array(
	float diameter,
	Position start_pos,
	CP_Vector dir,
	float distance,
	float max_timer,
	CP_Color color,
	float (*pos_lerp_func)(float start, float end, float value)
) {
	init_particle(
		&(particle_array[particle_count % PARTICLE_CAP]),
		diameter,
		start_pos,
		dir,
		distance,
		max_timer,
		color,
		pos_lerp_func
	);
	particle_count++;
}
void create_particle_burst(
	float timer,
	float (*pos_lerp_func)(float start, float end, float value),
	CP_Color color,
	Position pos,
	float distance,
	float start_size,
	float end_size,
	float start_deg,
	float end_deg,
	unsigned int density) {
	for (unsigned int i = 0; i < density; ++i) {
		insert_to_particle_array(
			CP_Random_RangeFloat(start_size, end_size),
			pos,
			angleToVector(CP_Random_RangeFloat(start_deg, end_deg)),
			distance,
			timer,
			color,
			pos_lerp_func
		);
	}
}
void draw_hud(Player* player) {
	//for (int i = 1; i <= player->health; i++)
	//{
	//	CP_Image_Draw(player_heart, 32 * i, 64, 32, 32, 255);
	//}
	CP_Image_Draw(player_heart[player->health - 1], player_heart_pos.x, player_heart_pos.y, WALL_DIM * .5f, WALL_DIM * .5f, 255);
	CP_Vector player_dir = getVectorBetweenPositions(player_heart_pos, player->pos);
	float distance = CP_Math_Distance(player_heart_pos.x, player_heart_pos.y, player->pos.x, player->pos.y),
		min_distance = (MAX_PARRYRADIUS + WALL_DIM * .5f);
	player_heart_speed = (PLAYER_HEART_SPEED * (distance / min_distance)) - PLAYER_HEART_SPEED;
	moveEntity(&player_heart_pos, player_dir.x * player_heart_speed, player_dir.y * player_heart_speed);
	
}
void flash_hue(CP_Color color, float time, int min_alpha, int max_alpha) {
	hue_color = color;
	hue_min_alpha = min_alpha;
	hue_max_alpha = max_alpha;
	hue_color.a = min_alpha;
	hue_flashing = 1;
	hue_max_flashing_timer = time;
	hue_flashing_timer = 0.0f;
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


