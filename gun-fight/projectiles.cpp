#include "entities.h"
bool entities::projectile::operator==(const entities::entity& other) {
	if (typeid(*this) != typeid(other)) { return false; }
	const auto projectile_ptr = dynamic_cast<const entities::projectile*>(&other);
	if (projectile_ptr == nullptr) { return false; }
	return entities::entity::operator==(other)
		and speed_direction_.x == projectile_ptr->get_speed_direction().x and speed_direction_.y == projectile_ptr->get_speed_direction().y;
}
// projectile - accessors
Vector2 entities::projectile::get_speed_direction() const {
	return speed_direction_;
}
int entities::projectile::get_damage() {
	return damage_;
}
int entities::projectile::get_penetration() {
	return penetration_;
}
//projectile - other beahaviour
bool entities::projectile::penetrate(const int& obstacle_penetration) {
	return penetration_ >= obstacle_penetration;
}

bool entities::projectile::update(std::vector<std::shared_ptr<entity>>& entities) {
	// TODO collision both players and entities
	for (auto& e : entities) {
		if (CheckCollisionRecs(get_rectangle(), e->get_rectangle()) and this != e.get()) {
			if (not collide(*e)) {
				remove_ = true;
				return false;
			}
		}
	}
	// then check the gunmen
	position_.x += (speed_direction_.x * speed_direction_.y);
	if (position_.x < config::PLAYABLE_X or position_.x > config::PLAYABLE_WIDTH) {
		remove_ = true;
		return false; // will remove if out of bounds 
	}
	return true;
}
bool entities::projectile::collide(entities::entity& other) {
	// if gunman
	auto gunman = dynamic_cast<entities::gunman*>(&other);
	if (gunman != nullptr and gunman->get_direction() != speed_direction_.y) {
		gunman->take_damage(damage_);
		return false; // cannot move
	}
	// if obstacle
	auto obstacle = dynamic_cast<entities::obstacle*>(&other);
	if (obstacle != nullptr) {
		obstacle->take_damage(damage_);
		// check penetration for tumbleweeds
		auto tumbleweed = dynamic_cast<entities::tumbleweed*>(&other);
		if (tumbleweed == nullptr) {
			Sound s = LoadSound(config::BULLET_HIT_SOUND);
			PlaySound(s);
		}
		return penetrate(obstacle->get_penetration()); // a revolver can penetrate a tumbleweed but not a cactus
	}
	return true;
}
// --------- BULLET ----------------

bool entities::bullet::operator==(const entities::entity& other) {
	if (typeid(*this) != typeid(other)) { return false; }
	const auto bullet_ptr = dynamic_cast<const entities::bullet*>(&other);
	if (bullet_ptr == nullptr) { return false; }
	return entities::projectile::operator==(other);
}