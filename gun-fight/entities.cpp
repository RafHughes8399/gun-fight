#include "entities.h"
#include <iostream>
#include <algorithm>
#include <cmath>
//--------------- ENTITY --------------------------------------------------------------------------------------------------------
float entities::entity::get_x() const {
	return position_.x;
}
float entities::entity::get_y() const{
	return position_.y;
}

// interpret spritesheets as a 2 dimensional array of images
// move between frames by adjusting the rectnagle to display them 
// goto a frame in the current animation row

// these would be called in update to simulate animation
void entities::entity::set_pos(float x, float y){
	position_ = Vector2{ x, y };
}

Vector2 entities::entity::get_position() {
	return position_;
}

Rectangle entities::entity::get_rectangle(){
	return Rectangle{ position_.x, position_.y, animation_.get_frame_width(), animation_.get_frame_height()};
}

animation entities::entity::get_animation(){
	return animation_;
}

void entities::entity::set_animation(animation anim){
	animation_ = anim;
}

const char* entities::entity::get_path() const {
	return path_;
}
bool entities::entity::get_remove() {
	return remove_;
}
void entities::entity::set_remove(bool b) {
	remove_ = b;
}
entities::entity& entities::entity::operator=(const entities::entity& other) {

	position_ = other.position_;
	path_ = other.path_;
	return *this;
}

void entities::entity::draw() {
	// TODO eventually chagne to animation
	animation_.draw_frame(position_);
}

bool entities::entity::operator==(const entities::entity& other) {
	return typeid(*this) == typeid(other) and position_.x == other.get_x() and position_.y == other.get_y();
}

bool entities::entity::operator<(entity& other){
	return Vector2Length(get_position()) < Vector2Length(other.get_position());
}


//--------------- GUNMAN --------------------------------------------------------------------------------------------------------

// ------------------------------OBSTACLE------------------------------------------------







