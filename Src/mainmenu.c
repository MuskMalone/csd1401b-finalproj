#include "cprocessing.h"
#include "game.h"
#include "gametypes.h"

void Main_Menu_Init()
{
	//To be put in the Mainmenu.c file. (Setting the window size)

	// width and height of the window will be based on how wide each wall is
	float W_width =  WALL_DIM * GRID_COLS;
	float W_height = WALL_DIM * GRID_ROWS;
	float Menu_Rect_Width = W_width / 8;
	float Menu_Rect_Height = W_height / 8;
	CP_System_SetWindowSize(W_width, W_height);
}

void Main_Menu_Update()
{

	CP_Graphics_ClearBackground(CP_Color_Create(0, 0, 0, 255));
	CP_Font_DrawText("Main Menu Click SpaceBar to continue", CP_System_GetDisplayWidth() / 2, CP_System_GetDisplayHeight() / 2);
	if(CP_Input_KeyTriggered(KEY_SPACE)){
		CP_Engine_SetNextGameState(game_init, game_update, game_exit);
	    CP_Engine_Run();
	}

	
}

void Main_Menu_Exit()
{

}