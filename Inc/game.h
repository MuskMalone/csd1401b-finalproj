/*
@file game.h
@author Shannon Teo
@course CSD1401
@section B
@brief where the main game runs
*/
/*______________________________________________________________________*/
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

/*
This function loads in the different rooms for the levels from a text file using the
function atoi. Its reads a series of 1s and 0s and set the value in the array to 1(wall)
or 0(floor).
*/
static void load_maps(void);

/*
This function clears the map all entities except player.
*/
static void clear_all_entities(void);

/*
This function generates the current map and uses the array from room_wall_pos to
determine which tile is drawn.
*/
void generate_current_map(void);

/*
This functions draws the main menu and exit buttons on the game over screen.
*/
void draw_room_failed_buttons(void);

/*
This functions draws the main menu and exit buttons on the pause screen.
*/
void draw_pause_menu_btns(void);

/*
This functions draws the door animation.
*/
void draw_door(float timer, CP_Image sprites2d[][DOOR_SPRITE_COUNT]);

/*
This function draws the obstacles in the room.
*/
void draw_room_wall(void);

/*
This function draws the floor tiles.
*/
void draw_room_floor(void);

