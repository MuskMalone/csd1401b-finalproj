/***
 *                       _         _
 *                      | |       | |
 *     _ __ ___    ___  | |__     | |__
 *    | '_ ` _ \  / _ \ | '_ \    | '_ \
 *    | | | | | || (_) || |_) | _ | | | |
 *    |_| |_| |_| \___/ |_.__/ (_)|_| |_|
 *
 *
 */
 /*
 @file mob.h
 @author Vanessa Chua
 @course CSD1401
 @section B
 @brief  code for mob entity
 */
 /*______________________________________________________________________*/
#pragma once
#include "utils.h"
#include "cprocessing.h"
#include "gametypes.h"
#define MOB_EXP_TIMER 1.5f //timer for exploding
#define MOB_MELEE_TIMER 1.0f // 2 seconds
#define MOB_RANGED_TIMER CP_Random_RangeFloat(1.0f, 3.0f); //timer for mob range
#define MOB_HEALTH 1 //health for mobs

 //declaration of functions
entity_struct init_mob(void);
void update_mob(int mob_idx, int player_idx, Entity entities[], int wall_pos[GRID_ROWS][GRID_COLS]);
void expansion_mob_size(Entity entities[], int mob_idx);
void damage_mob(Mob* mob);
void draw_mob(Mob* mob);
