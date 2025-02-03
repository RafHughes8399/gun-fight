#include "control_screen.h"

int control_screen::update(){
	auto mouse_pos = GetMousePosition();
	// check play button is pressed
	if (return_.update()) {
		return 1;
	}
	return -1;
}
void control_screen::draw() {
	BeginDrawing();
	auto pos = Vector2{ 0,0 };
	background_.draw_frame(pos);
	pos = { 10.0, 10.0 };
	return_.draw(pos);
	EndDrawing();
}
