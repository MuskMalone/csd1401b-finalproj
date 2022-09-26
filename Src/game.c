#include "cprocessing.h"

struct Player_Stats
{
	float xpos;
	float ypos;
	float diameter;
	int speed;
};

struct Player_Stats player;
void game_init(void)
{

	
	float Window_Width = CP_System_GetWindowWidth();
	float Window_Height = CP_System_GetWindowHeight();
	player.xpos = (Window_Width/2) - (player.diameter/2);
	player.ypos = (Window_Height/2) - (player.diameter/2);
	player.speed = 200;
	player.diameter = 40.0f;
}

void game_update(void)
{
	CP_Graphics_ClearBackground(CP_Color_Create(150,150,150,255));
	CP_Settings_Fill(CP_Color_Create(255, 255, 90, 255));
	CP_Graphics_DrawCircle(player.xpos, player.ypos, player.diameter);
	
	player.xpos += CP_Input_KeyDown(KEY_D)? (1 * player.speed * CP_System_GetDt()) : CP_Input_KeyDown(KEY_A) ? (-1 * player.speed * CP_System_GetDt()) : 0;
	player.ypos += CP_Input_KeyDown(KEY_S) ? (1 * player.speed * CP_System_GetDt()) : CP_Input_KeyDown(KEY_W) ? (-1 * player.speed * CP_System_GetDt()) : 0;



}

void game_exit(void)
{
	
}

