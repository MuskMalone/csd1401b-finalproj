#pragma once
#include "player.h"
#include "mobs.h"
#include "boss.h"
#include "projectiles.h"
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

void game_init(void);

void game_update(void);

void game_exit(void);

void add_entity(entity_type type, Entity e);

