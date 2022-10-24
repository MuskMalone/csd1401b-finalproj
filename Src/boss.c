#include "boss.h"
#include "utils.h"

float Boss_Max_Parry = 70.0f;
float Boss_baseweight = 255.0f;
float Boss_radius_reduction = 4.8f;
float attack_cd = 3;
float attack_timer = 0.0f;
int is_atk = 0;
float count = 1;
Position target_location;
CP_Vector D_vector;

Boss init_boss(void) {
	Boss boss;
	float Window_Width = CP_System_GetWindowWidth();
	float Window_Height = CP_System_GetWindowHeight();
	Position p;


	boss.speed = 50;
	boss.diameter = 50.0f;
	p.x = (Window_Width / 2) - (boss.diameter / 2);
	p.y = (Window_Height* 1/4) - (boss.diameter / 2);
	boss.pos = p;
	boss.parryrad = Boss_Max_Parry;
	target_location = boss.pos;
	return boss;
}
void update_boss(int boss_idx, int player_idx, Entity entities[]) {
	Boss* boss = &(entities[boss_idx].boss);
	Player* player = &(entities[player_idx].boss);
	for (int i = 0, sw = 2, radius_size = Boss_radius_reduction, parry_color = 255, parry_weight = Boss_baseweight; i < 8; ++i) {	//Creates the Barrier Effect
		if (i == 8 - 1) {	//Sets the white color ring
			radius_size = Boss_radius_reduction;
			parry_color = 255;
			parry_weight = Boss_baseweight;
			sw = 3;
		}
		else { // Sets the translucent red barrier effect 
			radius_size += 4;
			parry_color -= 30;
			parry_weight -= 255 / 8;
		}
		CP_Settings_StrokeWeight(sw);
		CP_Settings_Stroke(CP_Color_Create(parry_color, 50, 0, parry_weight));
		CP_Settings_Fill(CP_Color_Create(0, 0, 0, 0));
		CP_Graphics_DrawCircle(boss->pos.x, boss->pos.y, boss->parryrad * 2 - radius_size);
	}
	//Prints the Boss Object
	CP_Settings_StrokeWeight(0.0f);
	CP_Settings_Fill(CP_Color_Create(255, 255, 255, 255));
	CP_Graphics_DrawCircle(boss->pos.x, boss->pos.y, boss->diameter);


	//Reduces the barrier strength whenever the player clicks spacebar
	// If you want to reduce the opacity of the barrier, uncomment the "basewieght" variable in the "if" statement bellow
	/*
	if (CP_Input_KeyTriggered(KEY_SPACE)) {

		if (Boss_baseweight >= 30) {
			baseweight -= 30;
		}
		else {
			baseweight = 0;
		}
	}
	else {

		//Increases the barrier's opacity over time ( Uncomment this if you want to change the opacity of the barrier when user click space)
		if (baseweight < 255) {
			baseweight += 50 * CP_System_GetDt();
		}
	}
	*/

	//Boss movement
	attack_timer += CP_System_GetDt();
	if (attack_timer > 3) {
		is_atk = 1;
		attack_timer = 0;
		target_location = player->pos;
		D_vector = CP_Vector_Set(target_location.x - boss->pos.x, target_location.y - boss->pos.y);
	}
	if(is_atk) {
		boss->pos.x += (D_vector.x*(count/28.0f));
		boss->pos.y += (D_vector.y*(count/28.0f));
		if (count==7){
			boss->pos = target_location;
			is_atk = 0;
			count = 1;
		}
		count++;
	}
	

}
