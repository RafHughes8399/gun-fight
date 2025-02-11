#include "player.h"

player& player::operator=(const player& other){
	gunman_ = other.gunman_;
	weapon_ = other.weapon_;
	item_ = other.item_;
	movement_ = other.movement_;
	fire_reload_ = other.fire_reload_;
	player_start_pos_ = other.player_start_pos_;
	score_ = other.score_;
	return *this;
}

std::shared_ptr<entities::gunman> player::get_gunman(){
	return gunman_;
}

std::shared_ptr<entities::weapon> player::get_weapon(){
	return weapon_;
}

std::shared_ptr<entities::pickup> player::get_item(){
	return item_;
}

// pass in the entities list
bool player::update_player(std::vector<std::shared_ptr<entities::entity>>& entities) {
	gunman_->update(entities);
	weapon_->update(entities);
	item_->update(entities);
	// here is where you check for player movement and player firing

	// check gunman movement
	std::for_each(movement_.begin(), movement_.end(), [this, &entities](auto& key_direction) {
	if (IsKeyDown(key_direction.first)) {
		if (gunman_->move(key_direction.second, entities)) {
			auto gunamn_centre_x = gunman_->get_x() + config::GUNMAN_WIDTH / 2;
			auto weapon_x = gunamn_centre_x + ((config::GUNMAN_WIDTH / 2) + 5) * gunman_->get_direction();
			weapon_->set_pos(weapon_x, gunman_->get_y() + 45);
		}
			return true;
		}
		return false; // no movement key was pressed
	});
	// check weapon firing 
	if (weapon_->get_cooldown() > 0) { weapon_->decrement_cooldown(); }

	if (IsKeyPressed(fire_reload_.first) and std::none_of(movement_.begin(), movement_.end(), [](auto& key_direction) {
		return IsKeyDown(key_direction.first); })) {
		if (weapon_->fire()) {
			// calculate the offset as distance from the centre of the gunman
			entities.push_back(std::move(weapon_->create_bullet(weapon_->get_x(), weapon_->get_y(), gunman_->get_direction())));
		}
	}
	if (IsKeyPressed(fire_reload_.second)) {
		weapon_->reload();
	}
	// check if gunman is colliding with an item, then pick it up
	pickup_item(entities);

	// check if an item is used
	if (IsKeyPressed(item_use_)) {
		// use the item
		auto health_item = dynamic_cast<entities::health_pickup*>(item_.get());
		if (health_item != nullptr) {
			health_item->use(gunman_);
		}
		// remove the item from the slot 
		std::cout << "remove item" << std::endl;
		item_ = std::make_shared<entities::empty_pickup>(entities::empty_pickup(0.0, 0.0, config::DEFAULT_PATH));
	}
	return true;
}

void player::pickup_item(std::vector<std::shared_ptr<entities::entity>>& entities) {
	// check gunman collision with items
	for (auto& e : entities) {
		auto pickup = dynamic_cast<entities::pickup*>(e.get());
		if (pickup != nullptr and CheckCollisionRecs(e->get_rectangle(), gunman_->get_rectangle())) {
			e->set_remove(true);
			item_ = std::dynamic_pointer_cast<entities::pickup>(e);
		}
	}
}
void player::draw_player(){
	// draw gunman
	gunman_->draw();
	// draw weapon hud
	if (gunman_->get_direction() == -1) {
		float x = config::SCREEN_WIDTH - 115 - weapon_->get_animation().get_frame_width() - 160;
		weapon_->draw(x, config::SCREEN_HEIGHT - 185);
		// draw hearts
		x += weapon_->get_animation().get_frame_width() + 5;
		auto heart_pos = Vector2{x, config::SCREEN_HEIGHT - 185};
		// draw hearts
		for (auto i = 0; i < gunman_->get_health(); ++i) {
			heart_.draw_frame(heart_pos);
			heart_pos.x += config::HEART_WIDTH + config::HEART_SPACING;
		
		}
		// draw item hud, underneath the heart
		x += item_->get_animation().get_frame_width();
		auto y = heart_pos.y + (item_->get_animation().get_frame_height() * 1.5);
		item_->draw(x, y);


	}
	else {
		float x = 115;
		weapon_->draw(x, config::SCREEN_HEIGHT - 185);
		x += weapon_->get_animation().get_frame_width() + 5;
		auto heart_pos = Vector2{x, config::SCREEN_HEIGHT - 185};
		// draw hearts
		for (auto i = 0; i < gunman_->get_health(); ++i) {
			heart_.draw_frame(heart_pos);
			heart_pos.x += config::HEART_WIDTH + config::HEART_SPACING;
		}


		// draw item hud, underneath the heart
		x += item_->get_animation().get_frame_width();
		auto y = heart_pos.y + (item_->get_animation().get_frame_height() * 1.5);
		item_->draw(x, y);
	}
}

void player::draw_win(){
	// make it last 10 frames
	auto win_pos = Vector2{ config::SCREEN_WIDTH_HALF - (config::WIN_WIDTH / 2), config::SCREEN_HEIGHT_HALF - (config::WIN_HEIGHT / 2) };
	win_.draw_frame(win_pos);
}

void player::increase_score(){
	++score_;
}

bool player::is_dead(){
	if (gunman_->get_health() <= 0) {
		// change anim, check the position
		if (gunman_->get_direction() == 1) {
			auto y = gunman_->get_y() + gunman_->get_animation().get_frame_height();
			gunman_->set_animation(animation(config::P1_DEAD_PATH, config::GUNMAN_DEAD_WIDTH, config::GUNMAN_DEAD_HEIGHT));
			gunman_->set_pos(gunman_->get_x(), gunman_->get_y());
		}
		else{
			gunman_->set_animation(animation(config::P2_DEAD_PATH, config::GUNMAN_DEAD_WIDTH, config::GUNMAN_DEAD_HEIGHT));
			auto x = gunman_->get_x();
			auto y = gunman_->get_y() + gunman_->get_animation().get_frame_height();
			gunman_->set_pos(gunman_->get_x(), y);
			if (x + gunman_->get_animation().get_frame_width() > config::SCREEN_WIDTH) {
				gunman_->set_pos(config::SCREEN_WIDTH - gunman_->get_animation().get_frame_width(), gunman_->get_y());
			}
		}
		return true;
	}
	return false;
}

void player::reset_player(){
	// reset the position
	gunman_->reset(player_start_pos_.x, player_start_pos_.y);
	weapon_->replenish();
	item_ = std::make_shared<entities::empty_pickup>(entities::empty_pickup(0.0, 0.0, config::DEFAULT_PATH));
	// reset the gun position
	auto gunamn_centre_x = gunman_->get_x() + config::GUNMAN_WIDTH / 2;
	auto weapon_x = gunamn_centre_x + ((config::GUNMAN_WIDTH / 2) + config::BULLET_WIDTH) * gunman_->get_direction();
	weapon_->set_pos(weapon_x, gunman_->get_y() + 45);


}

int player::get_score(){
	return score_;
}

void player::set_score(int score){
	score_ = score;
}

float player::get_draw_x(){
	return draw_x_;
}
