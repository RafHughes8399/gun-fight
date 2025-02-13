/*****************************************************************//**
 * \file   game_manager.cpp
 * \brief  implementation file for the game_manager class. 
 * 
 * \author raffa
 * \date   February 2025
 *********************************************************************/
#include "game_manager.h"
#include <iostream>
/**  erase entities that should be removed */
void game_manager::remove_entities(){
	auto new_end = std::remove_if(game_entities_.begin(), game_entities_.end(), [](auto& e) {
		return e->get_remove();
		});
	game_entities_.erase(new_end, game_entities_.end());
}

/**  remove all entities apart from the player characters */
void game_manager::clear_entities(){
	game_entities_.clear(); 
	game_entities_.push_back(player_1_.get_gunman());
	game_entities_.push_back(player_2_.get_gunman());
}

/**  update all entities */
void game_manager::update_entities(){
	// the gunman should be in the entity list but not
	for (auto& e : game_entities_) {
		auto gunman_ptr = dynamic_cast<entities::gunman*>(e.get());
		if (gunman_ptr == nullptr) {
			e->update(game_entities_);
		}
	}
}

/**  draw elemenets of the game */
void game_manager::draw_game(){
	draw_background();
	draw_players();
	draw_scores();
	draw_entities();
}

void game_manager::draw_entities(){
	for (auto& e : game_entities_) {
		e->draw();
	}
}

void game_manager::draw_scores(){
	auto pos = Vector2{ player_1_.get_draw_x(), 10.0};
	scores_.select_frame(player_1_.get_score());
	scores_.draw_frame(pos);

	pos = Vector2{ player_2_.get_draw_x(), 10.0};
	scores_.select_frame(player_2_.get_score());
	scores_.draw_frame(pos);
}	

void game_manager::update_players(){
	if (player_1_.is_dead()) {
		player_2_.increase_score();
		end_round();
	}
	else if (player_2_.is_dead()) {
		player_1_.increase_score();
		end_round();
	}
	else {
		player_1_.update_player(game_entities_);
		player_2_.update_player(game_entities_);
	}
}

void game_manager::draw_players(){
	player_1_.draw_player();
	player_2_.draw_player();
}

int game_manager::get_round_num(){
	return round_num_;
}

int game_manager::get_frame_count(){
	return frame_count_;
}

/**  draw the background and the hud frame */
void game_manager::draw_background() {
	auto pos = Vector2{ config::PLAYABLE_X, config::PLAYABLE_Y };
	background_.draw_frame(pos);

	pos = Vector2{ 0.0,0.0 };
	header_.draw_frame(pos);
	pos = Vector2{ 0.0,config::PLAYABLE_HEIGHT};
	footer_.draw_frame(pos);
}

void game_manager::increment_round_count(){
	++round_num_;
}

void game_manager::increment_frame_count(){
	++frame_count_;
}

void game_manager::reset_level(){
	game_entities_.clear();
}

void game_manager::reset_scores(){
	player_1_.set_score(0);
	player_2_.set_score(0);
}

void game_manager::end_round() {
	// wait a few seconds to let the dead animation appears	
	Sound s = LoadSound(config::DEATH_SOUND);
	PlaySound(s);
	round_over_ = true;
	last_spawn_time  = 0;
}

bool game_manager::is_round_over(){
	return round_over_;
}

void game_manager::pre_round(){
	auto start = GetTime();
	auto draw = LoadTexture(config::DRAW_PATH);
	/** before each round, draw the round intro texture */
	while (GetTime() - start < 1.1) {
		BeginDrawing();
		draw_game();
		auto draw_x = config::SCREEN_WIDTH_HALF - (config::DRAW_WIDTH / 2);
		auto draw_y = config::SCREEN_HEIGHT_HALF - (config::DRAW_HEIGHT / 2);
		DrawTexture(draw, draw_x, draw_y, WHITE);	
		EndDrawing();
	}
}

void game_manager::post_round(){
	auto start = GetTime();
	/** draw only the gunmen and the background, makes time for voicelines */
	while (GetTime() - start < 1.5) {
		BeginDrawing();
		draw_game();
		EndDrawing();
	}
}

bool game_manager::game_over(){
	return player_1_.get_score() == config::MAX_SCORE or player_2_.get_score() == config::MAX_SCORE;
}

void game_manager::draw_win(){
	if (player_1_.get_score() == config::MAX_SCORE) {
		player_1_.draw_win();
	}
	else if (player_2_.get_score() == config::MAX_SCORE) {
		player_2_.draw_win();
	}
}

void game_manager::play_voiceline(){
	auto index = util::generate_random_int(0, voicelines_.size() - 1);
	PlaySound(voicelines_[index]);
}

/**  build the level for each round */
void game_manager::build_level(){
	/** reset the players, remove obstacles */
	player_1_.reset_player();
	player_2_.reset_player();
	clear_entities();
	
	/** reset counters  */
	frame_count_ = 0;
	++round_num_;
	round_over_ = false;

	/**  pick random types of obstacles to generate, 0 is no obstalces */
	auto category = util::generate_random_num(0.0, 3.0);
	if (category <= 0.5) { category = 0; }
	else { category = ceil(category); }
	/**  determine the number of obstacles to generate */
	auto obstacles_to_generate = 2 * (util::generate_random_int(0, 4));
	auto builder = std::make_unique<level::level>(level::level(category, obstacles_to_generate));

	/**  build the environment by placing obstacles randomly */
	builder->build_level();
	auto& level_entities = builder->get_level_entities();

	/**  transfer obstacles to game manager */
	while (not level_entities.empty()) {
		auto it = level_entities.extract(level_entities.begin());
		game_entities_.push_back(std::move(it.value()));
	}
}
/** every 6.5 seconds, spawn an item on either side of the map */
void game_manager::spawn_items(){
	// check time
	auto time = GetTime();
	if (time - last_spawn_time >= config::ITEM_SPAWN_DELAY) {
		last_spawn_time = time;
		// pick two random items (use an enum)
		auto item_1_type = util::generate_random_int(config::item_codes::HEALTH, config::item_codes::AMMO);  // currently either health or armour 
		auto item_2_type = util::generate_random_int(config::item_codes::HEALTH, config::item_codes::AMMO);  // currently either health or armour 


		// generate the two positions
		auto item_1_x = util::generate_random_num<float>(config::P1_ITEM_SPAWN_X + config::ITEM_WIDTH, config::P1_ITEM_SPAWN_WIDTH - config::ITEM_WIDTH);
		auto item_1_y = util::generate_random_num<float>(config::P1_ITEM_SPAWN_Y + config::ITEM_WIDTH, config::P1_ITEM_SPAWN_HEIGHT - config::ITEM_HEIGHT);
		auto item_2_x = util::generate_random_num<float>(config::P2_ITEM_SPAWN_X - config::ITEM_WIDTH, config::SCREEN_WIDTH - config::ITEM_WIDTH);
		auto item_2_y = util::generate_random_num<float>(config::P2_ITEM_SPAWN_Y + config::ITEM_WIDTH, config::P2_ITEM_SPAWN_HEIGHT - config::ITEM_HEIGHT);
		// spawn an item for p1
		switch (item_1_type) {
			case config::item_codes::HEALTH:
				game_entities_.push_back(std::make_shared<entities::health_pickup>(item_1_x, item_1_y, config::HEALTH_PICKUP_PATH));
				break;
			case config::item_codes::ARMOUR:
				game_entities_.push_back(std::make_shared<entities::armour_pickup>(item_1_x, item_1_y, config::ARMOUR_PICKUP_PATH));
				break;
			case config::item_codes::AMMO:
				game_entities_.push_back(std::make_shared<entities::ammo_pickup>(item_1_x, item_1_y, config::AMMO_PICKUP_PATH));
				break;
			case config::item_codes::RIFLE:
				game_entities_.push_back(std::make_shared<entities::rifle_pickup>(item_1_x, item_1_y, config::RIFLE_PICKUP_PATH));
				break;
			case config::item_codes::DYNAMITE:
				game_entities_.push_back(std::make_shared<entities::dynamite_pickup>(item_1_x, item_1_y, config::DYNAMITE_PICKUP_PATH));
				break;
		}
		// spawn an item for p2
		switch (item_2_type) {
			case config::item_codes::HEALTH:
				game_entities_.push_back(std::make_shared<entities::health_pickup>(item_2_x, item_2_y, config::HEALTH_PICKUP_PATH));
				break;
			case config::item_codes::ARMOUR:
				game_entities_.push_back(std::make_shared<entities::armour_pickup>(item_2_x, item_2_y, config::ARMOUR_PICKUP_PATH));
				break;
			case config::item_codes::AMMO:
				game_entities_.push_back(std::make_shared<entities::ammo_pickup>(item_2_x, item_2_y, config::AMMO_PICKUP_PATH));
				break;
			case config::item_codes::RIFLE:
				game_entities_.push_back(std::make_shared<entities::rifle_pickup>(item_2_x, item_2_y, config::RIFLE_PICKUP_PATH));
				break;
			case config::item_codes::DYNAMITE:
				game_entities_.push_back(std::make_shared<entities::dynamite_pickup>(item_2_x, item_2_y, config::DYNAMITE_PICKUP_PATH));
				break;
		}
	}
	return;
}
