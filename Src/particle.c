/***
 *                         _    _        _
 *                        | |  (_)      | |
 *     _ __    __ _  _ __ | |_  _   ___ | |  ___      ___
 *    | '_ \  / _` || '__|| __|| | / __|| | / _ \    / __|
 *    | |_) || (_| || |   | |_ | || (__ | ||  __/ _ | (__
 *    | .__/  \__,_||_|    \__||_| \___||_| \___|(_) \___|
 *    | |
 *    |_|
 */

 /*
 @file particle.c
 @author Vanessa Chua
 @course CSD1401
 @section B
 @brief  code for particle effects
 */
 /*______________________________________________________________________*/
#include "gametypes.h"
#include "particle.h"
#include "camera.h"

//start function
void init_particle(
	Particle* ptcl,
	float diameter,
	Position start_pos,
	CP_Vector dir,
	float distance,
	float max_timer,
	CP_Color color,
	float (*pos_lerp_func)(float start, float end, float value)
) { //inits a particle struct's members given a pointer to a particle
	ptcl->running = 1;
	ptcl->diameter = diameter;
	ptcl->curr_pos = ptcl->start_pos = start_pos; //current position of particle
	ptcl->dir = dir; //direction
	ptcl->distance = distance;
	ptcl->max_timer = max_timer;
	ptcl->timer = 0.0f;
	ptcl->color = color; //colour of particle
	ptcl->pos_lerp_func = pos_lerp_func;
}

void update_particle(Particle* ptcl) 
{
	ptcl->timer += CP_System_GetDt(); //timer
	if (ptcl->timer >= ptcl->max_timer) {
		ptcl->running = 0; //running particle false
	}
	float min_x = ptcl->start_pos.x, min_y = ptcl->start_pos.y; //calculation of min x axis
	float max_x = ptcl->start_pos.x + (ptcl->dir.x * ptcl->distance), //max x axis
		max_y = ptcl->start_pos.y + (ptcl->dir.y * ptcl->distance); //max y axis
	ptcl->curr_pos.x = ptcl->pos_lerp_func(min_x, max_x, ptcl->timer); //slowly lerp
	ptcl->curr_pos.y = ptcl->pos_lerp_func(min_y, max_y, ptcl->timer);
}

//drawing of particles
void draw_particle(Particle* ptcl)
{
	ptcl->color.a = (int)ptcl->pos_lerp_func(255, 0, ptcl->timer);
	CP_Settings_StrokeWeight(0.0f);
	CP_Settings_Fill(ptcl->color);
	CP_Graphics_DrawCircle(
		get_camera_x_pos(ptcl->curr_pos.x),
		get_camera_y_pos(ptcl->curr_pos.y),
		ptcl->diameter
	);
}