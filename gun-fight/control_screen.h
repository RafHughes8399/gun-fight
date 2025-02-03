#pragma once
#include "animation.h"
#include "button.h"
#include "config.h"
class control_screen{
public:
	~control_screen() = default;
	control_screen()
		: background_(config::CONTROL_SCREEN_PATH, config::SCREEN_WIDTH, config::SCREEN_HEIGHT),
		return_(config::RETURN_PATH, config::RETURN_WIDTH, config::RETURN_HEIGHT, 10, 10) {};
	control_screen(const control_screen& other)
		: background_(other.background_), return_(other.return_) {};

	void draw();
	int update();
private:
	animation background_;
	button return_;
};

