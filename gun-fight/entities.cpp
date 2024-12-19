#include "entities.h"

// entity - accessors
float entities::entity::get_x() {
	return position_.x;
}
float entities::entity::get_y() {
	return position_.y;
}

Vector2 entities::entity::get_position() {
	return position_;
}

int entities::entity::get_height() {
	return height_;
}

int entities::entity::get_width() {
	return width_;
}

const char* entities::entity::get_path() {
	return path_;
}

// entity - operator overloads
entities::entity& entities::entity::operator=(const entities::entity& other) {
	position_ = other.position_;
	height_ = other.height_;
	width_ = other.width_;
	path_ = other.path_;
	return *this;
}

// entity - other behaviours
void entities::entity::draw() {
	//TODO: implement
	// draw the loaded texture at the position
	DrawTexture(texture_, position_.x, position_.y, WHITE);
	return;
}


//gunman - accessors
wep::weapon* entities::gunman::get_weapon() {
	return gun_.get();
}

//gunman - inherited behaviour
int entities::gunman::get_health() {
	return health_;
}

void entities::gunman::update() {
	//TODO: implement
	return;
}
bool entities::gunman::collide(const entities::entity& other) {
	//TODO: implement
	return true;
}

//gunman - unique behaviour
bool entities::gunman::move(Vector2& movement_vector) {
	return true;
}

// obstacle - accessors
int entities::obstacle::get_health(){
	return health_;
}

// obstacle -- other behaviour

void entities::obstacle::update() {
	//TODO implement
	return;
}
bool entities::obstacle::collide(const entities::entity& other) {
	//TODO implement
	return true;
}
void entities::obstacle::die() {
	//TODO: implement
	return;
}
void entities::obstacle::take_damage(int& damage) {
	//TODO: implement

	// subtract damage from health
	// if healthj > 0, return true, obstacle is not dead
	// if health < 0, destroy object
	// at some point incorporate draw update to show object damage
	return;
}
// projectile - accessors
Vector2 entities::projectile::get_speed_direction(){
	return speed_direction_;
}

//projectile - other beahaviour
void entities::projectile::update() {
	//TODO implement
	return;
}
bool entities::projectile::collide(const entities::entity& other) {
	//TODO implement
	return true;

	
// pickup - other behaviour
}void entities::pickup::update() {
	//TODO implement
	return;
}
bool entities::pickup::collide(const entities::entity& other) {
	//TODO implement
	return true;
}
