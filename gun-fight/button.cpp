#include "button.h"

void button::draw(Vector2& pos) {
	button_anim_.draw_frame(pos);
}

bool button::update(){
	if (is_pressed() or is_hovered()) {
		button_anim_.select_frame(1);
	}
	else {
		button_anim_.select_frame(0);
	}
	return is_pressed();
}

bool button::is_pressed(){
	auto mouse_pos = GetMousePosition();
	if (CheckCollisionPointRec(mouse_pos, button_rect_)) {
		if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
			return true;
		}
	}
	return false;
}

bool button::is_hovered(){
	auto mouse_pos = GetMousePosition();
	if (CheckCollisionPointRec(mouse_pos, button_rect_)) {
		return true;
	}
}
