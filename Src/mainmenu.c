#include "cprocessing.h"
#include "game.h"
#include "gametypes.h"
#include "gamestates.h"
#include "utils.h"
#include "camera.h"
#include "button.h"


#define SPLASH_SCREEN_DURATION 3.0f
float M_width;
float M_height;
CP_Image game_button_sprites[BUTTON_SPRITE_COUNT];
CP_Image tutorial[TUTORIAL_SPRITE_COUNT];
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

float splash_screen_timer = 0.0f;
float splash_screen_alpha = 0.0f;
CP_Image splash_screen_image;

int index = 0;

static Game_Button menu_buttons[3];
void goGame(void);
void turn_on_tut(void);
void turn_on_tut(void) {
	tutbool = 1;
}

void loadfile()
{
	xbut = CP_Image_Load("./Assets/xBut.png");
	background = CP_Image_Load("./Assets/main.png");
	Title = CP_Image_Load("./Assets/Title.png");
	splash_screen_image = CP_Image_Load("./Assets/DigiPen_Singapore_WEB_RED.png");
	menu_buttons[0].image = &(game_button_sprites[START_BUTTON]);
	menu_buttons[0].on_click_func = goGame;
	menu_buttons[2].image = &(game_button_sprites[EXIT_BUTTON]);//CP_Image_Load("./Assets/EXIT.png");
	menu_buttons[2].on_click_func = CP_Engine_Terminate;
	menu_buttons[1].image = &(game_button_sprites[TUTORIAL_BUTTON]);//CP_Image_Load("./Assets/HOWTOPLAY.png");
	menu_buttons[1].on_click_func = turn_on_tut;
	Tuts = CP_Image_Load("./Assets/TUTS.png");
	bgm = CP_Sound_Load("./Assets/SFX/MainMenu.ogg");
	for (int i = 0; i < 3; ++i) {
		menu_buttons[i].pos = (Position){
			M_width,
			(M_height * ((float)i + 5.0f)) / 4.0f
		};
		menu_buttons[i].size = (Position){ WALL_DIM * 10.0f, WALL_DIM * 2.0f };
		menu_buttons[i].scale = 1.0f;
		menu_buttons[i].timer = 0.0f;
	}
}

void goGame(void)
{
	CP_Engine_SetNextGameState(game_init, game_update, game_exit);
	CP_Engine_Run();
}

void Credit_screen(void)
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
	CP_System_SetWindowSize((int)W_width, (int)W_height);
	M_width = (float)CP_System_GetWindowWidth() / 2;
	M_height = (float)CP_System_GetWindowHeight() / 2;
	loadfile();
	CP_Sound_PlayMusic(bgm);

}

void Main_Menu_Update()
{
	CP_Graphics_ClearBackground(CP_Color_Create(0, 0, 0, 255));
	if (splash_screen_timer <= SPLASH_SCREEN_DURATION) {
		splash_screen_timer += CP_System_GetDt();
		splash_screen_alpha = 255.0f * (splash_screen_timer / SPLASH_SCREEN_DURATION);
		CP_Image_Draw(
			splash_screen_image,
			CP_System_GetWindowWidth() / 2.0f, CP_System_GetWindowHeight() / 2.0f,
			CP_System_GetWindowWidth() / 2.0f, CP_System_GetWindowHeight() / 5.0f,
			(int) splash_screen_alpha
		);
		return;
	}
	CP_Image_Draw(background, M_width, M_height, (float)CP_System_GetWindowWidth() * 4.0f, (float)CP_System_GetWindowHeight() * 3.0f, 190);
	CP_Image_Draw(Title, M_width - 20, M_height * 3 / 4, 1024, 512, 255);
	for (int i = 0; i < 3; i++)
	{
		update_button(&(menu_buttons[i]));
		draw_button(&(menu_buttons[i]));
	}
	
	if (CP_Input_KeyTriggered(KEY_A))
	{
		--index;
		if (index < 0)index = 4;

	}
	if (CP_Input_KeyTriggered(KEY_D))
	{
		++index;
		if (index > 4)index = 0;
	}
	if (tutbool == 1)
	{
		
		//index++;
		//if (index > 3)index = 0;
		CP_Image_Draw(tutorial[index], M_width, M_height, (float)CP_System_GetWindowWidth()*.8f, (float)CP_System_GetWindowHeight() * .8f, 255);
		CP_Image_Draw(xbut, M_width *3.15/2, M_height /10.0f, 75.0f, 75.0f, 255);
		if (CP_Input_MouseTriggered(MOUSE_BUTTON_1)) 
		{
			if (IsCircleClicked(M_width * 3.15f / 2.0f, M_height / 10, 50, CP_Input_GetMouseX(), CP_Input_GetMouseY()))
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