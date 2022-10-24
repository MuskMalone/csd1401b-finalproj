#pragma once
#include "utils.h"
// for player
typedef struct Player
{
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
	float diameter;
	int speed;
	float parryrad;
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