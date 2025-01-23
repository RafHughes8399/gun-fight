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
	if (not gunman_->update(entities)) { return false; }
	weapon_->update(entities);
	if (item_ != nullptr) {
		item_->update(entities);
	}

	// here is where you check for player movement and player firing

	// check gunman movement
	std::for_each(movement_.begin(), movement_.end(), [this, &entities](auto& key_direction) {
	if (IsKeyDown(key_direction.first)) {
		if (gunman_->move(key_direction.second, entities)) {
			auto gunamn_centre_x = gunman_->get_x() + config::GUNMAN_WIDTH / 2;
			auto weapon_x = gunamn_centre_x + ((config::GUNMAN_WIDTH / 2) + config::BULLET_WIDTH) * gunman_->get_direction();
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
	// draw gunman
	gunman_->draw();
	// draw weapon hyd
	weapon_->draw(player_start_pos_.x + (200 * gunman_->get_direction()), config::SCREEN_HEIGHT - 300);
	// draw item hud
	item_->draw();
	DrawText(std::to_string(score_).c_str(), player_start_pos_.x, 50, 36, colours::maize);

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


// some logic to consider, the weapon is at the position where the bullets spawn
// then the weapon position is used to spawn bullets
