#pragma once
#include "cprocessing.h"
#include "utils.h"
#include "gametypes.h"
#include "player.h"
#include "mobs.h"
#include "projectiles.h"
#include "boss.h"
// put all structs and enums under ifndef

static void load_maps(void);

static void generate_door(void);

static void clear_all_entities(void);

static void generate_current_map(void);

static void draw_door(void);

static void draw_room_wall(void);

static void draw_room_floor(void);

void game_init(void);

void game_update(void);

void game_exit(void);

