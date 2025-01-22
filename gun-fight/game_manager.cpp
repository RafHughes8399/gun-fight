#include "game_manager.h"

void game_manager::set_players(player p1, player p2){
	player_1 = p1;
	player_2 = p2;
}

void game_manager::remove_entities(){
	auto new_end = std::remove_if(game_entities_.begin(), game_entities_.end(), [](auto& e) {
		return e->get_remove();
		});
	game_entities_.erase(new_end, game_entities_.end());
}

void game_manager::clear_entities(){
	game_entities_.clear();
}


void game_manager::update_entities(){
	for (auto& e : game_entities_) {
		e->update(game_entities_);
	}
}

void game_manager::draw_entities(){
	for (auto& e : game_entities_) {
		e->draw();
	}
}

void game_manager::update_players(){
	if (not player_1.update_player(game_entities_)) {
		player_2.increase_score();
		end_round();
	}
	if (not player_2.update_player(game_entities_)) {
		player_1.increase_score();
		end_round();
	}
}

void game_manager::draw_players(){
	player_1.draw_player();
	player_2.draw_player();
	
}

int game_manager::get_round_num(){
	return round_num_;
}

int game_manager::get_frame_count(){
	return frame_count_;
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
	player_1.reset_player();
	player_2.reset_player();
	clear_entities();
	// update counters
	frame_count_ = 0;
	++round_num_;
	round_over_ = false;
	// generate environment
	auto category = util::generate_random_num(0.0, 2.0);
	if (category <= 0.5) { category = 0; }
	else { category = ceil(category); }
	auto obstacles_to_generate = 2 * (round_num_ % 4) + 1;
	auto builder = std::make_unique<level::level>(level::level(category, obstacles_to_generate));

	builder->build_level();
	auto& level_entities = builder->get_level_entities();

	while (not level_entities.empty()) {
		auto it = level_entities.extract(level_entities.begin());
		game_entities_.push_back(std::move(it.value()));
	}
}
