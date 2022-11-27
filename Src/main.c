/*
@file main.c
@author Tan Cheng Hian, Han Qin Ding, Vanessa Chua, Shannon Teo, Chun Ho Kok
@course CSD1401
@section B
@brief starting point for the game
*/
/*______________________________________________________________________*/
#include "cprocessing.h"
#include "gamestates.h"



int main(void)
{
	CP_System_SetFrameRate(60);
	CP_Engine_SetNextGameState(Main_Menu_Init, Main_Menu_Update, Main_Menu_Exit);
	CP_Engine_Run();

	return 0;
}