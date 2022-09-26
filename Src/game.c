#include "cprocessing.h"

struct Player_Stats
{
	float xpos;
	float ypos;
	float diameter;
	int speed;
};

struct Projectiles
{
	CP_Vector direction_vector;
	float xpos;
	float ypos;
	int speed;
};

struct Player_Stats player;
struct Projectiles Projectile_List[100];
int projectiles_count = 0;
void game_init(void)
{

	
	float Window_Width = CP_System_GetWindowWidth();
	float Window_Height = CP_System_GetWindowHeight();
	player.xpos = (Window_Width/2) - (player.diameter/2);
	player.ypos = (Window_Height/2) - (player.diameter/2);
	player.speed = 200;
	player.diameter = 50.0f;

}

void game_update(void)
{
	CP_Graphics_ClearBackground(CP_Color_Create(150,150,150,255));
	CP_Settings_Fill(CP_Color_Create(51, 255, 173, 255));
	CP_Graphics_DrawCircle(player.xpos, player.ypos, player.diameter);
	
	player.xpos += CP_Input_KeyDown(KEY_D)? (1 * player.speed * CP_System_GetDt()) : CP_Input_KeyDown(KEY_A) ? (-1 * player.speed * CP_System_GetDt()) : 0;
	player.ypos += CP_Input_KeyDown(KEY_S) ? (1 * player.speed * CP_System_GetDt()) : CP_Input_KeyDown(KEY_W) ? (-1 * player.speed * CP_System_GetDt()) : 0;

	if(CP_Input_MouseTriggered(MOUSE_BUTTON_LEFT))
	{
		struct Projectiles newprojecttile;
		newprojecttile.direction_vector = CP_Vector_Normalize(CP_Vector_Set(CP_Input_GetMouseX()-player.xpos, CP_Input_GetMouseY() - player.ypos));
		newprojecttile.xpos = player.xpos;
		newprojecttile.ypos = player.ypos;
		Projectile_List[projectiles_count] = newprojecttile;
		projectiles_count++;
	}
	CP_Settings_Fill(CP_Color_Create(255, 0, 0, 255));
	for (int i = 0; i < projectiles_count; i++) {
		struct Projectiles project = Projectile_List[i];
		CP_Graphics_DrawCircle(Projectile_List[i].xpos+=(Projectile_List[i].direction_vector.x * 600 * CP_System_GetDt()), Projectile_List[i].ypos += (Projectile_List[i].direction_vector.y * 600 * CP_System_GetDt()), 20.f);
	}

}

void game_exit(void)
{
	
}

