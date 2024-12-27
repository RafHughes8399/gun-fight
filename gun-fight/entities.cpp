#include "entities.h"
#include <iostream>
#include <algorithm>
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
int entities::gunman::get_score() const {
	return score_;
}
bool entities::gunman::update(std::vector<std::unique_ptr<entities::entity>>& entities) { // make this a pointer
	std::for_each(movement_.begin(), movement_.end(), [this](auto& key_direction) {
		if (IsKeyDown(key_direction.first)) {
			move(key_direction.second);
			return true;
		}
		return false;
		});

	// next do firing
	if (IsKeyPressed(fire_reload_.first) and std::none_of(movement_.begin(), movement_.end(),[](auto& key_direction) {
		return IsKeyDown(key_direction.first);
	})){
		//TODO gun firing is amiss
		if (gun_->fire()) {
			if (position_.x < config::SCREEN_WIDTH_HALF) {
				entities.push_back(std::make_unique<entities::bullet>(entities::bullet(position_.x + 95, position_.y + 45, "sprites/bullet-1.png", 1,gun_.get())));
			}
			else if (position_.x > config::SCREEN_WIDTH_HALF) {
				entities.push_back(std::make_unique<entities::bullet>(entities::bullet(position_.x- 20, position_.y + 45, "sprites/bullet-2.png", -1, gun_.get())));
			}
		}
	}
	if (IsKeyPressed(fire_reload_.second)) {
		gun_->reload();
	}
	// switch the health check to here
	return true;
}
bool entities::gunman::collide(entities::entity& other) {
	//TODO: implement, walks into 
	return true;
}

//gunman - unique behaviour
bool entities::gunman::move(Vector2& movement_vector) {
	// check which half the gunman is in
	Vector2 new_pos = { position_.x + movement_vector.x, position_.y + movement_vector.y };
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

// obstacle - accessors

bool entities::obstacle::operator==(const entities::entity& other) {
	return true;
}
int entities::obstacle::get_health(){
	return health_;
}

// obstacle -- other behaviour

bool entities::obstacle::update(std::vector<std::unique_ptr<entities::entity>>& entities) {
	//TODO implement
	// do a health check
	return health_ > 0;
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
void entities::obstacle::take_damage(int& damage) {
	health_ -= damage;
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
bool entities::projectile::update(std::vector<std::unique_ptr<entities::entity>>& entities) {
	//TODO implement
	// check collision 
	for (auto& e : entities) {
		// check collision
		if (collide(*e)) {
			return false; // will destroy the projectile
		}
		// will remove if collided
	}
	// then move the projctile
	position_.x += (speed_direction_.x * speed_direction_.y);
	if (position_.x < 0 or position_.x > config::SCREEN_WIDTH) {
		return false; // will remove if out of bounds 
	}
	return true;
}
bool entities::projectile::collide(entities::entity& other) {
	//TODO implement
	// if gunman or obstacle, damage it 

	// if pickup ignore 

	return true;
}

// --------- BULLET ----------------

bool entities::bullet::operator==(const entities::entity& other) {
	const auto bullet_ptr = dynamic_cast<const entities::bullet*>(&other);
	if (bullet_ptr == nullptr) { return false; }
	return entities::projectile::operator==(other);
}
bool entities::bullet::update(std::vector<std::unique_ptr<entities::entity>>& entities) {
	// move the bullet, check if out of bounds
	return projectile::update(entities);
}
bool entities::bullet::collide(entities::entity& other) {
	//// true if still alive, false if not
	//entities::gunman* gunman = dynamic_cast<entities::gunman*> (&other);
	//if (gunman != nullptr) {
	//	auto dmg = weapon_->get_damage();
	//	return gunman->take_damage(dmg);
	//}

	//entities::obstacle* obstacle = dynamic_cast<entities::obstacle*>(&other);
	//if (obstacle != nullptr) {
	//	auto dmg = weapon_->get_damage();
	//	obstacle->take_damage(dmg);
	//}
	//// if obstacle, decrease health of obstacle
	//return true; // return true if still alive, return false if not alive
	return true;
}

bool entities::pickup::operator==(const entities::entity& other) {
	return true;
}
// pickup - other behaviour
bool entities::pickup::update(std::vector<std::unique_ptr<entities::entity>>& entities) {
	//TODO implement
	return true;
}
bool entities::pickup::collide(entities::entity& other) {
	//TODO implement
	return true;
}
