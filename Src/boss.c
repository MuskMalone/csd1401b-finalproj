#include "boss.h"
#include "projectiles.h"
#include "utils.h"


#define  BOSS_HEALTH 10
#define  BOSS_ATK_CD 2
#define  BOSS_SPEED 10
#define  BOSS_DIAMETER 50.0f
#define  BOSS_PARRY_RAD 70.0f
#define  BOSS_PARRY_AMMO  10
#define  BOSS_PARRY_CD  5
#define  BOSS_PARRY_BASEWEIGHT  255.0f
#define  BOSS_RAD_GRADIENT 8
#define  ENTITY_CAP 100


static Position target_location;
static CP_Vector D_vector;
static CP_Vector Directional_Vector;
static float Atk_Time_Tracker = 0.0f;
static float Acceleration;
static int Parry_On_Cd = 0;
static int To_Atk = 0;
static int Acceleration_Count = 1;
static float Parry_CD_Timer = 0;
static float animationcount = 0;

CP_Image boss_def = NULL;
CP_Image Boss_Barrier_Img = NULL;
CP_Image boss_atk1 = NULL;
CP_Image boss_atk2 = NULL;
CP_Image boss_atk3 = NULL;
CP_Image boss_atk4 = NULL;
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
	Boss_Barrier_Img = CP_Image_Load("./Assets/Tiles/Boss/Boss_Barrier.png");
	return (entity_struct) { .boss = boss };
}
void update_boss(int boss_idx, int player_idx, Entity entities[], int wall_pos[GRID_ROWS][GRID_COLS]) {
	Boss* boss = &(entities[boss_idx].boss);
	Player* player = &(entities[player_idx].boss);
	boss_def = CP_Image_Load("./Assets/Tiles/Boss/Boss_Base.png");
	boss_atk1 = CP_Image_Load("./Assets/Tiles/Boss/BossAtk1.png");
	boss_atk2 = CP_Image_Load("./Assets/Tiles/Boss/BossAtk2.png");
	boss_atk3 = CP_Image_Load("./Assets/Tiles/Boss/BossAtk3.png");
	boss_atk4 = CP_Image_Load("./Assets/Tiles/Boss/BossAtk4.png");
	//boss_atk5 = CP_Image_Load("./Assets/Tiles/Boss/BossAtk5.png");
	CP_Image Boss_Image[] = { boss_def,boss_atk1 ,boss_atk2 ,boss_atk3,boss_atk4,boss_def };

	if (boss->health) {
		CP_Image_Draw(Boss_Barrier_Img, boss->pos.x, boss->pos.y, boss->parryrad * 2, boss->parryrad * 2, boss->Parry_BaseWeight);
		//Prints the Boss Object
		CP_Settings_StrokeWeight(0.0f);
		CP_Settings_Fill(CP_Color_Create(255 / (11 - boss->health), 255, 255, 255));

		
		CP_Image_Draw(Boss_Image[(int)animationcount % 6], boss->pos.x, boss->pos.y, boss->diameter * 2, boss->diameter * 2, 255);

		

		//Boss Deflect
		if (!Parry_On_Cd) {
			for (int i = 0; i < ENTITY_CAP; ++i) {
				if (entities[i].type != entity_null && entities[i].type == entity_projectile) {
					if (collisionCircle(boss->pos, boss->parryrad, entities[i].projectile.pos, entities[i].projectile.radius) && entities[i].projectile.source == 'p') {
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
			//To_Atk = 1;
			Atk_Time_Tracker = 0;
			D_vector = CP_Vector_Normalize(CP_Vector_Set(player->pos.x - boss->pos.x, player->pos.y - boss->pos.y));
			CP_Vector offset_Vector = CP_Vector_Negate(CP_Vector_Scale(D_vector, (boss->parryrad - (boss->diameter)) / CP_Vector_Length(D_vector)));
			target_location.x = player->pos.x + offset_Vector.x;
			target_location.y = player->pos.y + offset_Vector.y;
			D_vector = CP_Vector_Set(target_location.x - boss->pos.x, target_location.y - boss->pos.y);
			//Draw the Boss atk_prep animation
		}
		if (To_Atk) {
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
				To_Atk = 0;
				Acceleration_Count = 1;
			}
			else {
				moveEntity(&(boss->pos), D_vector.x * (Acceleration_Count / Acceleration) * CP_System_GetFrameRate(), D_vector.y * (Acceleration_Count / Acceleration) * CP_System_GetFrameRate());
				Acceleration_Count++;
			}
		}
	}
	else {
		entities[boss_idx].type = entity_null;

	}


	animationcount+= CP_System_GetDt()*10;


}
void Destory_Wall(int wall_pos[GRID_ROWS][GRID_COLS], Position Boss_Pos, int boss_diameter, int parry_rad ,int parry_ammo, int wall_width, int wall_height) {
	for (int i = 0; i < GRID_ROWS; ++i) {
		for (int j = 0; j < GRID_COLS; ++j) {
			if (wall_pos[i][j]) {
				int collided = 0;
				if(parry_ammo){collided = collisionCircleRect(Boss_Pos, parry_rad, (Position) { wall_width* (float)j, wall_height* (float)i }, wall_width, wall_height); }
				else { collided = collisionCircleRect(Boss_Pos, boss_diameter, (Position) { wall_width* (float)j, wall_height* (float)i }, wall_width, wall_height); }
				if (collided) { wall_pos[i][j] = 0; }
			}
		}
	}
}

void damage_boss(Boss* b) {
	b->health -= 1;
}
