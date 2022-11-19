#include "gametypes.h"
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