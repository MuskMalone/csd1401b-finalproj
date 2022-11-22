#include "projectiles.h"
#include "camera.h"
#include "easing.h"
#include <stdlib.h>
#include <stdio.h>
CP_Image Mobile_Proj_E;
CP_Image Mobile_Proj_P;
CP_Image player_projectile_sprites[EXPLOSION_PROJECTILE_SPRITE_COUNT];
CP_Image enemy_projectile_sprites[EXPLOSION_PROJECTILE_SPRITE_COUNT];
CP_Image sword_left[WEAPON_PROJECTILE_SPRITE_COUNT];

entity_struct init_projectile(void) {
	Projectile Proj;
	Proj.speed = 500;
	Proj.frame_idx = 0;
	return (entity_struct) {.projectile = Proj};
}
void set_projectile_values(Projectile* Proj, char Source, char type, int radius, Position Start_Pos, CP_Vector Direction_Vector) {
	Proj->type = type;			// 2 Projectile types. Static (For melee or exploding enemy) and Mobile (Ranged mobs)
	Proj->pos.x = Start_Pos.x; 
	Proj->pos.y = Start_Pos.y;
	Proj->Direction = Direction_Vector;
	Proj->radius = radius;	   // default size 20
	
	Proj->source = Source;	  // The owner of the projectile. Prevents the projectile from attacking its owner
	Proj->Future_Pos = Proj->pos;
	Proj->toRebound_NextFrame = PROJ_NOT_REBOUNDING;
	if (type == PROJ_TYPE_STATIC || type == PROJ_TYPE_WEAPON)
		Proj->LifeSpan = 0.0f;//PROJ_MELEE_LIFESPAN;
	else if (type == PROJ_TYPE_MOBILE)
		Proj->rebound_count = 0;
}
void projectile_wall_bounce_particles(Projectile* proj, char source) {
	float start_angle, end_angle;
	if (source == PROJ_VERTICAL_WALL) {
		if (proj->Direction.x > 0.0f) {
			start_angle = 270.0f, end_angle = 410.0f;
		}
		else {
			start_angle = 90.0f, end_angle = 270.0f;
		}
	}
	else if (source == PROJ_HORIZONTAL_WALL) {
		if (proj->Direction.y > 0.0f) {
			start_angle = 0.0f, end_angle = 180.0f;
		}
		else {
			start_angle = 180.0f, end_angle = 360.0f;
		}
		

	}
	create_particle_burst(
		1.0f,
		EaseOutExpo,
		CP_Color_Create(255, 255, 255, 255),
		proj->pos,
		WALL_DIM,
		2.0f,
		8.0f,
		start_angle,
		end_angle,
		10
	);
}
void update_projectile(int index, Entity entities[], int wall_pos[GRID_ROWS][GRID_COLS]) {
	Projectile* proj = &(entities[index].projectile);
	int to_Rebound = 0;
	
	if (proj->type == PROJ_TYPE_MOBILE) {
		if (proj->toRebound_NextFrame != PROJ_NOT_REBOUNDING) {
			proj->pos = proj->Future_Pos;
			deflectprojectiles(proj->toRebound_NextFrame, index, entities);
			if (proj->rebound_count++ >= MAX_REBOUND_COUNT)
				entities[index].type = entity_null;
		}
		else {
			moveEntity(&(proj->pos), proj->Direction.x * proj->speed, proj->Direction.y * proj->speed);
		}
		if(!Entities_Collision_Check(proj, index, entities)){
			Position Left_Edge_Wall = (Position){ -WALL_DIM , 0 };
			Position Right_Edge_Wall = (Position){ CP_System_GetWindowWidth() , 0 };
			Position Top_Edge_Wall = (Position){ 0,-WALL_DIM };
			Position Bottom_Edge_Wall = (Position){ 0 , CP_System_GetWindowHeight() };
			proj->toRebound_NextFrame = PROJ_NOT_REBOUNDING;
		


			for (int i = 0; i < GRID_ROWS; ++i) {
				for (int j = 0; j < GRID_COLS; ++j) {
					if (wall_pos[i][j]==1) {
						if(!to_Rebound)
						if (Wall_Edge_Check(proj, (Position) { WALL_DIM* (float)j, WALL_DIM* (float)i }, WALL_DIM, WALL_DIM)) {
							to_Rebound = 1;
							break;
						}
					}
				}
			}
			if (!to_Rebound) {
				if (!Wall_Edge_Check(proj, Left_Edge_Wall, WALL_DIM, CP_System_GetWindowHeight())) {
					if (!Wall_Edge_Check(proj, Right_Edge_Wall, WALL_DIM, CP_System_GetWindowHeight())) {
						if (!Wall_Edge_Check(proj, Top_Edge_Wall, CP_System_GetWindowWidth(), WALL_DIM)) {
							Wall_Edge_Check(proj, Bottom_Edge_Wall, CP_System_GetWindowWidth(), WALL_DIM);
						}
					}
				}
			}	
		}
	}
	else {
		
		if (proj->type == PROJ_TYPE_MOBILE) {
			if (PROJ_MELEE_LIFESPAN <= proj->LifeSpan) {
				//only checks for the collision at the end of the lifespan
				Entities_Collision_Check(proj, index, entities);
				entities[index].type = entity_null;
			}
		}
		else {
			if (proj->source == BOSS_PROJ_SOURCE || proj->source == MOB_PROJ_SOURCE) {
				if (PROJ_MELEE_LIFESPAN <= proj->LifeSpan) {
					//only checks for the collision at the end of the lifespan
					Entities_Collision_Check(proj, index, entities);
					entities[index].type = entity_null;
				}
			}

			else {
				if (proj->LifeSpan == 0) {
					Entities_Collision_Check(proj, index, entities);
				}
				if (PROJ_MELEE_LIFESPAN <= proj->LifeSpan) {
					entities[index].type = entity_null;
				}
			}
		}
		proj->LifeSpan += CP_System_GetDt();
	}
}

void deflectprojectiles(char source,int index, Entity entities[]) {
	Projectile* proj = &(entities[index].projectile);

	if (proj->type == PROJ_TYPE_MOBILE) {
		if (source == PROJ_VERTICAL_WALL) {
			proj->Direction.x *= -1;
		}
		else if (source == PROJ_HORIZONTAL_WALL) {
			proj->Direction.y *= -1;
		}
		else {			
			proj->Direction = CP_Vector_Negate(proj->Direction);
			proj->source = source;
			
		}
		if (proj->source == PLAYER_PROJ_SOURCE1)
			projectile_wall_bounce_particles(proj, source);
	}
	else{
		proj->type = PROJ_TYPE_STATIC;
		proj->source = source;
		proj->LifeSpan = 0.0f;
		proj->frame_idx = 0;
		proj->radius *= 2.0f;
	}
}

int Wall_Edge_Check(Projectile* proj, Position rect, float width, float height) {
	int collided = 0;
	for (int i = 5; i >0; i--) {
		Position Future_Pos = (Position){ proj->pos.x + (proj->Direction.x * (proj->speed / i) * CP_System_GetDt()) , proj->pos.y + (proj->Direction.y * (proj->speed / i) * CP_System_GetDt()) };
		if (collisionCircleRect(Future_Pos, proj->radius, rect, width, height)) {
			proj->Future_Pos = Future_Pos;
			proj->pos = Future_Pos;
			collided = 1;
			break;
		}
	}
	if (collided){
		
		
		Position Rect_Center = (Position){ rect.x + (width / 2),rect.y + (height / 2) };
		float y_diff = (fabs((proj->Future_Pos.y+proj->radius) - rect.y) < fabs((proj->Future_Pos.y-proj->radius) - (rect.y + height))) ? ((proj->Future_Pos.y+ proj->radius) - rect.y) : ((proj->Future_Pos.y-proj->radius) - (rect.y + height));
		float x_diff = (fabs((proj->Future_Pos.x+ proj->radius) - rect.x) < fabs((proj->Future_Pos.x- proj->radius) - (rect.x + width))) ? ((proj->Future_Pos.x+proj->radius) - rect.x) : ((proj->Future_Pos.x-proj->radius) - (rect.x + width));

		if (fabs(y_diff) < fabs(x_diff)) {
			proj->toRebound_NextFrame = PROJ_HORIZONTAL_WALL;
			int direction = (fabs((proj->Future_Pos.y + proj->radius) - rect.y) < fabs((proj->Future_Pos.y - proj->radius) - (rect.y + height))) ? -1 : 1;
			proj->Future_Pos.y = Rect_Center.y + direction * ((height / 2) + proj->radius);
		}
		else {
			proj->toRebound_NextFrame = PROJ_VERTICAL_WALL;
			int direction = (fabs((proj->Future_Pos.x + proj->radius) - rect.x) < fabs((proj->Future_Pos.x - proj->radius) - (rect.x + width))) ? -1 : 1;
			proj->Future_Pos.x = Rect_Center.x + direction * ((width / 2) + proj->radius);
		}
		
	}
	return collided;
}

int Entities_Collision_Check(Projectile* proj, int index, Entity entities[]){
	int Proj_Collided = 0;
	for (int i = 0; i < ENTITY_CAP; ++i) {
		if (entities[i].type == entity_null) continue;
		// if
		if (proj->source != PLAYER_PROJ_SOURCE1 && proj->source != PLAYER_PROJ_SOURCE2) {
			if (entities[i].type == entity_player) {
				if (collisionCircle(entities[i].player.pos, entities[i].player.diameter / 2, proj->pos, proj->radius)) {
					damage_player(&(entities[i].player));
					if (proj->type == PROJ_TYPE_MOBILE) {
						entities[index].type = entity_null;
					}
					Proj_Collided = 1;
					break;
				}
			}

		}
		else if (proj->source == PLAYER_PROJ_SOURCE1) {
			if (entities[i].type == entity_boss) {
				if (collisionCircle(entities[i].boss.pos, entities[i].boss.diameter / 2, proj->pos, proj->radius)) {
					damage_boss(&(entities[i].boss), &(entities[PLAYER_IDX].player));
					if (proj->type == PROJ_TYPE_MOBILE) {
						entities[index].type = entity_null;
					}
					Proj_Collided = 1;
					break;
				}
			}
			if (entities[i].type == entity_mob) {
				if (collisionCircle(entities[i].mob.pos, entities[i].mob.diameter / 2, proj->pos, proj->radius)) {
					damage_mob(&(entities[i].mob));
					if (proj->type == PROJ_TYPE_MOBILE) {
						entities[index].type = entity_null;
					}
					Proj_Collided = 1;
					//break;
				}
			}
			// Check if its Mob
		}
	}
	if (Proj_Collided) {
		//Draw the Mobile projectile destroy animation
	}
	return Proj_Collided;
}
void draw_projectile(Projectile* proj) {
	if (proj->type == PROJ_TYPE_MOBILE) {
		if (proj->source == PLAYER_PROJ_SOURCE1 || proj->source == PLAYER_PROJ_SOURCE2)
			CP_Image_Draw(Mobile_Proj_P, get_camera_x_pos(proj->pos.x), get_camera_y_pos(proj->pos.y), proj->radius * 2, proj->radius * 2, 255);
		else
			CP_Image_Draw(Mobile_Proj_E, get_camera_x_pos(proj->pos.x), get_camera_y_pos(proj->pos.y), proj->radius * 2, proj->radius * 2, 255);

	}
	else if (proj->type == PROJ_TYPE_STATIC) {
		float diff = (float)(proj->frame_idx % EXPLOSION_PROJECTILE_SPRITE_COUNT);
		if (proj->frame_idx > EXPLOSION_PROJECTILE_SPRITE_COUNT) return;

		if (proj->source == PLAYER_PROJ_SOURCE1)
			CP_Image_Draw(player_projectile_sprites[proj->frame_idx % 7], get_camera_x_pos(proj->pos.x), get_camera_y_pos(proj->pos.y), proj->radius * 2, proj->radius * 2, 255);
			//CP_Image_Draw(player_projectile_sprites[0 % 7], get_camera_x_pos(proj->pos.x), get_camera_y_pos(proj->pos.y), proj->radius * 2, proj->radius * 2, 255);
		else if (proj->source == MOB_PROJ_SOURCE)
			CP_Image_Draw(enemy_projectile_sprites[proj->frame_idx % 7], get_camera_x_pos(proj->pos.x), get_camera_y_pos(proj->pos.y), proj->radius * 2, proj->radius * 2, 255);

		if (proj->LifeSpan >= (PROJ_EXPLOSION_FRAME_DT * diff)) {
			(proj->frame_idx)++;
		}
	}
	else if (proj->type == PROJ_TYPE_WEAPON) {
		//CP_Settings_Fill(CP_Color_Create(0, 0, 0, 100));
		//CP_Graphics_DrawRect(proj->pos.x - proj->radius * 3.0f, proj->pos.y - proj->radius * 3.0f, proj->radius * 6.0f, proj->radius * 6.0f);

		//CP_Settings_Fill(CP_Color_Create(255, 255, 255, 255));
		//CP_Graphics_DrawCircle(proj->pos.x, proj->pos.y, proj->radius * 2.0f);


		float diff = (float)(proj->frame_idx % WEAPON_PROJECTILE_SPRITE_COUNT);
		if (proj->frame_idx > WEAPON_PROJECTILE_SPRITE_COUNT) return;
		
		float angle = vectorToAngle(proj->Direction);
			CP_Image_DrawAdvanced(
				sword_left[proj->frame_idx % WEAPON_PROJECTILE_SPRITE_COUNT], 
				get_camera_x_pos(proj->pos.x), 
				get_camera_y_pos(proj->pos.y), 
				proj->radius * 3.0f, proj->radius * 3.0f, 
				255,
				angle
			);
		if (proj->LifeSpan >= (PROJ_WEAPON_FRAME_DT * diff)) {
			(proj->frame_idx)++;
		}

		//CP_Settings_Fill(CP_Color_Create(255, 0, 0, 255));
		//CP_Settings_TextSize(50.0f);

		//char buffer[500] = { 0 };
		//sprintf_s(buffer, _countof(buffer), "%d %f", proj->frame_idx, proj->LifeSpan);
		//CP_Font_DrawText(buffer, proj->pos.x, proj->pos.y - proj->radius * 2.0f);
	}
}

