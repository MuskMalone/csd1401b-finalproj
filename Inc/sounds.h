#pragma once
#include "cprocessing.h"
#define SOUND_COUNT 5
typedef enum sound_idx { 
	EXPLOSION, 
	OOF, 
	RICOCHET, 
	SPLAT, 
	EXPLOSION2 
} sound_idx;
typedef struct Sound {
	int is_playing;
	CP_Sound sound;
} Sound;

void init_sounds(void);
void update_sounds(void);
void free_sounds(void);
void play_sound(sound_idx idx);