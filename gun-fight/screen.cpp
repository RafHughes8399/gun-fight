#include "screen.h"

int screen::update(){
	for (auto i = 0; i < buttons_.size(); ++i) {
		if (buttons_.at(i).update()) {
			return i;
		}
	}
	return -1;

}

void screen::draw(){
	BeginDrawing();
	auto background_pos = Vector2{ 0,0 };
	background_.draw_frame(background_pos);
	//draw play, the x is the centre of the screen - half button width
	auto button_pos = Vector2{ config::SCREEN_WIDTH_HALF - (config::BUTTON_WIDTH / 2), config::BUTTONS_START_Y };
	for (auto i = 0; i < buttons_.size(); ++i) {
		buttons_.at(i).draw(button_pos);
		button_pos.y += config::BUTTON_HEIGHT + 50;
	}
	EndDrawing();
}
