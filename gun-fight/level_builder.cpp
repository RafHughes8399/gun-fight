#include "level_builder.h"
#include "utility.h"
#include <iostream>
#include <iterator>
#include <cmath>

float clamp(float value, float min, float max) {
	return std::fmax(min, std::fmin(value, max));
}
bool can_insert_obstacle(Rectangle insert_rectangle, const std::set<std::unique_ptr<entities::entity>, decltype(util::cmp)>& entities) {
	// this should caclulate the distance between the squares, not the points

	// find the centre point of the insert rectangle
	Vector2 insert_centre = {insert_rectangle.x + insert_rectangle.x + (insert_rectangle.width /2), 
							 insert_rectangle.y + insert_rectangle.y + (insert_rectangle.height / 2)};	// find the centre point of the insert rectangle
	for (auto& e : entities) {
		// find the centre point of the current rectangle
		Rectangle current_rectangle = e->get_rectangle();
		Vector2 current_centre = { current_rectangle.x + current_rectangle.x + (current_rectangle.width / 2),
								current_rectangle.y + current_rectangle.y + (current_rectangle.height / 2) };
		// then apply the formula being
		// max ( |x1 - x2| - (w1 + w2)/2, |y1 - y2| - (h1 + h2) / 2)
		auto distance = std::fmax(std::abs(insert_centre.x - current_centre.x) - ((insert_rectangle.width + current_rectangle.width) / 2),
			std::abs(insert_centre.y - current_centre.y) - ((insert_rectangle.height + current_rectangle.height) / 2));
		if (distance < config::MINIMUM_OBSTACLE_DISTANCE) { return false; }
	}
	return true;
}
void level::level::build_level(){
	// generate up to 2 random numbers between 0 and 3 inclusive
	// check level_category
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
	auto tumbleweed_dimensions = Vector2{ config::TUMBLEWEED_WIDTH, config::TUMBLEWEED_HEIGHT };
	for (auto i = 0; i < num_tumbleweed; ++i) {
		
		auto random_x = util::generate_random_num<float>(config::OBSTACLE_RANGE_X + config::TUMBLEWEED_WIDTH, config::OBSTACLE_RANGE_X + config::OBSTACLE_RANGE_WIDTH - config::TUMBLEWEED_WIDTH);
		auto random_y = util::generate_random_num<float>(config::OBSTACLE_RANGE_Y + config::TUMBLEWEED_HEIGHT, config::OBSTACLE_RANGE_Y + config::OBSTACLE_RANGE_HEIGHT - config::TUMBLEWEED_HEIGHT);
		auto tumbleweed = std::make_unique<entities::tumbleweed>(entities::tumbleweed(
			static_cast<float>(random_x), static_cast<float>(random_y)));

		while (not can_insert_obstacle(tumbleweed->get_rectangle(), level_entities_)) {
			tumbleweed->set_pos(util::generate_random_num<float>(config::OBSTACLE_RANGE_X + config::TUMBLEWEED_WIDTH, config::OBSTACLE_RANGE_X + config::OBSTACLE_RANGE_WIDTH - config::TUMBLEWEED_WIDTH),
				util::generate_random_num<float>(config::OBSTACLE_RANGE_Y + config::TUMBLEWEED_HEIGHT, config::OBSTACLE_RANGE_Y + config::OBSTACLE_RANGE_HEIGHT - config::TUMBLEWEED_HEIGHT));
		}

		level_entities_.insert(std::make_unique<entities::tumbleweed>(*tumbleweed.get()));
	}
}

void level::level::build_cacti(){
	int num_cacti = ceil(obstacles_to_generate_ * util::generate_random_num<double>(0.2, 0.4));
	obstacles_to_generate_ -= num_cacti;
	// if empty, just pick a random position and add the entity there
	auto cactus_dimensions = Vector2{ config::CACTUS_WIDTH, config::CACTUS_HEIGHT };
	
	for (auto i = 0; i < num_cacti; ++i) {
		
		auto random_x = util::generate_random_num(config::OBSTACLE_RANGE_X + config::CACTUS_WIDTH, config::OBSTACLE_RANGE_X + config::OBSTACLE_RANGE_WIDTH - config::CACTUS_WIDTH);
		auto random_y = util::generate_random_num(config::OBSTACLE_RANGE_Y + config::CACTUS_HEIGHT, config::OBSTACLE_RANGE_Y + config::OBSTACLE_RANGE_HEIGHT - config::CACTUS_HEIGHT);
		auto cactus = std::make_unique<entities::cactus>(entities::cactus(
			static_cast<float>(random_x), static_cast<float>(random_y)));
		
		while (not can_insert_obstacle(cactus->get_rectangle(), level_entities_)) {

			cactus->set_pos(static_cast<float>(util::generate_random_num(config::OBSTACLE_RANGE_X + config::CACTUS_WIDTH, config::OBSTACLE_RANGE_X + config::OBSTACLE_RANGE_WIDTH - config::CACTUS_WIDTH)),
				static_cast<float>(util::generate_random_num(config::OBSTACLE_RANGE_Y + config::CACTUS_HEIGHT, config::OBSTACLE_RANGE_Y + config::OBSTACLE_RANGE_HEIGHT - config::CACTUS_HEIGHT)));
		}

		level_entities_.insert(std::make_unique<entities::cactus>(*cactus.get()));
	}

// otherwise pick a random distance that is at least, the minimum distance away
}

void level::level::build_barrels(){
	int num_barrels = ceil(obstacles_to_generate_ * util::generate_random_num<double>(0.2, 0.4));
	obstacles_to_generate_ -= num_barrels;
	auto barrel_dimensions = Vector2{ config::BARREL_WIDTH, config::BARREL_HEIGHT };
	for (auto i = 0; i < num_barrels; ++i) {
		auto random_x = util::generate_random_num<float>(config::OBSTACLE_RANGE_X + config::BARREL_WIDTH, config::OBSTACLE_RANGE_X + config::OBSTACLE_RANGE_WIDTH - config::BARREL_WIDTH);
		auto random_y = util::generate_random_num<float>(config::OBSTACLE_RANGE_Y + config::BARREL_HEIGHT, config::OBSTACLE_RANGE_Y + config::OBSTACLE_RANGE_HEIGHT - config::BARREL_HEIGHT);
		auto barrel = std::make_unique<entities::barrel>(entities::barrel(
			static_cast<float>(random_x), static_cast<float>(random_y)));

		while (not can_insert_obstacle(barrel->get_rectangle(), level_entities_)) {

			barrel->set_pos(util::generate_random_num<float>(config::OBSTACLE_RANGE_X + config::BARREL_WIDTH, config::OBSTACLE_RANGE_X + config::OBSTACLE_RANGE_WIDTH - config::BARREL_WIDTH),
				util::generate_random_num<float>(config::OBSTACLE_RANGE_Y + config::BARREL_HEIGHT, config::OBSTACLE_RANGE_Y + config::OBSTACLE_RANGE_HEIGHT - config::BARREL_HEIGHT));
		}
		level_entities_.insert(std::make_unique<entities::barrel>(*barrel.get()));
	}
}

void level::level::build_wagons(){
	std::cout << "make wagons " << std::endl;
	int num_wagons = ceil(obstacles_to_generate_ * util::generate_random_num<double>(0.3, 0.6));
	obstacles_to_generate_ -= num_wagons;
	auto barrel_dimensions = Vector2{ config::WAGON_DOWN_WIDTH, config::WAGON_DOWN_HEIGHT };
	for (auto i = 0; i < num_wagons; ++i) {
		auto random_x = util::generate_random_num<float>(config::OBSTACLE_RANGE_X + config::WAGON_DOWN_WIDTH, config::OBSTACLE_RANGE_X + config::OBSTACLE_RANGE_WIDTH - config::WAGON_DOWN_WIDTH);
		auto random_y = util::generate_random_num<float>(config::OBSTACLE_RANGE_Y + config::WAGON_DOWN_HEIGHT, config::OBSTACLE_RANGE_Y + config::OBSTACLE_RANGE_HEIGHT - config::WAGON_DOWN_HEIGHT);
		auto wagon = std::make_unique<entities::wagon>(entities::wagon(
			static_cast<float>(random_x), static_cast<float>(random_y), 0.0, config::WAGON_SPEED));

		while (not can_insert_obstacle(wagon->get_rectangle(), level_entities_)) {

			wagon->set_pos(util::generate_random_num<float>(config::OBSTACLE_RANGE_X + config::WAGON_DOWN_WIDTH, config::OBSTACLE_RANGE_X + config::OBSTACLE_RANGE_WIDTH - config::WAGON_DOWN_WIDTH),
				util::generate_random_num<float>(config::OBSTACLE_RANGE_Y + config::WAGON_DOWN_HEIGHT, config::OBSTACLE_RANGE_Y + config::OBSTACLE_RANGE_HEIGHT - config::WAGON_DOWN_HEIGHT));
		}
		level_entities_.insert(std::make_unique<entities::wagon>(*wagon.get()));
	}
}

void level::level::build_train() {
	return;
}

std::set<std::unique_ptr<entities::entity>, decltype(util::cmp)>& level::level_builder::get_level_entities() {
	return level_entities_;
}
