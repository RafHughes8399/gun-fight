#pragma once
#include "raylib.h"
#include "animation.h"
#include "config.h"
#include "button.h"
#include <vector>
#include <utility>

//TODO implement draw strategies
class draw_strategy {
public:
private:

};

class main_menu_strategy {
public:
private:
};

class control_screen_strategy {
public:
private:

};
class screen{
	public:
		~screen() = default;
		template <typename InputIt>
		screen(const char* path, float width, float height, int anim_length, int anims, int button_offset, InputIt first, InputIt last)
			:background_(animation(path, width, height, anim_length, anims)), buttons_(first, last), button_offset_(button_offset) {
		}
		screen(const screen& other)
			:background_(other.background_), buttons_(other.buttons_), button_offset_(other.button_offset_){
		}
		int update();
		void draw();
	private:
		animation background_;
		std::vector<button> buttons_;
		int button_offset_;
		draw_strategy draw_strategy_;
};

