#pragma once
#include "utils.h"
#define WALL_DIM 3.5f * ((float)CP_System_GetDisplayHeight()/100.0f)
#define ENTITY_CAP 100
#define PLAYER_IDX 0
#define GRID_ROWS 25
#define GRID_COLS 30
#define PARTICLE_CAP 5000

typedef struct Game_Buton {
	int mouse_on_button;
	float timer;
} Game_Button;

typedef struct Particle {
	int running;
	float diameter;
	Position curr_pos; 
	Position start_pos;
	CP_Vector dir;
	float distance;
	float max_timer;
	float timer;
	CP_Color color;
	float (*pos_lerp_func)(float start, float end, float value);
} Particle;

enum tile_type { FLOOR_TILE, WALL_TILE, MOB_TILE, BOSS_TILE };
typedef enum room_state { room_pause, room_active, room_clear, loading, room_failed } room_state;
// for player

typedef enum player_state { resting, moving, dashing, dead, holding } player_state;
typedef struct Player
{
	player_state state;
	int horizontal_dir; int vertical_dir;
	int health;
	Position pos;
	float diameter;
	int speed;
	float parryrad;
} Player;

// for mobs
typedef enum attack_type { range, melee, explode } attack_type;
typedef enum melee_attack_state { mob_resting, mob_moving, mob_attacking, mob_attacked } melee_attack_state;
typedef struct Mob {
	CP_Vector direction;
	attack_type type;
	Position pos;
	float diameter;
	float timer;
	int health;
	union {
		BOOL is_exploding;
		melee_attack_state melee_state;
	};

} Mob;
// for projectiles
typedef struct Projectile {
	int frame_idx;
	Position pos;
	Position Future_Pos;
	CP_Vector Direction;
	CP_Image *Proj_Img;
	float radius;
	int speed;
	char source;
	char type;
	char toRebound_NextFrame;
	union {
		float LifeSpan;
		int rebound_count;
	};
	
} Projectile;
// for boss
typedef struct Boss {
	Position pos;
	int health;
	int atk_cd;
	int speed;
	int parry_ammo;
	int parry_cd;
	float diameter;
	float parryrad;
	float Parry_BaseWeight;
}Boss;
// overarching entity struct
typedef union entity_struct {
	Player player;
	Mob mob;
	Boss boss;
	Projectile projectile;
} entity_struct;
typedef enum entity_type {
	entity_null,
	entity_player,
	entity_mob,
	entity_boss,
	entity_projectile
} entity_type;
typedef struct Entity
{
	entity_type type;
	entity_struct;
} Entity;

int insert_to_entity_array(entity_type type, Entity entities[], entity_struct(*init_func)());