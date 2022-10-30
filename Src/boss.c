#include "boss.h"
#include "projectiles.h"
#include "utils.h"


#define  BOSS_HEALTH 10
#define  BOSS_ATK_CD 3
#define  BOSS_SPEED 10
#define  BOSS_DIAMETER 50.0f
#define  BOSS_PARRY_RAD 70.0f
#define  BOSS_PARRY_AMMO  10
#define  BOSS_PARRY_CD  5
#define  BOSS_PARRY_BASEWEIGHT  255.0f
#define  BOSS_RAD_GRADIENT 8
#define  ENTITY_CAP 100


Position target_location;
CP_Vector D_vector;
float Atk_Time_Tracker = 0.0f;
float Acceleration;
int Parry_On_Cd = 0;
int To_Atk = 0;
int Acceleration_Count = 1;
float Parry_CD_Timer = 0;


Boss init_boss(void) {
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
	return boss;
}
void update_boss(int boss_idx, int player_idx, Entity entities[]) {
	Boss* boss = &(entities[boss_idx].boss);
	Player* player = &(entities[player_idx].boss);
	for (int i = 1, stroke_weight = 3, radius_size = BOSS_PARRY_RAD, parry_color = 255, parry_weight = boss->Parry_BaseWeight; i < (BOSS_RAD_GRADIENT+1); ++i) {	//Creates the Barrier Effect
		CP_Settings_StrokeWeight(stroke_weight);
		CP_Settings_Stroke(CP_Color_Create(parry_color, 50, 0, parry_weight));
		CP_Settings_Fill(CP_Color_Create(0, 0, 0, 0));
		CP_Graphics_DrawCircle(boss->pos.x, boss->pos.y, radius_size*2);
		radius_size -= (BOSS_PARRY_RAD/BOSS_RAD_GRADIENT)-stroke_weight*2;
		parry_color -= (255/BOSS_RAD_GRADIENT);
		parry_weight -= (boss->Parry_BaseWeight/ BOSS_RAD_GRADIENT);
	}
	//Prints the Boss Object
	CP_Settings_StrokeWeight(0.0f);
	CP_Settings_Fill(CP_Color_Create(255, 255, 255, 255));
	CP_Graphics_DrawCircle(boss->pos.x, boss->pos.y, boss->diameter);


	//Boss Deflect
	if(!Parry_On_Cd){
		for (int i = 0; i < ENTITY_CAP; ++i) {
			if (entities[i].type != entity_null && entities[i].type == entity_projectile) {
				if (collisionCircle(boss->pos, boss->parryrad, entities[i].projectile.pos, entities[i].projectile.radius) && entities[i].projectile.source =='p') {
					if (boss->parry_ammo > 0) {
						deflectprojectiles('e', i, entities,(int)Acceleration_Count);
						boss->parry_ammo--;
						boss->Parry_BaseWeight -= BOSS_PARRY_BASEWEIGHT/BOSS_PARRY_AMMO;
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
		target_location = player->pos;
		D_vector = CP_Vector_Set(target_location.x - boss->pos.x, target_location.y - boss->pos.y);
	}
	if (To_Atk) {
		moveEntity(&(boss->pos), D_vector.x * (Acceleration_Count / Acceleration) * CP_System_GetFrameRate(), D_vector.y * (Acceleration_Count / Acceleration) * CP_System_GetFrameRate());
		if (Acceleration_Count == boss->speed) {
			boss->pos = target_location;
			To_Atk = 0;
			Acceleration_Count = 1;
		}
		else {
			Acceleration_Count++;
		}

	}
	
	

}
