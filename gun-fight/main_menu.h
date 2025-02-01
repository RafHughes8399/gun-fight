#pragma once
#include "raylib.h"
#include "animation.h"
#include "config.h"
#include <vector>
#include <utility>
class main_menu{
public:
	~main_menu() = default;
	main_menu()
		:background_(animation(config::MENU_PATH, config::SCREEN_WIDTH, config::SCREEN_HEIGHT)),
		buttons_(animation(config::BUTTONS_PATH, config::BUTTON_WIDTH, config::BUTTON_HEIGHT, 3, 1)) {
		float y = config::BUTTONS_START_Y;
		for (auto i = 0; i < 3; i++) {
			button_rectangles_.push_back(Rectangle{ config::SCREEN_WIDTH_HALF - (config::BUTTON_WIDTH / 2), y, config::BUTTON_WIDTH, config::BUTTON_HEIGHT });
			y += config::BUTTON_HEIGHT + 50;
		}
	}
	main_menu(const main_menu& other)
		:background_(other.background_), buttons_(other.buttons_), button_rectangles_(other.button_rectangles_) {
	}
	int update();
	void draw();
	Rectangle get_play_rectangle();
	Rectangle get_quit_rectangle();
	Rectangle get_controls_rectangle();
private:
	animation background_;
	animation buttons_;
	std::vector<Rectangle> button_rectangles_;
};

