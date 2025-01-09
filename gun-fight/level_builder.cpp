#include "level_builder.h"
#include "utility.h"
#include <iostream>
#include <iterator>

bool can_insert_obstacle(const Vector2& obs_position, const Vector2& obstacle_dimensions, const std::set<std::unique_ptr<entities::entity>, decltype(util::cmp)>& entities) {
	// calculate the distances
	// check the distance to each other obstacle to determine if it meets the threshold
	// should be entities.end() if all obstacles are satisfied
	auto it = std::find_if(entities.begin(), entities.end(), [obs_position, obstacle_dimensions](auto& e) {
		auto distance = std::sqrt(Vector2DistanceSqr(obs_position, e->get_position())) + sqrt(Vector2LengthSqr(obstacle_dimensions));
		return distance < config::MINIMUM_OBSTACLE_DISTANCE;
		});
	return it == entities.end();
}
void level::level::build_level(){
	// generate up to 2 random numbers between 0 and 3 inclusive
	// check level_category
	auto obstacle_categories = std::set<int>{};
	for (auto i = 0; i < level_category_; ++i) {
		obstacle_categories.insert(util::generate_random_int(config::TUMBLEWEED_CATEGORY, config::BARREL_CATEGORY));
	}
	if (obstacle_categories.contains(config::TUMBLEWEED_CATEGORY)) { build_tumbleweed(); }
	if (obstacle_categories.contains(config::CACTUS_CATEGORY)) { build_cacti(); }
	if (obstacle_categories.contains(config::BARREL_CATEGORY)) { build_barrels(); }
	if (obstacle_categories.contains(config::WAGON_CATEGORY)) { build_wagons(); }
	// then generate up to level_category unique numbers between 0 and 3 

	// if the set contains the obstacle category, generate that obstacle 
}

void level::level::build_tumbleweed(){
	int num_tumbleweed =  ceil(obstacles_to_generate_ * util::generate_random_num<double>(0.2, 0.6));
	obstacles_to_generate_ -= num_tumbleweed;
	auto tumbleweed_dimensions = Vector2{ config::TUMBLEWEED_LENGTH, config::TUMBLEWEED_HEIGHT };
	for (auto i = 0; i < num_tumbleweed; ++i) {
		auto random_x = util::generate_random_num<float>(config::OBSTACLE_RANGE_X + config::TUMBLEWEED_LENGTH, config::OBSTACLE_RANGE_X + config::OBSTACLE_RANGE_WIDTH - config::TUMBLEWEED_LENGTH);
		auto random_y = util::generate_random_num<float>(config::OBSTACLE_RANGE_Y + config::TUMBLEWEED_HEIGHT, config::OBSTACLE_RANGE_Y + config::OBSTACLE_RANGE_HEIGHT - config::TUMBLEWEED_HEIGHT);
		auto tumbleweed = std::make_unique<entities::tumbleweed>(entities::tumbleweed(
			static_cast<float>(random_x), static_cast<float>(random_y)));
		while (not can_insert_obstacle(tumbleweed->get_position(), tumbleweed_dimensions, level_entities_)) {
			tumbleweed->set_pos(util::generate_random_num<float>(config::OBSTACLE_RANGE_X + config::TUMBLEWEED_LENGTH, config::OBSTACLE_RANGE_X + config::OBSTACLE_RANGE_WIDTH - config::TUMBLEWEED_LENGTH),
				util::generate_random_num<float>(config::OBSTACLE_RANGE_Y + config::TUMBLEWEED_HEIGHT, config::OBSTACLE_RANGE_Y + config::OBSTACLE_RANGE_HEIGHT - config::TUMBLEWEED_HEIGHT));
		}
		level_entities_.insert(std::make_unique<entities::tumbleweed>(*tumbleweed.get()));
	}
}

void level::level::build_cacti(){
	int num_cacti = ceil(obstacles_to_generate_ * util::generate_random_num<double>(0.2, 0.6));
	obstacles_to_generate_ -= num_cacti;
	// if empty, just pick a random position and add the entity there
	auto cactus_dimensions = Vector2{ config::CACTUS_LENGTH, config::CACTUS_HEIGHT };
	for (auto i = 0; i < num_cacti; ++i) {
		auto random_x = util::generate_random_num(config::OBSTACLE_RANGE_X + config::CACTUS_LENGTH, config::OBSTACLE_RANGE_X + config::OBSTACLE_RANGE_WIDTH - config::CACTUS_LENGTH);
		auto random_y = util::generate_random_num(config::OBSTACLE_RANGE_Y + config::CACTUS_HEIGHT, config::OBSTACLE_RANGE_Y + config::OBSTACLE_RANGE_HEIGHT - config::CACTUS_HEIGHT);
		auto cactus = std::make_unique<entities::cactus>(entities::cactus(
			static_cast<float>(random_x), static_cast<float>(random_y)));
		while (not can_insert_obstacle(cactus->get_position(), cactus_dimensions, level_entities_)) {
			cactus->set_pos(static_cast<float>(util::generate_random_num(config::OBSTACLE_RANGE_X + config::CACTUS_LENGTH, config::OBSTACLE_RANGE_X + config::OBSTACLE_RANGE_WIDTH - config::CACTUS_LENGTH)),
				static_cast<float>(util::generate_random_num(config::OBSTACLE_RANGE_Y + config::CACTUS_HEIGHT, config::OBSTACLE_RANGE_Y + config::OBSTACLE_RANGE_HEIGHT - config::CACTUS_HEIGHT)));
		}
		level_entities_.insert(std::make_unique<entities::cactus>(*cactus.get()));
	}

// otherwise pick a random distance that is at least, the minimum distance away
}

void level::level::build_barrels(){
	int num_barrels = ceil(obstacles_to_generate_ * util::generate_random_num<double>(0.2, 0.6));
	obstacles_to_generate_ -= num_barrels;
	auto barrel_dimensions = Vector2{ config::BARREL_LENGTH, config::CACTUS_LENGTH };
	for (auto i = 0; i < num_barrels; ++i) {
		auto random_x = util::generate_random_num<float>(config::OBSTACLE_RANGE_X + config::BARREL_LENGTH, config::OBSTACLE_RANGE_X + config::OBSTACLE_RANGE_WIDTH - config::BARREL_LENGTH);
		auto random_y = util::generate_random_num<float>(config::OBSTACLE_RANGE_Y + config::BARREL_HEIGHT, config::OBSTACLE_RANGE_Y + config::OBSTACLE_RANGE_HEIGHT - config::BARREL_HEIGHT);
		auto barrel = std::make_unique<entities::barrel>(entities::barrel(
			static_cast<float>(random_x), static_cast<float>(random_y)));
		while (not can_insert_obstacle(barrel->get_position(), barrel_dimensions, level_entities_)) {
			barrel->set_pos(util::generate_random_num<float>(config::OBSTACLE_RANGE_X + config::BARREL_LENGTH, config::OBSTACLE_RANGE_X + config::OBSTACLE_RANGE_WIDTH - config::BARREL_LENGTH),
				util::generate_random_num<float>(config::OBSTACLE_RANGE_Y + config::BARREL_HEIGHT, config::OBSTACLE_RANGE_Y + config::OBSTACLE_RANGE_HEIGHT - config::BARREL_HEIGHT));
		}
		level_entities_.insert(std::make_unique<entities::barrel>(*barrel.get()));
	}
}

void level::level::build_wagons(){
	int num_wagons = ceil(obstacles_to_generate_ * util::generate_random_num<double>(0.3, 0.6));

	obstacles_to_generate_ -= num_wagons;
}

void level::level::build_train() {
	return;
}

std::set<std::unique_ptr<entities::entity>, decltype(util::cmp)>& level::level_builder::get_level_entities() {
	return level_entities_;
}
