/*****************************************************************//**
 * \file   level_builder.cpp
 * \brief  implementation file for the level builder class
 * 
 * \author raffa
 * \date   February 2025
 *********************************************************************/
#include "level_builder.h"
#include "utility.h"
#include <iostream>
#include <iterator>
#include <cmath>

float clamp(float value, float min, float max) {
	return std::fmax(min, std::fmin(value, max));
}
/** checks if the obstacle can be added at the proposed position, some bugs persist, TODO change */
bool can_insert_obstacle(Rectangle insert_rectangle, const std::set<std::unique_ptr<entities::entity>, decltype(util::cmp)>& entities) {
	for (auto& e : entities) {
		// Get the current rectangle
		Rectangle current_rectangle = e->get_rectangle();

		// Calculate the distances between the edges of the rectangles
		float deltaX = std::max(insert_rectangle.x, current_rectangle.x) - std::min(insert_rectangle.x + insert_rectangle.width, current_rectangle.x + current_rectangle.width);
		float deltaY = std::max(insert_rectangle.y, current_rectangle.y) - std::min(insert_rectangle.y + insert_rectangle.height, current_rectangle.y + current_rectangle.height);

		// If the distance is less than the minimum required distance, return false
		if (deltaX < config::MINIMUM_OBSTACLE_DISTANCE && deltaY < config::MINIMUM_OBSTACLE_DISTANCE) {
			return false;
		}
	}
	return true;
}
void level::level::build_level(){
	/**  generate two numbers between 1 and 3, to determine which obstacles to generate */
	auto obstacle_categories = std::set<int>{};
	for (auto i = 0; i < level_category_; ++i) {
		obstacle_categories.insert(util::generate_random_int(config::TUMBLEWEED_CATEGORY, config::WAGON_CATEGORY));
	}
	if (obstacle_categories.contains(config::TUMBLEWEED_CATEGORY)) { 
		build_tumbleweed(); 
	}
	if (obstacle_categories.contains(config::CACTUS_CATEGORY)) { 
		build_cacti(); }
	if (obstacle_categories.contains(config::BARREL_CATEGORY)) {
		build_barrels(); }
	if (obstacle_categories.contains(config::WAGON_CATEGORY)) { build_wagons(); }
	// then generate up to level_category unique numbers between 0 and 3 

	// if the set contains the obstacle category, generate that obstacle 
}

void level::level::build_tumbleweed(){
	int num_tumbleweed =  ceil(obstacles_to_generate_ * util::generate_random_num<double>(0.2, 0.4));
	obstacles_to_generate_ -= num_tumbleweed;
	for (auto i = 0; i < num_tumbleweed; ++i) {
		
		auto random_x = util::generate_random_num<float>(config::OBSTACLE_RANGE_X + config::TUMBLEWEED_WIDTH, config::OBSTACLE_RANGE_X + config::OBSTACLE_RANGE_WIDTH - config::TUMBLEWEED_WIDTH);
		auto random_y = util::generate_random_num<float>(config::OBSTACLE_RANGE_Y + config::TUMBLEWEED_HEIGHT, config::OBSTACLE_RANGE_HEIGHT - config::TUMBLEWEED_HEIGHT);
		auto tumbleweed = std::make_unique<entities::tumbleweed>(entities::tumbleweed(
			static_cast<float>(random_x), static_cast<float>(random_y)));
		auto num_attempts = 0;
		while (not can_insert_obstacle(tumbleweed->get_rectangle(), level_entities_) and num_attempts < 20) {
			tumbleweed->set_pos(util::generate_random_num<float>(config::OBSTACLE_RANGE_X + config::TUMBLEWEED_WIDTH, config::OBSTACLE_RANGE_X + config::OBSTACLE_RANGE_WIDTH - config::TUMBLEWEED_WIDTH),
				util::generate_random_num<float>(config::OBSTACLE_RANGE_Y + config::TUMBLEWEED_HEIGHT,config::OBSTACLE_RANGE_HEIGHT - config::TUMBLEWEED_HEIGHT));
			++num_attempts;
		}
		if (num_attempts < 20) {
			level_entities_.insert(std::make_unique<entities::tumbleweed>(*tumbleweed.get()));
		}
	}
}
/**  the following methods have the same logic, just are separated by the obstacle that they place in the level */
void level::level::build_cacti(){
	/**  the number of the obstacle to generate */
	int num_cacti = ceil(obstacles_to_generate_ * util::generate_random_num<double>(0.2, 0.4));
	obstacles_to_generate_ -= num_cacti;
	// if empty, just pick a random position and add the entity there	
	for (auto i = 0; i < num_cacti; ++i) {
		/**  generate a random position within the bounds defined in the config file  */
		auto random_x = util::generate_random_num(config::OBSTACLE_RANGE_X + config::CACTUS_WIDTH, config::OBSTACLE_RANGE_X + config::OBSTACLE_RANGE_WIDTH - config::CACTUS_WIDTH);
		auto random_y = util::generate_random_num(config::OBSTACLE_RANGE_Y + config::CACTUS_HEIGHT, config::OBSTACLE_RANGE_HEIGHT - config::CACTUS_HEIGHT);
		auto cactus = std::make_unique<entities::cactus>(entities::cactus(
			static_cast<float>(random_x), static_cast<float>(random_y)));
		
		/**  until it can be inserted, generate a random position  */
		auto num_attempts = 0;
		while (not can_insert_obstacle(cactus->get_rectangle(), level_entities_) and num_attempts < 20) {

			cactus->set_pos(static_cast<float>(util::generate_random_num(config::OBSTACLE_RANGE_X + config::CACTUS_WIDTH, config::OBSTACLE_RANGE_X + config::OBSTACLE_RANGE_WIDTH - config::CACTUS_WIDTH)),
				static_cast<float>(util::generate_random_num(config::OBSTACLE_RANGE_Y + config::CACTUS_HEIGHT, config::OBSTACLE_RANGE_HEIGHT - config::CACTUS_HEIGHT)));
			++num_attempts;
		}
		/**  if it can be inserted in the level, do so */
		if (num_attempts < 20) {
			level_entities_.insert(std::make_unique<entities::cactus>(*cactus.get()));
		}
	}
}

void level::level::build_barrels(){
	int num_barrels = ceil(obstacles_to_generate_ * util::generate_random_num<double>(0.2, 0.4));
	obstacles_to_generate_ -= num_barrels;
	for (auto i = 0; i < num_barrels; ++i) {
		auto random_x = util::generate_random_num<float>(config::OBSTACLE_RANGE_X + config::BARREL_WIDTH, config::OBSTACLE_RANGE_X + config::OBSTACLE_RANGE_WIDTH - config::BARREL_WIDTH);
		auto random_y = util::generate_random_num<float>(config::OBSTACLE_RANGE_Y + config::BARREL_HEIGHT, config::OBSTACLE_RANGE_HEIGHT - config::BARREL_HEIGHT);
		auto barrel = std::make_unique<entities::barrel>(entities::barrel(
			static_cast<float>(random_x), static_cast<float>(random_y)));

		auto num_attempts = 0;
		while (not can_insert_obstacle(barrel->get_rectangle(), level_entities_) and num_attempts < 20) {

			barrel->set_pos(util::generate_random_num<float>(config::OBSTACLE_RANGE_X + config::BARREL_WIDTH, config::OBSTACLE_RANGE_X + config::OBSTACLE_RANGE_WIDTH - config::BARREL_WIDTH),
				util::generate_random_num<float>(config::OBSTACLE_RANGE_Y + config::BARREL_HEIGHT, config::OBSTACLE_RANGE_HEIGHT - config::BARREL_HEIGHT));
		
			++num_attempts;
		}
		if (num_attempts < 20) {
			level_entities_.insert(std::make_unique<entities::barrel>(*barrel.get()));
		}
	}
}

void level::level::build_wagons(){
	int num_wagons = ceil(obstacles_to_generate_ * util::generate_random_num<double>(0.3, 0.6));
	obstacles_to_generate_ -= num_wagons;
	for (auto i = 0; i < num_wagons; ++i) {
		auto random_x = util::generate_random_num<float>(config::OBSTACLE_RANGE_X + config::WAGON_DOWN_WIDTH, config::OBSTACLE_RANGE_X + config::OBSTACLE_RANGE_WIDTH - config::WAGON_DOWN_WIDTH);
		auto random_y = util::generate_random_num<float>(config::OBSTACLE_RANGE_Y + config::WAGON_DOWN_HEIGHT, config::OBSTACLE_RANGE_HEIGHT - config::WAGON_DOWN_HEIGHT);
		auto wagon = std::make_unique<entities::wagon>(entities::wagon(
			static_cast<float>(random_x), static_cast<float>(random_y), 0.0, config::WAGON_SPEED));
		
		auto num_attempts = 0;
		while (not can_insert_obstacle(wagon->get_rectangle(), level_entities_) and num_attempts < 20) {

			wagon->set_pos(util::generate_random_num<float>(config::OBSTACLE_RANGE_X + config::WAGON_DOWN_WIDTH, config::OBSTACLE_RANGE_X + config::OBSTACLE_RANGE_WIDTH - config::WAGON_DOWN_WIDTH),
				util::generate_random_num<float>(config::OBSTACLE_RANGE_Y + config::WAGON_DOWN_HEIGHT, config::OBSTACLE_RANGE_HEIGHT - config::WAGON_DOWN_HEIGHT));
			++num_attempts;
		}
		if (num_attempts < 20) {
			level_entities_.insert(std::make_unique<entities::wagon>(*wagon.get()));
		}
	}
}

void level::level::build_train() {
	return;
}

std::set<std::unique_ptr<entities::entity>, decltype(util::cmp)>& level::level_builder::get_level_entities() {
	return level_entities_;
}
