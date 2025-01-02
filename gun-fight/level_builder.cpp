#include "level_builder.h"

//TODO make this a set of obstacles, be able to compare obstacles based on vector length (from origin)
void level::level::build_level(){
	if (level_category_ >= config::TUMBLEWEED_CATEGORY and obstacles_to_generate_ > 0) {
		build_tumbleweed();
	}
	if (level_category_ >= config::CACTUS_CATEGORY and obstacles_to_generate_ > 0) {
		build_cacti();
	}
	if (level_category_ >= config::BARREL_CATEGORY and obstacles_to_generate_ > 0) {
		build_barrels();
	}
	if (level_category_ >= config::WAGON_CATEGORY and obstacles_to_generate_ > 0) {
		build_wagons();
	}
}

void level::level::build_tumbleweed(){
	// select random number between 0.4 and 0.6

	int num_tumbleweed = ceil(obstacles_to_generate_ * generate_random_num(0.3, 0.6));
	// check the list of entities in the level
	// just check the most recent one lets keep it simple for now and see what it does
	// if empty, just pick a random position and add the entity there
	for (auto i = 0; i < num_tumbleweed; ++i) {
		auto random_pos = Vector2{static_cast<float>(generate_random_num(config::OBSTACLE_RANGE_X, config::OBSTACLE_RANGE_X + config::OBSTACLE_RANGE_WIDTH)), static_cast<float>(generate_random_num(config::OBSTACLE_RANGE_Y, config::OBSTACLE_RANGE_HEIGHT))};
		auto tumbleweed = std::make_unique<entities::tumbleweed>(entities::tumbleweed(random_pos.x, random_pos.y, config::TUMBLEWEED_PATH, config::TUMBLEWEED_HEALTH));
		if (level_entities_.empty()) {
			level_entities_.insert(std::move(tumbleweed));
		}
		else {
			// make a copy of the pointer 
			bool inserted = false; 
			while (not inserted) {
				// get the bounds

				// check the bounds 

				// if in the bounds add

				// otherwise generate a new position
			
			}
			auto lower = level_entities_.lower_bound(std::make_unique<entities::tumbleweed>(*tumbleweed.get()));
			auto upper = level_entities_.lower_bound(std::make_unique<entities::tumbleweed>(*tumbleweed.get())); // maybe need to do copy
		}
	}
	// otherwise pick a random distance that is at least, the minimum distance away

	// for all num_tumbleweed
		// geerate a random position 
		// 
		// make the obstacle
		// if empty, add element

		// otherwise get upper and lower bound for the obstaacle

		// check lower, if exists make sure it is minimum away
		// check upper, if exists make sure it is minimum away
		

		// if valid, add the obstacle, otherwise generate a new position

	obstacles_to_generate_ -= num_tumbleweed;
}

void level::level::build_cacti(){
	int num_cacti = ceil(obstacles_to_generate_ * generate_random_num(0.3, 0.6));
	// if empty, just pick a random position and add the entity there

// otherwise pick a random distance that is at least, the minimum distance away
	obstacles_to_generate_ -= num_cacti;
}

void level::level::build_barrels(){
	int num_barrels = ceil(obstacles_to_generate_ * generate_random_num(0.3, 0.6));

	obstacles_to_generate_ -= num_barrels;
}

void level::level::build_wagons(){
	int num_wagons = ceil(obstacles_to_generate_ * generate_random_num(0.3, 0.6));

	obstacles_to_generate_ -= num_wagons;
}

void level::level::build_train(){
	return;
}

double level::level_builder::generate_random_num(double min, double max){
	std::random_device rd;
	std::mt19937 gen(rd());

	std::uniform_real_distribution<double> dis(min, max);
	return dis(gen);
}
