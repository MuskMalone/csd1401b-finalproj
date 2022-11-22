#include "cprocessing.h"
#include "game.h"
#include "gametypes.h"
#include "gamestates.h"
#include "utils.h"
#include "camera.h"
#include "button.h"

float M_width;
float M_height;
CP_Image game_button_sprites[BUTTON_SPRITE_COUNT];
int tutbool = 0;
CP_Image background = NULL;
CP_Image Title = NULL;
CP_Image Tuts = NULL;
CP_Image xbut = NULL;
CP_Sound bgm;
//typedef struct Button
//{
//	float Posx;
//	float Posy;
//	float Width;
//	float Height;
//	CP_Image image;
//}Button;

static Game_Button menu_buttons[3];

void loadfile()
{
	xbut = CP_Image_Load("./Assets/xBut.png");
	background = CP_Image_Load("./Assets/main.png");
	Title = CP_Image_Load("./Assets/Title.png");
	menu_buttons[0].image = &(game_button_sprites[START_BUTTON]);
	menu_buttons[2].image = &(game_button_sprites[EXIT_BUTTON]);//CP_Image_Load("./Assets/EXIT.png");
	menu_buttons[1].image = &(game_button_sprites[TUTORIAL_BUTTON]);//CP_Image_Load("./Assets/HOWTOPLAY.png");
	Tuts = CP_Image_Load("./Assets/TUTS.png");
	bgm = CP_Sound_Load("./Assets/SFX/MainMenu.wav");
	for (int i = 0; i < 3; ++i) {
		menu_buttons[i].pos = (Position){
			M_width,
			(M_height * ((float)i + 5.0f)) / 4.0f
		};
		menu_buttons[i].size = (Position){ 512.0f, 104.0f };
		menu_buttons[i].scale = 1.0f;
		menu_buttons[i].timer = 0.0f;
		menu_buttons[i].mouse_on_button = 0;
	}
}

void goGame()
{
	CP_Engine_SetNextGameState(game_init, game_update, game_exit);
	CP_Engine_Run();
}

void Credit_screen()
{

}

void Main_Menu_Init()
{
	init_sprites();
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
	CP_Sound_PlayMusic(bgm);

}

void Main_Menu_Update()
{

	float mousePosX = CP_Input_GetMouseX();
	float mousePosY = CP_Input_GetMouseY();

	
	
	CP_Graphics_ClearBackground(CP_Color_Create(0, 0, 0, 255));
	CP_Image_Draw(background, M_width, M_height, CP_System_GetWindowWidth() * 4, CP_System_GetWindowHeight() * 3, 190);
	CP_Image_Draw(Title, M_width - 20, M_height * 3 / 4, 1024, 512, 255);
	for (int i = 0; i < 3; i++)
	{
		update_button(&(menu_buttons[i]));
		draw_button(&(menu_buttons[i]));
	}

	if (CP_Input_MouseTriggered(MOUSE_BUTTON_1))
	{
		if (is_mouse_on(&(menu_buttons[0]))) {
			goGame();
		}
		if (is_mouse_on(&(menu_buttons[1]))) {
			//show tuts
			tutbool = 1;
		}
		if (is_mouse_on(&(menu_buttons[2]))) {
			CP_Engine_Terminate();
		}
	}

	if (tutbool == 1)
	{
		CP_Image_Draw(Tuts, M_width, M_height, 900, CP_System_GetWindowHeight(), 255);
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
	//CP_Sound_PauseAll();
}