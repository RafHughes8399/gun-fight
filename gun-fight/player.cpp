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
	// check item usage 
	return true;
}
void player::draw_player(){
	auto heart_pos = Vector2{draw_x_,  config::SCREEN_HEIGHT - 175 };
	// draw gunman
	gunman_->draw();
	// draw weapon hyd
	if (gunman_->get_direction() == -1) {
		weapon_->draw(draw_x_ - weapon_->get_animation().get_frame_width(), config::SCREEN_HEIGHT - 175);
		heart_pos.x = weapon_->get_animation().get_frame_width() + 15;
	}
	else {
		weapon_->draw(draw_x_, config::SCREEN_HEIGHT - 175);
		heart_pos.x = draw_x_ + 15;
	}
	// draw item hud
	item_->draw();
	if (not is_dead()) {
		heart_.draw_frame(heart_pos);
	}
}

void player::increase_score(){
	++score_;
}

bool player::is_dead(){
	return gunman_->get_health() <= 0;
}

void player::reset_player(){
	// reset the position
	gunman_->reset(player_start_pos_.x, player_start_pos_.y);
	weapon_->replenish();
	// reset the gun position
	auto gunamn_centre_x = gunman_->get_x() + config::GUNMAN_WIDTH / 2;
	auto weapon_x = gunamn_centre_x + ((config::GUNMAN_WIDTH / 2) + config::BULLET_WIDTH) * gunman_->get_direction();
	weapon_->set_pos(weapon_x, gunman_->get_y() + 45);
}

int player::get_score(){
	return score_;
}

float player::get_draw_x(){
	return draw_x_;
}
