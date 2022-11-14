#pragma once
#include "gametypes.h"
#define MELEE_PROJECTILE_SPRITE_COUNT 6
#define MELEE_MOB_SPRITE_COUNT 2
#define RANGE_MOB_SPRITE_COUNT 8
#define ROOM_TILE_TYPES 10
#define BOSS_SPRITE_COUNT 7

//HUD
extern CP_Image player_heart;

//player 
extern CP_Image Player_Barrier_Img;

//boss
extern CP_Image Cannon_Img;
extern CP_Image boss_def;
extern CP_Image Boss_Barrier_Img;
extern CP_Image Boss_Atk_Right[BOSS_SPRITE_COUNT];
extern CP_Image Boss_Atk_Left[BOSS_SPRITE_COUNT];

//projectiles
extern CP_Image player_projectile_sprites[MELEE_PROJECTILE_SPRITE_COUNT];
extern CP_Image enemy_projectile_sprites[MELEE_PROJECTILE_SPRITE_COUNT];
extern CP_Image Mobile_Proj_E;
extern CP_Image Mobile_Proj_P;

//mob
extern CP_Image melee_mob[MELEE_MOB_SPRITE_COUNT];
extern CP_Image explode_mob[MELEE_MOB_SPRITE_COUNT];
extern CP_Image range_mob[RANGE_MOB_SPRITE_COUNT];

extern CP_Image tile_list[ROOM_TILE_TYPES];

void init_sprites(void);
void draw_all(Entity entities[], int tile_map[GRID_ROWS][GRID_COLS], room_state state);
void draw_hud(Player* player);
float get_camera_x_pos(float x);
float get_camera_y_pos(float y);
void shake_camera(float scale, int override);