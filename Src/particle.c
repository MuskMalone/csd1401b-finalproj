#include "gametypes.h"
#include "particle.h"
#include "camera.h"

void init_particle(
	Particle* ptcl,
	float diameter,
	Position start_pos,
	CP_Vector dir,
	float distance,
	float max_timer,
	CP_Color color,
	float (*pos_lerp_func)(float start, float end, float value)
) {
	ptcl->running = 1;
	ptcl->diameter = diameter;
	ptcl->curr_pos = ptcl->start_pos = start_pos;
	ptcl->dir = dir;
	ptcl->distance = distance;
	ptcl->max_timer = max_timer;
	ptcl->timer = 0.0f;
	ptcl->color = color;
	ptcl->pos_lerp_func = pos_lerp_func;
}

void update_particle(Particle* ptcl) {
	ptcl->timer += CP_System_GetDt();
	if (ptcl->timer >= ptcl->max_timer) {
		ptcl->running = 0;
	}
	float min_x = ptcl->start_pos.x, min_y = ptcl->start_pos.y;
	float max_x = ptcl->start_pos.x + (ptcl->dir.x * ptcl->distance),
		max_y = ptcl->start_pos.y + (ptcl->dir.y * ptcl->distance);
	ptcl->curr_pos.x = ptcl->pos_lerp_func(min_x, max_x, ptcl->timer);
	ptcl->curr_pos.y = ptcl->pos_lerp_func(min_y, max_y, ptcl->timer);
}
void draw_particle(Particle* ptcl) {
	ptcl->color.a = (int)ptcl->pos_lerp_func(255, 0, ptcl->timer);
	CP_Settings_StrokeWeight(0.0f);
	CP_Settings_Fill(ptcl->color);
	CP_Graphics_DrawCircle(
		get_camera_x_pos(ptcl->curr_pos.x),
		get_camera_y_pos(ptcl->curr_pos.y),
		ptcl->diameter
	);
}