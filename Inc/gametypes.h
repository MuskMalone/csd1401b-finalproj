#pragma once
#include "utils.h"
#define WALL_DIM 50.0f
#define GRID_ROWS 25
#define GRID_COLS 30
// for player
typedef enum player_state { resting, moving, dashing } player_state;
typedef struct Player
{
	player_state state;
	int health;
	Position pos;
	float diameter;
	int speed;
	float parryrad;
} Player;
// for mobs
typedef enum attack_type { range, melee } attack_type;
typedef enum mob_id { mob1 } mob_id;
typedef struct Mob {
	mob_id id;
	attack_type type;
	Position pos;
} Mob;
// for projectiles
typedef struct Projectile {
	Position pos;
	float radius;
} Projectile;
// for boss
typedef struct Boss {
	Position pos;
	float health;
}Boss;
// overarching entity struct
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
	union {
		Player player;
		Mob mob;
		Boss boss;
		Projectile projectile;
	};
} Entity;