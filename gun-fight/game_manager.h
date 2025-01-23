#pragma once
#include "entities.h"
#include "level_builder.h"
#include "player.h"
#include <map>
#include <utility>
class game_manager{
public:
	~game_manager() = default;
	game_manager(player player1, player player2)
		: player_1_(player1), player_2_(player2), game_entities_(std::vector<std::shared_ptr<entities::entity>>{player1.get_gunman(), player2.get_gunman()}) {
	};


	// manage entities
	void remove_entities();
	void clear_entities();

	void add_gunmen();
	void update_entities();
	void draw_entities();

	void update_players();
	void draw_players();
	int get_round_num();
	int get_frame_count();

	void increment_round_count();
	void increment_frame_count();
	void reset_level();
	void build_level();
	void end_round();
	bool is_round_over();
private:
	player player_1_;
	player player_2_;
	std::vector<std::shared_ptr<entities::entity>> game_entities_;
	int frame_count_ = 0;
	int round_num_ = 1;
	bool round_over_ = false;
};

