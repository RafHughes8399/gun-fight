#include "game_manager.h"

void game_manager::remove_entities(){
	auto new_end = std::remove_if(game_entities_.begin(), game_entities_.end(), [](auto& e) {
		return e->get_remove();
		});
	game_entities_.erase(new_end, game_entities_.end());
}

void game_manager::clear_entities(){
	game_entities_.clear(); // i am so dumb and stupid
	game_entities_.push_back(player_1_.get_gunman());
	game_entities_.push_back(player_2_.get_gunman());
}

void game_manager::update_entities(){
	// the gunman should be in the entity list but not
	for (auto& e : game_entities_) {
		auto gunman_ptr = dynamic_cast<entities::gunman*>(e.get());
		if (gunman_ptr == nullptr) {
			e->update(game_entities_);
		}
	}
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

void game_manager::end_round() {
	round_over_ = true;
}

bool game_manager::is_round_over(){
	return round_over_;
}

void game_manager::build_level(){
	// reset the players, clear the environment
	player_1_.reset_player();
	player_2_.reset_player();
	clear_entities();
	// update counters
	frame_count_ = 0;
	++round_num_;
	round_over_ = false;
	// generate environment
	auto category = util::generate_random_num(0.0, 3.0);
	if (category <= 0.5) { category = 0; }
	else { category = ceil(category); }
	auto obstacles_to_generate = 2 * (round_num_ % 3) + 1;
	auto builder = std::make_unique<level::level>(level::level(category, obstacles_to_generate));

	builder->build_level();
	auto& level_entities = builder->get_level_entities();

	while (not level_entities.empty()) {
		auto it = level_entities.extract(level_entities.begin());
		game_entities_.push_back(std::move(it.value()));
	}
}
