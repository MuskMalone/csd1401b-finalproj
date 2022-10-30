#include "cprocessing.h"
#include "game.h"
#include "gametypes.h"

float M_width;
float M_height;
float buttonwidth = 1024;
float buttonheight = 512;
CP_Image StartBut = NULL;
CP_Image HowBut = NULL;
CP_Image EndBut = NULL;
CP_Image background = NULL;
CP_Image Title = NULL;

void loadfile()
{
	StartBut = CP_Image_Load("./Assets/startbutton.png");
	EndBut = CP_Image_Load("./Assets/EXIT.png");
	HowBut = CP_Image_Load("./Assets/HOWTOPLAY.png");
	background = CP_Image_Load("./Assets/main.png");
	Title = CP_Image_Load("./Assets/Title.png");
}

void goGame()
{
	CP_Engine_SetNextGameState(game_init, game_update, game_exit);
	CP_Engine_Run();

}
void Main_Menu_Init()
{

	//To be put in the Mainmenu.c file. (Setting the window size)

	// width and height of the window will be based on how wide each wall is
	float W_width =  WALL_DIM * GRID_COLS;
	float W_height = WALL_DIM * GRID_ROWS;
	float Menu_Rect_Width = W_width / 8;
	float Menu_Rect_Height = W_height / 8;
	CP_System_SetWindowSize(W_width, W_height);
	M_width = CP_System_GetWindowWidth() / 2;
	M_height = CP_System_GetWindowHeight() / 2;
	loadfile();
}

void Main_Menu_Update()
{
	CP_Graphics_ClearBackground(CP_Color_Create(0, 0, 0, 255));
	//CP_Font_DrawText("Main Menu Click SpaceBar to continue", CP_System_GetDisplayWidth() / 2, CP_System_GetDisplayHeight() / 2);
	/*if(CP_Input_KeyTriggered(KEY_SPACE)){
		goGame();
	}*/
	CP_Image_Draw(background, M_width, M_height, CP_System_GetWindowWidth() * 4, CP_System_GetWindowHeight() * 3, 190);
	CP_Image_Draw(StartBut, M_width, M_height, buttonwidth, buttonheight, 255);
	CP_Image_Draw(HowBut, M_width, M_height * 5 / 4, buttonwidth, buttonheight, 255);
	CP_Image_Draw(EndBut, M_width, M_height * 6 / 4, buttonwidth, buttonheight, 255);
	CP_Image_Draw(Title, M_width - 20, M_height * 3 / 4, buttonwidth, buttonheight, 255);
	

	
}

void Main_Menu_Exit()
{

}