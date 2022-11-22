#include "button.h"
#include "easing.h"
#define BUTTON_MAX_TIMER .5f
#define BUTTON_MAX_SCALE 1.05f

void update_button(Game_Button* button) {
	if (IsAreaClicked(
		button->pos.x, button->pos.y,
		button->size.x / 2.0f, button->size.y / 2.0f,
		CP_Input_GetMouseX(), CP_Input_GetMouseY()
	)){
		if (button->timer < BUTTON_MAX_TIMER)
			button->timer += CP_System_GetDt();
		else button->timer = BUTTON_MAX_TIMER;
		button->scale = EaseOutExpo(1.0f, BUTTON_MAX_SCALE, button->timer / BUTTON_MAX_TIMER);
		if (CP_Input_MouseTriggered(MOUSE_BUTTON_1)) {
			button->on_click_func();
		}
	}
	else {
		if (button->timer > 0.0f) 
			button->timer -= CP_System_GetDt();
		else 
			button->timer = 0.0f;
		button->scale = EaseInExpo(1.0f, BUTTON_MAX_SCALE, button->timer / BUTTON_MAX_TIMER);
	}
	
}
int is_mouse_on(Game_Button* button) {
	return IsAreaClicked(
		button->pos.x, button->pos.y,
		button->size.x / 2.0f, button->size.y / 2.0f,
		CP_Input_GetMouseX(), CP_Input_GetMouseY()
	);
}
void draw_button(Game_Button* button) {
	CP_Image_Draw(*(button->image), 
		button->pos.x, button->pos.y, 
		button->size.x * button->scale, button->size.y * button->scale, 
		255
	);
}