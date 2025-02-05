/*****************************************************************//**
 * \file   gunman.cpp
 * \brief  implementation file for the gunman class
 * 
 * \author raffa
 * \date   February 2025
 *********************************************************************/
#include "entities.h"


bool entities::gunman::operator==(const entities::entity& other) {
	if (typeid(*this) != typeid(other)) { return false; }
	const auto gunman_ptr = dynamic_cast<const entities::gunman*>(&other);
	if (gunman_ptr == nullptr) { return false; }
	return entities::entity::operator==(other) and health_ == gunman_ptr->get_health();
}
int entities::gunman::get_health() const {
	return health_;
}
int entities::gunman::get_direction() const {
	return direction_;
}
bool entities::gunman::update(std::vector<std::shared_ptr<entity>>& entities) { // make this a pointer
	return health_ > 0;
}
bool entities::gunman::collide(entities::entity& other) {
	/** collide with obstacle, blocks movement */
	auto obstacle = dynamic_cast<entities::obstacle*>(&other);
	if (obstacle != nullptr) {
		return false; // the gunman cannot move
	}
	/**  collide with pickup, pick it up if able to */
	auto pickup = dynamic_cast<entities::pickup*>(&other);
	if (pickup != nullptr) {
		/** TODO implement */
		return true; // the gunman can move
	}
	return true;
}
/**  returns boolean based on whether the gunman can move or not */
bool entities::gunman::move(Vector2& movement_vector, std::vector<std::shared_ptr<entities::entity>>& entities) {

	animation_.next_frame_loop();
	Vector2 new_pos = { position_.x + movement_vector.x, position_.y + movement_vector.y };
	/**  new proposed position  */
	Rectangle proposed_rect = get_rectangle();
	proposed_rect.x = new_pos.x;
	proposed_rect.y = new_pos.y;

	/**  check if  gunman movement is blocked by an obstacle */
	bool blocked = false;
	std::for_each(entities.begin(), entities.end(), [this, &blocked, &proposed_rect](std::shared_ptr<entities::entity>& e)
		{
			if (this != e.get() and CheckCollisionRecs(proposed_rect, e->get_rectangle())) {
				// if collide is false then do not move
				if (not this->collide(*e)) {
					blocked = true;
				}
			}
		});

	if (blocked) {
		return false;
	}
	/**  if not blocked by an obstacle, check that movement is not out of bounds  */
	if (new_pos.x >= config::PLAYABLE_X and new_pos.x + animation_.get_frame_width() <= config::PLAYABLE_WIDTH and
		new_pos.y >= config::PLAYABLE_Y and new_pos.y + animation_.get_frame_height() <= config::PLAYABLE_HEIGHT + config::PLAYABLE_Y) {
		position_ = new_pos;
		return true;
	}
	return false;
}
void entities::gunman::take_damage(int damage) {
	health_ -= damage;

}
void entities::gunman::reset(float x, float y) {
	/**  reset the position, health and animation of the gunman */
	position_.x = x;
	position_.y = y;
	health_ = config::GUNMAN_HEALTH;
	// reset the animation
	if (direction_ == 1) {
		animation_ = animation(config::P1_PATH, config::GUNMAN_WIDTH, config::GUNMAN_HEIGHT, config::GUNMAN_ANIMAITON_LENGTH, config::GUNMAN_ANIMATIONS);
	
	}
	else {
		animation_ = animation(config::P2_PATH, config::GUNMAN_WIDTH, config::GUNMAN_HEIGHT, config::GUNMAN_ANIMAITON_LENGTH, config::GUNMAN_ANIMATIONS);
	}
}