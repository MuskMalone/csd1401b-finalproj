#include "cprocessing.h"
#define MAX_PARRYRADIUS 50.0f;

struct Player_Stats
{
	float xpos;
	float ypos;
	float diameter;
	int speed;
	float parryrad;
};

/*
struct Projectiles
{
	CP_Vector direction_vector;
	float xpos;
	float ypos;
	int speed;
};
*/

struct Player_Stats player;
float baseweight = 255.0f;
float radius_reduction = 4.8f;
//struct Projectiles Projectile_List[100];

void game_init(void)
{
	float Window_Width = CP_System_GetWindowWidth();
	float Window_Height = CP_System_GetWindowHeight();
	player.xpos = (Window_Width/2) - (player.diameter/2);
	player.ypos = (Window_Height/2) - (player.diameter/2);
	player.speed = 200;
	player.diameter = 50.0f;
	player.parryrad = MAX_PARRYRADIUS;
}

void game_update(void)
{
	CP_Graphics_ClearBackground(CP_Color_Create(150,150,150,255));
	for (int i = 0, sw = 2, radius_size = radius_reduction, parry_color = 255, parry_weight = baseweight; i < 8; i++) {	//Creates the Barrier Effect
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
		CP_Graphics_DrawCircle(player.xpos, player.ypos, player.parryrad * 2 - radius_size);
	}
	 //Prints the player Object
	CP_Settings_StrokeWeight(0.0f);
	CP_Settings_Fill(CP_Color_Create(51, 255, 173, 255));
	CP_Graphics_DrawCircle(player.xpos, player.ypos, player.diameter);


	//Reduces the barrier strength whenever the player clicks spacebar
	// If you want to reduce the opacity of the barrier, uncomment the "basewieght" variable in the "if" statement bellow
	// If you want to reduce the size of the barrier, uncomment the "radius_reduction" variable in the "if" statement bellow
	if (CP_Input_KeyDown(KEY_SPACE)) {
		baseweight -= 15;
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
	player.xpos += CP_Input_KeyDown(KEY_D)? (1 * player.speed * CP_System_GetDt()) : CP_Input_KeyDown(KEY_A) ? (-1 * player.speed * CP_System_GetDt()) : 0;
	player.ypos += CP_Input_KeyDown(KEY_S) ? (1 * player.speed * CP_System_GetDt()) : CP_Input_KeyDown(KEY_W) ? (-1 * player.speed * CP_System_GetDt()) : 0;
	
	CP_Settings_Fill(CP_Color_Create(255, 0, 0, 255));

	/* //Projectile Stuff [ Ignore for now ]
	if(CP_Input_MouseTriggered(MOUSE_BUTTON_LEFT))
	{
		struct Projectiles newprojectile;
		newprojecttile.direction_vector = CP_Vector_Normalize(CP_Vector_Set(CP_Input_GetMouseX()-player.xpos, CP_Input_GetMouseY() - player.ypos));
		newprojectile.xpos = player.xpos;
		newprojectile.ypos = player.ypos;
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

