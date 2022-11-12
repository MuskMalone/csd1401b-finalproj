#pragma once
#include "utils.h"
#define WALL_DIM 3.5f * ((float)CP_System_GetDisplayHeight()/100.0f)
#define ENTITY_CAP 100
#define PLAYER_IDX 0
#define GRID_ROWS 25
#define GRID_COLS 30

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
	Position pos;
	Position Future_Pos;
	CP_Vector Direction;
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