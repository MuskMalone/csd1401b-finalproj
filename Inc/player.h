#pragma once;
#include "utils.h"
#include "cprocessing.h"
typedef struct Player
{
	Position pos;
	float diameter;
	int speed;
	float parryrad;
} Player;

Player init_player(void);
void update_player(void);
