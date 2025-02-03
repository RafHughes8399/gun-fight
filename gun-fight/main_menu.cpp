#include "main_menu.h"

int main_menu::update(){
	// check buttons are pressed
	// returns int based on button pressed, -1 means nothing is pressed
	auto mouse_pos = GetMousePosition();
	// check play button is pressed
	for (auto i = 0; i < 3; ++i) {
		if (buttons_.at(i).update()) {
			return i;
		}
	}
	return -1;

}

void main_menu::draw(){
	BeginDrawing();
	auto background_pos = Vector2 { 0,0 };
	background_.draw_frame(background_pos);
	//draw play, the x is the centre of the screen - half button width
	auto button_pos = Vector2{ config::SCREEN_WIDTH_HALF - (config::BUTTON_WIDTH / 2), config::BUTTONS_START_Y };
	for (auto i = 0; i < 3; ++i) {
		buttons_.at(i).draw(button_pos);
		button_pos.y += config::BUTTON_HEIGHT + 50;
	}
	EndDrawing();
}
