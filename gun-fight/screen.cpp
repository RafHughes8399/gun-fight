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
	draw_strategy_->draw(background_, buttons_);
	EndDrawing();
}

void main_menu_strategy::draw(animation& background, std::vector<button>& buttons){
	//draw play, the x is the centre of the screen - half button width
	auto button_pos = Vector2{ config::SCREEN_WIDTH_HALF - (config::BUTTON_WIDTH / 2), config::BUTTONS_START_Y };
	for (auto i = 0; i < buttons.size(); ++i) {
		auto pos = buttons.at(i).get_position();
		buttons.at(i).draw(pos);
	}
}

std::unique_ptr<draw_strategy> main_menu_strategy::clone(){
	return std::make_unique<main_menu_strategy>(*this);
}

void return_strategy::draw(animation& background, std::vector<button>& buttons){
	auto button_pos = buttons.at(0).get_position();
	buttons.at(0).draw(button_pos);
}

std::unique_ptr<draw_strategy> return_strategy::clone(){
	return std::make_unique<return_strategy>(*this);
}
