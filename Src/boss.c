#include "boss.h"
#include "projectiles.h"
#include "utils.h"


#define  BOSS_HEALTH 10
#define  BOSS_ATK_CD 3
#define  BOSS_SPEED 8
#define  BOSS_DIAMETER 50.0f
#define  BOSS_PARRY_RAD 70.0f
#define  BOSS_PARRY_AMMO  10
#define  BOSS_PARRY_CD  5
#define  BOSS_PARRY_BASEWEIGHT  255.0f
#define  BOSS_RAD_GRADIENT 8
#define  ENTITY_CAP 100


Position target_location;
CP_Vector D_vector;
CP_Vector Directional_Vector;
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
void update_boss(int boss_idx, int player_idx, Entity entities[], int wall_pos[GRID_ROWS][GRID_COLS]) {
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
		D_vector = CP_Vector_Normalize(CP_Vector_Set(player->pos.x - boss->pos.x, player->pos.y - boss->pos.y));
		CP_Vector offset_Vector = CP_Vector_Negate(CP_Vector_Scale(D_vector, (boss->parryrad - (boss->diameter)) / CP_Vector_Length(D_vector)));
		//D_vector = CP_Vector_Subtract(D_vector, offset_Vector);
		target_location.x = player->pos.x + offset_Vector.x;
		target_location.y = player->pos.y + offset_Vector.y;
		D_vector = CP_Vector_Set(target_location.x - boss->pos.x, target_location.y - boss->pos.y);
		/*
		target_location = player->pos;
		D_vector = 
		CP_Vector offset_Vector = CP_Vector_Scale(CP_Vector_Normalize(D_vector), -1*(player->parryrad - player->diameter / 2) / CP_Vector_Length(D_vector));
		moveEntity(&target_location, offset_Vector.x, offset_Vector.y);
		D_vector = CP_Vector_Add(D_vector, offset_Vector);
		boss->parry_ammo -= 10;
		*/
		//Need Target Location
		//Need D_vector
	}
	if (To_Atk) {
		// Check for wall Collision
		Position Next_Position;
		Next_Position.x = boss->pos.x + (D_vector.x * (Acceleration_Count / Acceleration));
		Next_Position.y = boss->pos.y + (D_vector.x * (Acceleration_Count / Acceleration));
		
		Position Old_Position = boss->pos;
		CP_Vector Directional_Vector = getVectorBetweenPositions(&(boss->pos), &Next_Position);
		float scalar = boss->parryrad / CP_Vector_Length(Directional_Vector);
		Directional_Vector = CP_Vector_Scale(Directional_Vector, scalar);
		
		moveEntity(&(boss->pos), D_vector.x * (Acceleration_Count / Acceleration) * CP_System_GetFrameRate(), D_vector.y * (Acceleration_Count / Acceleration) * CP_System_GetFrameRate());
		
		for (int i = 0; i < GRID_ROWS; ++i) {
			for (int j = 0; j < GRID_COLS; ++j) {
				if (wall_pos[i][j]){
					float distance = CP_Vector_Distance(CP_Vector_Set(Old_Position.x, Old_Position.y),CP_Vector_Set(Next_Position.x, Next_Position.y));
					int iteration = distance / WALL_DIM;
					Position Dummy_Position = Old_Position;
					for (int iter = 0; iter < iteration; iter++) {
						moveEntity(&Dummy_Position, Directional_Vector.x, Directional_Vector.y);
						if (collisionCircleRect(Dummy_Position, boss->parryrad, (Position) { WALL_DIM* (float)j, WALL_DIM* (float)i }, WALL_DIM, WALL_DIM)) {
							wall_pos[i][j] = 0;
						}
					}
				}
			}
		}
		
		if (Acceleration_Count == boss->speed) {
			boss->pos = target_location;
			for (int i = 0; i < ENTITY_CAP; ++i) {
				if (entities[i].type == entity_null) {
					Position Melee_Atk_Pos = boss->pos;
					Projectile proj = init_projectile('b', Melee_Atk_Pos, CP_Vector_Set(0,0),'m');
					proj.radius = boss->parryrad;
					entities[i].type = entity_projectile;
					entities[i].projectile = proj;
					break;
				}
			}
			To_Atk = 0;
			Acceleration_Count = 1;
		}
		else {
			Acceleration_Count++;
		}

	}
	
	

}
