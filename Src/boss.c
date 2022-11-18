#include "boss.h"
#include "projectiles.h"
#include "utils.h"
#include <stdlib.h>
#include "camera.h"


#define  BOSS_HEALTH 10
#define  BOSS_ATK_CD 2
#define  BOSS_SPEED 10
#define  BOSS_DIAMETER 1.851f*WALL_DIM
#define  BOSS_PARRY_RAD 1.851f*WALL_DIM
#define  BOSS_PARRY_AMMO  10
#define  BOSS_PARRY_CD  5
#define  BOSS_PARRY_BASEWEIGHT  255.0f
#define  ENTITY_CAP 100


static Position target_location;
static CP_Vector D_vector;
static float Atk_Time_Tracker = 0.0f;
static float Acceleration;
static int Parry_On_Cd = 0;
static int To_Atk = 0;
static int Acceleration_Count = 1;
static float Parry_CD_Timer = 0;
static float Cannon_Timer = 0;
static float animationcount = 0;
static int boss_direction = 0;
static int Boss_is_stun = 0;


CP_Image* boss_img = NULL;
CP_Image* Boss_Atk_Img[2];

CP_Image Cannon_Img;
CP_Image boss_def;
CP_Image Boss_Stun[2];
CP_Image Boss_Barrier_Img;
CP_Image Boss_Atk_Right[BOSS_SPRITE_COUNT];
CP_Image Boss_Atk_Left[BOSS_SPRITE_COUNT];
//CP_Image boss_atk5 = NULL;

entity_struct init_boss(void) {

	Boss boss;
	boss.health = BOSS_HEALTH;
	boss.atk_cd = BOSS_ATK_CD;
	boss.speed = BOSS_SPEED;
	boss.parry_ammo = BOSS_PARRY_AMMO;
	boss.parry_cd = BOSS_PARRY_CD;
	boss.diameter = BOSS_DIAMETER;
	boss.parryrad = BOSS_PARRY_RAD;
	boss.Parry_BaseWeight = BOSS_PARRY_BASEWEIGHT;
	boss.pos.x = ((float)CP_System_GetWindowWidth() /2) - (boss.diameter / 2);
	boss.pos.y = ((float)CP_System_GetWindowHeight() * 1 / 4) - (boss.diameter / 2);

	for (int i = 1; i <= boss.speed; i++) {
		Acceleration += i;
	}
	target_location = boss.pos;

	Boss_Atk_Img[0] = Boss_Atk_Left;
	Boss_Atk_Img[1] = Boss_Atk_Right;
	
	boss_img = &boss_def;
	return (entity_struct) { .boss = boss };
}
void update_boss(int boss_idx, int player_idx, Entity entities[], int wall_pos[GRID_ROWS][GRID_COLS]) {
	Boss* boss = &(entities[boss_idx].boss);
	Player* player = &(entities[player_idx].boss);
	if (boss->health) {
		if (Boss_is_stun) {
			
			
			//target_location = (Position){boss->pos.x+D_vector.x,boss->pos.y+D_vector.y};
			moveEntity(&(boss->pos), D_vector.x, D_vector.y);
			//boss->pos = target_location;
			if (animationcount >=0.2) {
				Boss_is_stun = 0;
			}
			CP_Image_DrawAdvanced(Cannon_Img, get_camera_x_pos(0.5 * (float)CP_System_GetWindowWidth()), get_camera_y_pos(0 + WALL_DIM), WALL_DIM * 2, WALL_DIM * 2, 255, 180);
			
		}
		else{
			if (!Parry_On_Cd) {
				for (int i = 0; i < ENTITY_CAP; ++i) {
					if (entities[i].type != entity_null && entities[i].type == entity_projectile) {
						if (collisionCircle(boss->pos, boss->parryrad, entities[i].projectile.pos, entities[i].projectile.radius) && entities[i].projectile.source != 'e' && entities[i].projectile.type == PROJ_TYPE_MOBILE) {
							if (boss->parry_ammo > 0) {
								deflectprojectiles('e', i, entities);
								boss->parry_ammo--;
								boss->Parry_BaseWeight -= BOSS_PARRY_BASEWEIGHT / BOSS_PARRY_AMMO;
							}
						}
					}
				}
				if (boss->parry_ammo == 0) {
					Parry_CD_Timer = 0;
					Parry_On_Cd = 1;
				}
			}
			else {
				Parry_CD_Timer += CP_System_GetDt();

				if (Parry_CD_Timer >= boss->parry_cd) {
					boss->parry_ammo = BOSS_PARRY_AMMO;
					boss->Parry_BaseWeight = 255;
					Parry_On_Cd = 0;
				}
			}



			Atk_Time_Tracker += CP_System_GetDt();
			//Boss movement
			if (Atk_Time_Tracker > boss->atk_cd) {
				To_Atk = 1;
				Atk_Time_Tracker = 0;
			
				D_vector = CP_Vector_Normalize(CP_Vector_Set(player->pos.x - boss->pos.x, player->pos.y - boss->pos.y));
				CP_Vector offset_Vector = CP_Vector_Negate(CP_Vector_Scale(D_vector, player->diameter)); ;//CP_Vector_Negate(CP_Vector_Scale(D_vector, (boss->parryrad - (boss->diameter)) / CP_Vector_Length(D_vector)));
				target_location.x = player->pos.x + offset_Vector.x;
				target_location.y = player->pos.y + offset_Vector.y;
				D_vector = CP_Vector_Set(target_location.x - boss->pos.x, target_location.y - boss->pos.y);
				//Draw the Boss atk_prep animation
			}
			if (To_Atk) {
			
				boss_img = *(Boss_Atk_Img + boss_direction) + (int)(((float)Acceleration_Count / boss->speed) * 7);
				for (int x = 10; x > 0; x--) { //Destroy any walls that it collides with
					Position Next_Position;
					Next_Position.x = boss->pos.x + (D_vector.x * (Acceleration_Count / Acceleration) / x);
					Next_Position.y = boss->pos.y + (D_vector.x * (Acceleration_Count / Acceleration) / x);
					Destory_Wall(wall_pos, Next_Position, boss->diameter, boss->parryrad, boss->parry_ammo, WALL_DIM, WALL_DIM);
				}

				if (Acceleration_Count == boss->speed) {
					boss->pos = target_location;
					//Draw the Boss atked Animation 
					Destory_Wall(wall_pos, boss->pos, boss->diameter, boss->parryrad, boss->parry_ammo, WALL_DIM, WALL_DIM);
					int p_idx = insert_to_entity_array(entity_projectile, entities, init_projectile);
					if (p_idx > -1) {
						set_projectile_values(&(entities[p_idx].projectile), 'e', 's', boss->parryrad, boss->pos, CP_Vector_Set(0, 0));
					}
					boss_img = *(Boss_Atk_Img + boss_direction) + 6;
					To_Atk = 0;
					Acceleration_Count = 1;
					animationcount = 0;
				}
				else {
					moveEntity(&(boss->pos), D_vector.x * (Acceleration_Count / Acceleration) * CP_System_GetFrameRate(), D_vector.y * (Acceleration_Count / Acceleration) * CP_System_GetFrameRate());
					Acceleration_Count++;
				}
			
			}
			else {
				if (animationcount >= 0.5) {
					boss_img = &boss_def;
				}
				if (Atk_Time_Tracker >= 3.0/4.0 * (float)boss->atk_cd) {
					boss_direction = ((getVectorBetweenPositions(&(boss->pos), &(player->pos)).x > 0));
					boss_img = *(Boss_Atk_Img + boss_direction);
				}
			}

		}
	}
	else {
		entities[boss_idx].type = entity_null;

	}
	Cannon_Fire_Proj(entities, player);
	animationcount += CP_System_GetDt();
	Cannon_Timer += CP_System_GetDt();

}
void Destory_Wall(int wall_pos[GRID_ROWS][GRID_COLS], Position Boss_Pos, int boss_diameter, int parry_rad ,int parry_ammo, int wall_width, int wall_height) {
	for (int i = 0; i < GRID_ROWS; ++i) {
		for (int j = 0; j < GRID_COLS; ++j) {
			if (wall_pos[i][j]) {
				int collided = 0;
				collided = collisionCircleRect(Boss_Pos, parry_rad, (Position) { wall_width* (float)j, wall_height* (float)i }, wall_width, wall_height);			
				if(parry_ammo){collided = collisionCircleRect(Boss_Pos, parry_rad, (Position) { wall_width* (float)j, wall_height* (float)i }, wall_width, wall_height); }
				else { collided = collisionCircleRect(Boss_Pos, boss_diameter, (Position) { wall_width* (float)j, wall_height* (float)i }, wall_width, wall_height); }
				if (collided) { wall_pos[i][j] = 0; }
			}
		}
	}
}

void damage_boss(Boss* b, Player* p) {
	b->health -= 1;
	Boss_is_stun = 1;
	animationcount = 0;
	D_vector = CP_Vector_Scale(getVectorBetweenPositions(&(p->pos), &(b->pos)), 500);
	boss_img = &(Boss_Stun[(D_vector.x>0)]);
	
}

void draw_boss(Boss* b) {
	CP_Image_Draw(Boss_Barrier_Img, get_camera_x_pos(b->pos.x), get_camera_y_pos(b->pos.y), b->parryrad * 2, b->parryrad * 2, b->Parry_BaseWeight);
	int size = 4.63 *WALL_DIM;
	CP_Image_Draw(*boss_img, get_camera_x_pos(b->pos.x), get_camera_y_pos(b->pos.y), size, size, 255);
	//CP_Image_DrawAdvanced(Cannon_Img, get_camera_x_pos(0.5 * (float)CP_System_GetWindowWidth()), get_camera_y_pos(0 + WALL_DIM), WALL_DIM * 2, WALL_DIM * 2, 255,180);

}

void Cannon_Fire_Proj(Entity entities[],Player *player) {
	
	Position CanonProj1 = (Position){0.5* CP_System_GetWindowWidth(),0+WALL_DIM+10};
	Position CanonProj2 = (Position){ 0.5 * CP_System_GetWindowWidth(),CP_System_GetWindowHeight()-WALL_DIM-10 };
	if(Cannon_Timer >= 2){
		int p_idx1 = insert_to_entity_array(entity_projectile, entities, init_projectile);
		if (p_idx1 > 0) {
			set_projectile_values(&(entities[p_idx1].projectile), 'e', 'm', 10, CanonProj1, getVectorBetweenPositions(&(CanonProj1), &(player->pos)));
		}
		int p_idx2 = insert_to_entity_array(entity_projectile, entities, init_projectile);
		if (p_idx1 > 0) {
			set_projectile_values(&(entities[p_idx2].projectile), 'e', 'm', 10, CanonProj2, getVectorBetweenPositions(&(CanonProj2), &(player->pos)));
		}
		Cannon_Timer = 0;
	}
}