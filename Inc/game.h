#pragma once
#include "cprocessing.h"
#include "utils.h"
#include "gametypes.h"
#include "player.h"
#include "mobs.h"
#include "projectiles.h"
#include "boss.h"
#include "gamestates.h"
// put all structs and enums under ifndef

static void load_maps(void);

static void generate_door(void);

static void clear_all_entities(void);

static void generate_current_map(void);

void draw_door(Position offset);

void draw_room_wall(Position offset);

void draw_room_floor(Position offset);

