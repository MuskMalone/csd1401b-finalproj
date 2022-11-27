/*
@file sounds.c
@author Chun Ho Kok
@course CSD1401
@section B
@brief plays sounds once when play_sound is called
*/
/*______________________________________________________________________*/
#include "sounds.h"

static Sound sounds[SOUND_COUNT];
static int is_loaded = 0;

void init_sounds(void) {
	if (is_loaded) return;
	sounds[EXPLOSION] = (Sound){ .is_playing = 0, .sound = CP_Sound_Load("./Assets/SFX/explosion.ogg") };
	sounds[EXPLOSION2] = (Sound){ .is_playing = 0, .sound = CP_Sound_Load("./Assets/SFX/explosion2.ogg") };
	sounds[OOF] = (Sound){ .is_playing = 0, .sound = CP_Sound_Load("./Assets/SFX/oof.ogg") };
	sounds[RICOCHET] = (Sound){ .is_playing = 0, .sound = CP_Sound_Load("./Assets/SFX/ricochet.ogg") };
	sounds[SPLAT] = (Sound){ .is_playing = 0, .sound = CP_Sound_Load("./Assets/SFX/splat.ogg") };
	is_loaded = 1;
}
void update_sounds(void) {
	for (int i = 0; i < SOUND_COUNT; ++i) {
		if (sounds[i].is_playing) {
			CP_Sound_Play(sounds[i].sound);
			sounds[i].is_playing = 0;
		}
	}
}
void free_sounds(void) {
	for (int i = 0; i < SOUND_COUNT; ++i) {
		CP_Sound_Free(&(sounds[i].sound));
	}
}
void play_sound(sound_idx idx) {
	sounds[idx].is_playing = 1;
}