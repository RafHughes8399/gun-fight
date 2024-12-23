#include "entities.h"
#include <iostream>
// entity - accessors
float entities::entity::get_x() const {
	return position_.x;
}
float entities::entity::get_y() const{
	return position_.y;
}

Vector2 entities::entity::get_position() {
	return position_;
}

Rectangle entities::entity::get_rectangle(){
	return Rectangle{ position_.x, position_.y, static_cast<float>(texture_.width), static_cast<float>(texture_.height) };
}

const char* entities::entity::get_path() const {
	return path_;
}

// entity - operator overloads
entities::entity& entities::entity::operator=(const entities::entity& other) {
	position_ = other.position_;
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
//TODO implement
bool entities::entity::operator==(const entities::entity& other) {
	return position_.x == other.get_x() and position_.y == other.get_y() and
		path_ == other.get_path();
}

//--------------- GUNMAN ------------------------
bool entities::gunman::operator==(const entities::entity& other) {
	const auto gunman_ptr = dynamic_cast<const entities::gunman*>(&other);
	if (gunman_ptr == nullptr) { return false; }
	return entities::entity::operator==(other)
		and gun_.get() == gunman_ptr->get_weapon() and health_ == gunman_ptr->get_health();
}

bool entities::gunman::take_damage(int& damage) {
	// return true if still alive, return false if not
	health_ -= damage;
	return (health_ > 0);
}
wep::weapon* entities::gunman::get_weapon() const {
	return gun_.get();
}

int entities::gunman::get_health() const {
	return health_;
}

bool entities::gunman::update() {
	//TODO: implement

	// changes health
	// changes textures too, for animation, or that would be draw
	// do a health check
	return true;
}
bool entities::gunman::collide(entities::entity& other) {
	//TODO: implement
	return true;
}

//gunman - unique behaviour
bool entities::gunman::move(Vector2& movement_vector, const int& screen_width, const int& screen_height) {
	// check that the new position is in bounds and does not cross the halfway point, do this 
	Vector2 new_pos = { position_.x + movement_vector.x, position_.y + movement_vector.y };
	// check does not exceed screen width 
	if (new_pos.x >= 0 and new_pos.x + texture_.width <= screen_width) {
		if (new_pos.y >= 0 and new_pos.y + texture_.height <= screen_height) {
			position_ = new_pos;
			return true;
		}
	}
	return false;
}

// obstacle - accessors

bool entities::obstacle::operator==(const entities::entity& other) {
	return true;
}
int entities::obstacle::get_health(){
	return health_;
}

// obstacle -- other behaviour

bool entities::obstacle::update() {
	//TODO implement
	// do a health chek
	return true;
}
bool entities::obstacle::collide(entities::entity& other) {
	//TODO implement
	return true;
}
void entities::obstacle::die() {
	//TODO: implement
	return;
}
bool entities::obstacle::take_damage(int& damage) {
	health_ -= damage;
	return health_ > 0;
}

bool entities::projectile::operator==(const entities::entity& other) {
	const auto projectile_ptr = dynamic_cast<const entities::projectile*>(&other);
	if (projectile_ptr == nullptr) { return false; }
	return entities::entity::operator==(other)
		and speed_direction_.x == projectile_ptr->get_speed_direction().x and speed_direction_.y == projectile_ptr->get_speed_direction().y
		and weapon_ == projectile_ptr->get_weapon();
}
// projectile - accessors
Vector2 entities::projectile::get_speed_direction() const{
	return speed_direction_;
}
wep::weapon* entities::projectile::get_weapon() const {
	return weapon_;
}
//projectile - other beahaviour
bool entities::projectile::update() {
	//TODO implement
	return true;
}
bool entities::projectile::collide(entities::entity& other) {
	//TODO implement
	return true;
}

// --------- BULLET ----------------

bool entities::bullet::operator==(const entities::entity& other) {
	const auto bullet_ptr = dynamic_cast<const entities::bullet*>(&other);
	if (bullet_ptr == nullptr) { return false; }
	return entities::projectile::operator==(other);
}
bool entities::bullet::update() {
	// move the bullet, check if out of bounds
	position_.x += (speed_direction_.x * speed_direction_.y);
	if (position_.x < 0 or position_.x > config::SCREEN_WIDTH) {
		return false;
	}
	return true;

}
bool entities::bullet::collide(entities::entity& other) {
	// true if still alive, false if not
	entities::gunman* gunman = dynamic_cast<entities::gunman*> (&other);
	if (gunman != nullptr) {
		auto dmg = weapon_->get_damage();
		return gunman->take_damage(dmg);
	}

	entities::obstacle* obstacle = dynamic_cast<entities::obstacle*>(&other);
	if (obstacle != nullptr) {
		auto dmg = weapon_->get_damage();
		return obstacle->take_damage(dmg);
	}
	// if obstacle, decrease health of obstacle
	return true; // return true if still alive, return false if not alive
}

bool entities::pickup::operator==(const entities::entity& other) {
	return true;
}
// pickup - other behaviour
bool entities::pickup::update() {
	//TODO implement
	return true;
}
bool entities::pickup::collide(entities::entity& other) {
	//TODO implement
	return true;
}
