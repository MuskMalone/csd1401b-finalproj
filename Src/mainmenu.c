#include "cprocessing.h"
#include "game.h"
#include "gametypes.h"
#include "utils.h"

float M_width;
float M_height;

int tutbool = 0;
CP_Image background = NULL;
CP_Image Title = NULL;
CP_Image Tuts = NULL;
CP_Image xbut = NULL;

typedef struct Button
{
	float Posx;
	float Posy;
	float Width;
	float Height;
	CP_Image image;
}Button;

Button button[4];

void loadfile()
{
	xbut = CP_Image_Load("./Assets/xBut.png");
	background = CP_Image_Load("./Assets/main.png");
	Title = CP_Image_Load("./Assets/Title.png");
	button[0].image = CP_Image_Load("./Assets/startbutton.png");
	button[2].image = CP_Image_Load("./Assets/EXIT.png");
	button[1].image = CP_Image_Load("./Assets/HOWTOPLAY.png");
	Tuts = CP_Image_Load("./Assets/TUTS.png");
	button->Height = 104;
	button->Width = 512;
	button->Posx = M_width;
	button[0].Posy = M_height * 5 / 4;
	button[1].Posy = M_height * 6 / 4;
	button[2].Posy = M_height * 7 / 4;
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
	float W_width = WALL_DIM * GRID_COLS;
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
	float mousePosX = CP_Input_GetMouseX();
	float mousePosY = CP_Input_GetMouseY();

	

	CP_Graphics_ClearBackground(CP_Color_Create(0, 0, 0, 255));
	//CP_Font_DrawText("Main Menu Click SpaceBar to continue", CP_System_GetDisplayWidth() / 2, CP_System_GetDisplayHeight() / 2);
	/*if(CP_Input_KeyTriggered(KEY_SPACE)){
		goGame();
	}*/
	CP_Image_Draw(background, M_width, M_height, CP_System_GetWindowWidth() * 4, CP_System_GetWindowHeight() * 3, 190);
	CP_Image_Draw(Title, M_width - 20, M_height * 3 / 4, 1024, 512, 255);
	for (int i = 0; i < 4; i++)
	{
		CP_Image_Draw(button[i].image, button->Posx, button[i].Posy, button->Width, button->Height, 255);
	}

	if (CP_Input_MouseTriggered(MOUSE_BUTTON_1))
	{
		if (IsAreaClicked(button->Posx, button[0].Posy, button->Width / 2, button->Height / 2, mousePosX, mousePosY) == 1) {
			goGame();
		}
		if (IsAreaClicked(button->Posx, button[1].Posy, button->Width / 2, button->Height / 2, mousePosX, mousePosY) == 1) {
			//show tuts
			tutbool = 1;
		}
		if (IsAreaClicked(button->Posx, button[2].Posy, button->Width / 2, button->Height / 2, mousePosX, mousePosY) == 1) {
			CP_Engine_Terminate();
		}
	}

	if (tutbool == 1)
	{
		CP_Image_Draw(Tuts, M_width, M_height, 900, 1000, 255);
		CP_Image_Draw(xbut, M_width *3.15/2, M_height /10, 75, 75, 255);
		if (CP_Input_MouseTriggered(MOUSE_BUTTON_1)) 
		{
			if (IsCircleClicked(M_width * 3.15 / 2, M_height / 10, 50, mousePosX, mousePosY) == 1)
			{
				tutbool = 0;
			}
		}
	}
}

void Main_Menu_Exit()
{

}