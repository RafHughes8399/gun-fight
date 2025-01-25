#pragma once
#include "entities.h"
#include <tuple>
class player{
public:
	~player() = default;
	//TODO cant clone a nullptr
	player(std::shared_ptr<entities::gunman> gunman, std::shared_ptr<entities::weapon> weapon, std::map<int, Vector2>& movement_keys, std::pair<int, int>& fire_reload_keys, int draw_x)
		: gunman_(gunman), weapon_(std::move(weapon)),
		item_(std::make_unique<entities::empty_pickup>(entities::empty_pickup(0.0,0.0, config::DEFAULT_PATH))), movement_(movement_keys), fire_reload_(fire_reload_keys), score_(0), draw_x_(draw_x) {
		player_start_pos_ = gunman_->get_position();
	};
	player(const player& other)
		: gunman_(other.gunman_), weapon_(other.weapon_),
		item_(other.item_), score_(other.score_), player_start_pos_(other.player_start_pos_),
		movement_(other.movement_), fire_reload_(other.fire_reload_), draw_x_(other.draw_x_) {};

	player& operator=(const player& other);
	// get player gunman
	std::shared_ptr<entities::gunman> get_gunman();
	// get player weapon
	std::shared_ptr<entities::weapon> get_weapon();
	// get player item
	std::shared_ptr<entities::pickup> get_item();
	// update player
	bool update_player(std::vector<std::shared_ptr<entities::entity>>& entities);
	// draw player
	void draw_player();
	// increase_score
	void increase_score();
	// is dead
	bool is_dead();
	// player reset
	void reset_player();
private:
// a tuple of gunman, weapon, item
	std::shared_ptr<entities::gunman> gunman_;
	std::shared_ptr<entities::weapon> weapon_;
	std::shared_ptr<entities::pickup> item_;
	int score_;
	Vector2 player_start_pos_;
	float draw_x_;
	// should have the movement keys
	std::map<int, Vector2>& movement_;
	std::pair<int, int>& fire_reload_;
};

