#pragma once
#include "raylib.h"
#include "animation.h"
#include "config.h"
#include "button.h"
#include <vector>
#include <utility>
class main_menu{
public:
	~main_menu() = default;
	main_menu()
		:background_(animation(config::MENU_PATH, config::SCREEN_WIDTH, config::SCREEN_HEIGHT)), buttons_() {

		// play
		auto y = config::BUTTONS_START_Y;
		buttons_.push_back(button(config::PLAY_PATH, config::BUTTON_WIDTH, config::BUTTON_HEIGHT, config::SCREEN_WIDTH_HALF - (config::BUTTON_WIDTH / 2), y));
		// controls
		y += config::BUTTON_HEIGHT + 50;
		buttons_.push_back(button(config::CONTROLS_PATH, config::BUTTON_WIDTH, config::BUTTON_HEIGHT, config::SCREEN_WIDTH_HALF - (config::BUTTON_WIDTH / 2), y));	// controls
		y += config::BUTTON_HEIGHT + 50;
		buttons_.push_back(button(config::QUIT_PATH, config::BUTTON_WIDTH, config::BUTTON_HEIGHT, config::SCREEN_WIDTH_HALF - (config::BUTTON_WIDTH / 2), y));
		// quit
	}
	main_menu(const main_menu& other)
		:background_(other.background_), buttons_(other.buttons_) {
	}
	int update();
	void draw();
private:
	animation background_;
	std::vector<button> buttons_;
};

