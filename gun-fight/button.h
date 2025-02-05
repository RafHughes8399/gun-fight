/*****************************************************************//**
 * \file   button.h
 * \brief  header file for button class
 * 
 * \author raffa
 * \date   February 2025
 *********************************************************************/
#pragma once
#include "animation.h"
#include "raylib.h"
class button{
public:
	/**.constructors and destructors*/
	~button() = default;
	button(const char* path, float width, float height, float x, float y)
		: button_anim_(animation(path, width, height, 2, 1)), button_rect_(Rectangle{ x, y, width, height }), position_({ x,y }) {}
	button(const button& other)
		: button_anim_(other.button_anim_), button_rect_(other.button_rect_), position_(other.position_) {}
	
	/** methods */
	void draw(Vector2& pos);
	bool update();
	bool is_pressed();
	bool is_hovered();
	Vector2 get_position();
private:
	animation button_anim_;
	Rectangle button_rect_;
	Vector2 position_;
};

