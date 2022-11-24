#pragma once
#include "cprocessing.h"
#include "utils.h"
#include "gametypes.h"
#include "player.h"
#include "mobs.h"
#include "projectiles.h"
#include "boss.h"
#include "gamestates.h"
#include "camera.h"
// put all structs and enums under ifndef

static void load_maps(void);

static void clear_all_entities(void);

void generate_current_map(void);

void draw_room_failed_buttons(void);

void draw_pause_menu_btns(void);

void draw_door(float timer, CP_Image sprites2d[][DOOR_SPRITE_COUNT]);

void draw_room_wall(void);

void draw_room_floor(void);

void load_room_done(void);

