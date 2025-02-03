#pragma once
#include "animation.h"
#include "raylib.h"
class button{
public:
	~button() = default;
	button(const char* path, float width, float height, float x, float y)
		: button_anim_(animation(path, width, height, 2, 1)), button_rect_(Rectangle{ x, y, width, height }) {}
	button(const button& other)
		: button_anim_(other.button_anim_), button_rect_(other.button_rect_) {}
	
	void draw(Vector2& pos);
	bool update();
	bool is_pressed();
	bool is_hovered();
private:
	animation button_anim_;
	Rectangle button_rect_;
};

