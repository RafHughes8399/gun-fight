/*****************************************************************//**
 * \file   game_manager.h
 * \brief  header file to define the game_manager class. Responsible for 
 * updating and drawing the game, players, and entities. It manages player scores
 * round transitions and win conditions.
 * 
 * \author raffa
 * \date   February 2025
 *********************************************************************/
#pragma once
#include "entities.h"
#include "level_builder.h"
#include "player.h"
#include <map>
#include <utility>
class game_manager{
public:
	/**  constructors and destructors */
	~game_manager() = default;
	game_manager(player player1, player player2)
		: player_1_(player1), player_2_(player2), game_entities_(std::vector<std::shared_ptr<entities::entity>>{player1.get_gunman(), player2.get_gunman()}) {
		background_ = animation(config::BACKGROUND_PATH, config::PLAYABLE_WIDTH, config::PLAYABLE_HEIGHT);
		scores_ = animation(config::SCORE_PATH, config::SCORE_WIDTH, config::SCORE_HEIGHT, config::SCORES_LENGTH, config::SCORES_ANIMATIONS);
		header_ = animation(config::HUD_HEAD_PATH, config::SCREEN_WIDTH, config::PLAYABLE_Y);
		footer_ = animation(config::HUD_FOOT_PATH, config::SCREEN_WIDTH, config::PLAYABLE_Y + config::HUD_HEIGHT);
		
		for (auto& path : config::VOICE_LINES) {
			voicelines_.push_back(LoadSound(path));
		}
	};


	/**  manage entities in the game */
	void remove_entities();
	void clear_entities();
	void build_level();
	void spawn_items();

	/**  update game entities */
	void update_entities();

	/**  draw the game  */
	void draw_game();
	void draw_background();
	void draw_entities();
	void draw_scores();
	void update_players();
	void draw_players();

	/**  accessors  */
	int get_round_num();
	int get_frame_count();

	/**  modifiers */
	void increment_round_count();
	void increment_frame_count();
	void reset_level();
	void reset_scores();

	/**  round transitions and win conditions */
	void end_round();
	bool is_round_over();
	void pre_round();
	void post_round();
	bool game_over();
	void draw_win();
	void play_voiceline();
private:
	/**  the two players and entities*/
	player player_1_;
	player player_2_;
	std::vector<std::shared_ptr<entities::entity>> game_entities_;

	/**  game info */
	int frame_count_ = 0;
	double last_spawn_time = 0.0;
	int round_num_ = 1;
	bool round_over_ = false;

	/**  animations for drawing */
	animation scores_;
	animation background_;
	animation header_;
	animation footer_;

	/**  voice lines */
	std::vector<Sound> voicelines_;
};




