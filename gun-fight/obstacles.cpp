/*****************************************************************//**
 * \file   obstacles.cpp
 * \brief  implementation file for obstacles
 * 
 * \author raffa
 * \date   February 2025
 *********************************************************************/
#include "entities.h"
bool entities::obstacle::operator==(const entities::entity& other) {
	return true;
}
int entities::obstacle::get_penetration() {
	return penetration_;
}
int entities::obstacle::get_category() {
	return obstacle_category_;
}
int entities::obstacle::get_health() {
	return health_;
}

// obstacle -- other behaviour

bool entities::obstacle::update(std::vector<std::shared_ptr<entity>>& entities) {
	//TODO implement
	// do a health check
	if (health_ <= 0) {
		remove_ = true;
	}
	return health_ > 0;
}
bool entities::obstacle::collide(entities::entity& other) {
	//TODO implement
	return true;
}
void entities::obstacle::take_damage(int damage) {
	health_ -= damage;
}

bool entities::moveable_obstacle::update(std::vector<std::shared_ptr<entity>>& entities) {
	bool alive = obstacle::update(entities);
	if (not alive) { return alive; }

	// then do movement
	move(entities);
	++frames_existed_;
	return true;
}

bool entities::moveable_obstacle::collide(entity& other) {
	// if collision, change direction 
	auto projectile_ptr = dynamic_cast<entities::projectile*>(&other);
	if (projectile_ptr != nullptr) { return true; }
	else {
		return false;
	}
}
/**  basic movement for obstacles, similar logic to gunman movement */
bool entities::moveable_obstacle::move(std::vector<std::shared_ptr<entity>>& entities) {
	Vector2 new_pos = { position_.x + movement_speed_.x, position_.y + movement_speed_.y };

	// Create a rectangle for the proposed new position
	Rectangle proposed_rect = get_rectangle();
	proposed_rect.x = new_pos.x;
	proposed_rect.y = new_pos.y;

	// TODO:: check players and entities
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
		change_direction();
	}
	// Check screen boundaries based on which half the gunman is in
	if (new_pos.x >= config::PLAYABLE_X and new_pos.x + animation_.get_frame_width() <= config::PLAYABLE_WIDTH and
		new_pos.y >= config::PLAYABLE_Y and new_pos.y + animation_.get_frame_height() <= config::PLAYABLE_HEIGHT) {
		position_ = new_pos;
		return true;
	}
	else {
		change_direction();
	}
	return false;
}
Vector2 entities::moveable_obstacle::get_speed() {
	return movement_speed_;
}
void entities::tumbleweed::change_direction() {
	movement_speed_.x *= -1;
	position_.x += movement_speed_.x;
}
/**  override tumbleweed movement so it can bounce, essentially sine movement function */
bool entities::tumbleweed::move(std::vector<std::shared_ptr<entity>>& entities) {
	auto new_y = abs(sin(frames_existed_ / 15)) * config::TUMBLEWEED_AMPLITUDE + baseline_; // add the baseline not the y
	Vector2 new_pos{ position_.x + movement_speed_.x, new_y };
	// Create a rectangle for the proposed new position
	Rectangle proposed_rect = get_rectangle();
	proposed_rect.x = new_pos.x;
	proposed_rect.y = new_pos.y;

	// Check if any obstacle interrupts at the new position
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
		change_direction();
	}
	// Check screen boundaries based on which half the gunman is in
	if (new_pos.x >= config::PLAYABLE_X and new_pos.x + animation_.get_frame_width() <= config::PLAYABLE_WIDTH and
		new_pos.y >= config::PLAYABLE_Y and new_pos.y + animation_.get_frame_height() <= config::PLAYABLE_HEIGHT) {
		position_ = new_pos;
		return true;
	}
	else {
		change_direction();
	}
	return false;
}

bool entities::tumbleweed::update(std::vector<std::shared_ptr<entity>>& entities) {
	// checks if alive, then moves 
	entities::moveable_obstacle::update(entities);
	if (frames_existed_ >= lifespan_) {
		remove_ = true;
	}
	return true;
}

void entities::tumbleweed::draw() {
	// update the frame and maybe animation
	animation_.next_frame();
	if (animation_.get_frame_num() == static_cast<int>(config::TUMBLEWEED_ANIMATION_LENGTH)) {
		animation_.reset_animation();
	}
	if (frames_existed_ == lifespan_ - config::TUMBLEWEED_ANIMATION_LENGTH) {
		animation_.next_animation();
	}
	entities::entity::draw();
}

void entities::wagon::draw() {
	animation_.draw_frame(position_);
	animation_.next_frame_loop();
}

void entities::wagon::change_direction() {
	movement_speed_.y *= -1;
	position_.y += movement_speed_.y;
	// moving down
	if (movement_speed_.y > 0) {
		animation_ = animation(config::WAGON_DOWN_PATH, config::WAGON_DOWN_WIDTH, config::WAGON_DOWN_HEIGHT, config::WAGON_ANIMATION_LENGTH, config::WAGON_ANIMATIONS);
	}
	// moveing up
	else if (movement_speed_.y < 0) {
		animation_ = animation(config::WAGON_UP_PATH, config::WAGON_UP_WIDTH, config::WAGON_UP_HEIGHT, config::WAGON_ANIMATION_LENGTH, config::WAGON_ANIMATIONS);

	}
}
void entities::cactus::take_damage(int damage) {
	obstacle::take_damage(damage);
	if (health_ > 0) {
		animation_.next_frame();
	}
}

void entities::barrel::take_damage(int damage) {
	obstacle::take_damage(damage);
	if (health_ > 0) {
		animation_.next_frame();
	}

}
