#include "cprocessing.h"
#include "utils.h"
#include "game.h"

#define MAX_PARRYRADIUS 50.0f;

typedef struct Player_Stats
{
	Position pos;
	float diameter;
	int speed;
	float parryrad;
} Player_Stats;

struct Player_Stats playerchar;
float baseweight = 255.0f;
float radius_reduction = 4.8f;
//struct Projectiles Projectile_List[100];

void game_init(void)
{
	float Window_Width = CP_System_GetWindowWidth();
	float Window_Height = CP_System_GetWindowHeight();
	Position p; 
	p.x = (Window_Width / 2) - (playerchar.diameter / 2); 
	p.y = (Window_Height / 2) - (playerchar.diameter / 2);
	playerchar.pos = p;
	playerchar.speed = 200;
	playerchar.diameter = 50.0f;
	playerchar.parryrad = MAX_PARRYRADIUS;
}

void game_update(void)
{
	CP_Graphics_ClearBackground(CP_Color_Create(150,150,150,255));
	for (int i = 0, sw = 2, radius_size = radius_reduction, parry_color = 255, parry_weight = baseweight; i < 8; ++i) {	//Creates the Barrier Effect
		if (i == 8 - 1) {	//Sets the white color ring
			radius_size = radius_reduction;
			parry_color = 255;
			parry_weight = baseweight;
			sw = 3;
		}
		else { // Sets the translucent blue barrier effect 
			radius_size += 4;
			parry_color -= 30;
			parry_weight -= 255/8;
		}
		CP_Settings_StrokeWeight(sw);
		CP_Settings_Stroke(CP_Color_Create(parry_color, 255, 255, parry_weight));
		CP_Settings_Fill(CP_Color_Create(218, 240, 255, 0));
		CP_Graphics_DrawCircle(playerchar.pos.x, playerchar.pos.y, playerchar.parryrad * 2 - radius_size);
	}
	 //Prints the playerchar Object
	CP_Settings_StrokeWeight(0.0f);
	CP_Settings_Fill(CP_Color_Create(51, 255, 173, 255));
	CP_Graphics_DrawCircle(playerchar.pos.x, playerchar.pos.y, playerchar.diameter);


	//Reduces the barrier strength whenever the playerchar clicks spacebar
	// If you want to reduce the opacity of the barrier, uncomment the "basewieght" variable in the "if" statement bellow
	// If you want to reduce the size of the barrier, uncomment the "radius_reduction" variable in the "if" statement bellow
	if (CP_Input_KeyTriggered(KEY_SPACE)) {

		if (baseweight >= 100)  baseweight -= 100; // if baseweight 
		//radius_reduction += 2;
	}
	
	
	//Increases the barrier's opacity over time ( Uncomment this if you want to change the opacity of the barrier when user click space)
	if (baseweight < 255 ) {
		baseweight += 60 * CP_System_GetDt();
	}
	

	/*
	//Increases the size of the barrier over time ( Uncomment this if you want to change the size of the barrier when user click space)
	if (radius_reduction >  4.8) {
		radius_reduction -= 10 * CP_System_GetDt();
	}
	*/

	//Basic Player movement
	if (CP_Input_KeyDown(KEY_D)) moveEntity(&(playerchar.pos), playerchar.speed, 0);
	else if (CP_Input_KeyDown(KEY_A)) moveEntity(&(playerchar.pos), -playerchar.speed, 0);
	if (CP_Input_KeyDown(KEY_W)) moveEntity(&(playerchar.pos), 0, -playerchar.speed) ;
	else if (CP_Input_KeyDown(KEY_S)) moveEntity(&(playerchar.pos), 0, playerchar.speed);
	
	CP_Settings_Fill(CP_Color_Create(255, 0, 0, 255));

	/* //Projectile Stuff [ Ignore for now ]
	if(CP_Input_MouseTriggered(MOUSE_BUTTON_LEFT))
	{
		struct Projectiles newprojectile;
		newprojecttile.direction_vector = CP_Vector_Normalize(CP_Vector_Set(CP_Input_GetMouseX()-playerchar.xpos, CP_Input_GetMouseY() - playerchar.ypos));
		newprojectile.xpos = playerchar.xpos;
		newprojectile.ypos = playerchar.ypos;
		Projectile_List[projectiles_count] = newprojectile;
		projectiles_count++;
	}
	for (int i = 0; i < projectiles_count; i++) {
		CP_Graphics_DrawCircle(Projectile_List[i].xpos+=(Projectile_List[i].direction_vector.x * 600 * CP_System_GetDt()), Projectile_List[i].ypos += (Projectile_List[i].direction_vector.y * 600 * CP_System_GetDt()), 20.f);
	}
	*/
}

void game_exit(void)
{
	
}

