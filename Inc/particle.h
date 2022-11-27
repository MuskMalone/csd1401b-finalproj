/***
 *                         _    _        _           _
 *                        | |  (_)      | |         | |
 *     _ __    __ _  _ __ | |_  _   ___ | |  ___    | |__
 *    | '_ \  / _` || '__|| __|| | / __|| | / _ \   | '_ \
 *    | |_) || (_| || |   | |_ | || (__ | ||  __/ _ | | | |
 *    | .__/  \__,_||_|    \__||_| \___||_| \___|(_)|_| |_|
 *    | |
 *    |_|
 */
 /*
 @file particle.h
 @author Vanessa Chua
 @course CSD1401
 @section B
 @brief  code for particle effects
 */
 /*______________________________________________________________________*/
#include "gametypes.h"

 //declaration of functions
void init_particle(
	Particle* ptcl,
	float diameter,
	Position start_pos,
	CP_Vector dir,
	float distance,
	float max_timer,
	CP_Color color,
	float (*pos_lerp_func)(float start, float end, float value)
);
void update_particle(Particle* ptcl);
void draw_particle(Particle* ptcl);