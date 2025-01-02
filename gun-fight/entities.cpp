#include "entities.h"
#include <iostream>
#include <algorithm>
//--------------- ENTITY --------------------------------------------------------------------------------------------------------
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
	DrawTexture(texture_, position_.x, position_.y, WHITE);
}

bool entities::entity::operator==(const entities::entity& other) {
	return typeid(*this) == typeid(other) and position_.x == other.get_x() and position_.y == other.get_y();
}

auto entities::entity::operator<=>(entity& other){
	return Vector2Length(get_position()) <=> Vector2Length(other.get_position());
}

//--------------- GUNMAN --------------------------------------------------------------------------------------------------------
bool entities::gunman::operator==(const entities::entity& other) {
	if (typeid(*this) != typeid(other)) { return false; }
	const auto gunman_ptr = dynamic_cast<const entities::gunman*>(&other);
	if (gunman_ptr == nullptr) { return false; }
	return entities::entity::operator==(other)
		and gun_.get() == gunman_ptr->get_weapon() and health_ == gunman_ptr->get_health();
}
// accessors
wep::weapon* entities::gunman::get_weapon() const {
	return gun_.get();
}

int entities::gunman::get_health() const {
	return health_;
}
int entities::gunman::get_score() const {
	return score_;
}
// overloaded behaviour
bool entities::gunman::update(std::vector<std::unique_ptr<entity>>& entities) { // make this a pointer
	// TODO: check collisions with obstacles and pickup items pending implementation, and projectiles, 
	// check collision
	// then check health of the gunman 
	if (health_ <= 0) { return false;} // indicates dead gunman
	// then check movment key input
	std::for_each(movement_.begin(), movement_.end(), [this, &entities](auto& key_direction) {
		if (IsKeyDown(key_direction.first)) {
			this->move(key_direction.second, entities);
			return true;
		}
		return false; // no movement key was pressed
		});

	// then check firing and reloading key inputs
	if (IsKeyPressed(fire_reload_.first) and std::none_of(movement_.begin(), movement_.end(),[](auto& key_direction) {
		return IsKeyDown(key_direction.first);
	})){
		if (gun_->fire()) {
			if (position_.x < config::SCREEN_WIDTH_HALF) {
				entities.push_back(std::make_unique<entities::bullet>(entities::bullet(position_.x + 100, position_.y + 45, "sprites/bullet-1.png", 1,gun_.get())));
			}
			else if (position_.x > config::SCREEN_WIDTH_HALF) {
				entities.push_back(std::make_unique<entities::bullet>(entities::bullet(position_.x- 20, position_.y + 45, "sprites/bullet-2.png", -1, gun_.get())));
			}
		}
	}
	if (IsKeyPressed(fire_reload_.second)) {
		gun_->reload();
	}
	return true;
}
bool entities::gunman::collide(entities::entity& other) {
	// collides with obstacle
	auto obstacle = dynamic_cast<entities::obstacle*>(&other);
	if (obstacle != nullptr) {
		// block movement
		return false; // the gunman cannot move
	}
	auto pickup = dynamic_cast<entities::pickup*>(&other);
	if (pickup != nullptr) {
		//wrap the gunman with the item 
		return true; // the gunman can move
	}
	// collides with pickup
	return true;
}
// unique behaviour
bool entities::gunman::move(Vector2& movement_vector, std::vector<std::unique_ptr<entities::entity>>& entities) {

	// check if the gunman would collide with anything at the x 
	// 
	// check if the gunman would collide with anything at the y
	// check which half the gunman is in
	Vector2 new_pos = { position_.x + movement_vector.x, position_.y + movement_vector.y };
	// check if any obstacle interrupts, if so, prevent the movement
	bool blocked = false;
	std::for_each(entities.begin(), entities.end(), [this, &blocked](std::unique_ptr<entities::entity>& e)
		{
			if (CheckCollisionRecs(this->get_rectangle(), e->get_rectangle()) and this != e.get()) {
				// if collide is false then do not move
				if (not this->collide(*e)) {
					blocked = true;
				}
			}
		});
	if (blocked) { return false; }
	if (position_.x >= 0 and position_.x <= config::SCREEN_WIDTH_HALF) {
		if (new_pos.x >= 0 and new_pos.x + texture_.width <= config::SCREEN_WIDTH_HALF) {
			if (new_pos.y >= 0 and new_pos.y + texture_.height <= config::SCREEN_HEIGHT) {
				position_ = new_pos;
				return true;
			}
		}	
	}
	// there is a bug where if the second gunman hits the halfway point it gets stuck
	else if (position_.x >= config::SCREEN_WIDTH_HALF and position_.x <= config::SCREEN_WIDTH) {
		if (new_pos.x >= config::SCREEN_WIDTH_HALF and new_pos.x + texture_.width <= config::SCREEN_WIDTH) {
			if (new_pos.y >= 0 and new_pos.y + texture_.height <= config::SCREEN_HEIGHT) {
				position_ = new_pos;
				return true;
			}
		}
	}
	return false;
}

void entities::gunman::take_damage(int damage) {
	// return true if still alive, return false if not
	health_ -= damage;
}
void entities::gunman::win_point() {
	++score_;
}
void entities::gunman::reset(float x, float y) {
	position_.x = x;
	position_.y = y;
	health_ = config::GUNMAN_HEALTH;
	gun_->replenish();
}
// ------------------------------OBSTACLE------------------------------------------------
bool entities::obstacle::operator==(const entities::entity& other) {
	return true;
}
int entities::obstacle::get_health(){
	return health_;
}

// obstacle -- other behaviour

bool entities::obstacle::update(std::vector<std::unique_ptr<entity>>& entities) {
	//TODO implement
	// do a health check
	return health_ > 0;
}
bool entities::obstacle::collide(entities::entity& other) {
	//TODO implement
	return true;
}
void entities::obstacle::die() {
	//TODO: implement
	return;
}
void entities::obstacle::take_damage(int damage) {
	health_ -= damage;
}

bool entities::projectile::operator==(const entities::entity& other) {
	if (typeid(*this) != typeid(other)) { return false; }
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
bool entities::projectile::update(std::vector<std::unique_ptr<entity>>& entities) {
	//TODO collision with obstacles

	// check colliding with gunman
	for (auto& e : entities) {
		if(CheckCollisionRecs(get_rectangle(), e->get_rectangle()) and this != e.get())
		if (not collide(*e)) {
			remove_ = true;
			return false;
		}
	}
	// then check the gunmen
	position_.x += (speed_direction_.x * speed_direction_.y);
	if (position_.x < 0 or position_.x > config::SCREEN_WIDTH) {
		return false; // will remove if out of bounds 
	}
	return true;
}
bool entities::projectile::collide(entities::entity& other) {
	// if gunman
	auto gunman = dynamic_cast<entities::gunman*>(&other);
	if (gunman != nullptr) {
		gunman->take_damage(weapon_->get_damage());
		return false; // cannot move
	}
	// if obstacle
	auto obstacle = dynamic_cast<entities::obstacle*>(&other);
	// otherwise do nothing 
	return true;
}
// --------- BULLET ----------------

bool entities::bullet::operator==(const entities::entity& other) {
	if (typeid(*this) != typeid(other)) { return false; }
	const auto bullet_ptr = dynamic_cast<const entities::bullet*>(&other);
	if (bullet_ptr == nullptr) { return false; }
	return entities::projectile::operator==(other);
}

bool entities::pickup::operator==(const entities::entity& other) {
	return true;
}
// pickup - other behaviour
bool entities::pickup::update(std::vector<std::unique_ptr<entity>>& entities) {
	//TODO implement
	return true;
}
bool entities::pickup::collide(entities::entity& other) {
	//TODO implement
	return true;
}
