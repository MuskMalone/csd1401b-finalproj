#pragma once;
#include "utils.h"
#include "cprocessing.h"
typedef enum attack_type {range, melee} attack_type;
typedef struct Mob {
	attack_type type;
	Position pos;
}Mob;