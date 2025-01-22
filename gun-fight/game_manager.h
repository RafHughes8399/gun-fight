#pragma once
#include "entities.h"
#include "level_builder.h"
#include "player.h"
class game_manager{
public:
	~game_manager() = default;
	game_manager(player p1, player p2)
		:game_entities_(std::vector<std::unique_ptr<entities::entity>>()), player_1(p1), player_2(p2) {
	};

	// manage entities
	void set_players(player p1, player p2);
	void remove_entities();
	void clear_entities();

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
	std::vector<std::unique_ptr<entities::entity>> game_entities_;
	player player_1;
	player player_2;
	int frame_count_ = 0;
	int round_num_ = 1;
	bool round_over_ = false;
};

