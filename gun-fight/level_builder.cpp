#include "level_builder.h"
#include "utility.h"
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
	// select random number between 0.0 and 0.6, it is the propeortion of total obstacles 
	// that will be tumbleweeds
	//int num_tumbleweed = ceil(obstacles_to_generate_ * util::generate_random_num(0.0, 0.6));
	
	// for initial testing for random positions, keep it static
	//TODO get a basic image for a tumbleweed and test the random generation
	int num_tumbleweed = 5;
	
	
	for (auto i = 0; i < num_tumbleweed; ++i) {
		auto random_x = util::generate_random_num(config::OBSTACLE_RANGE_X, config::OBSTACLE_RANGE_X + config::OBSTACLE_RANGE_WIDTH);
		auto random_y = util::generate_random_num(config::OBSTACLE_RANGE_Y, config::OBSTACLE_RANGE_Y + config::OBSTACLE_RANGE_HEIGHT);
		auto tumbleweed = std::make_unique<entities::tumbleweed>(entities::tumbleweed(
			static_cast<float>(random_x), static_cast<float>(random_y), config::TUMBLEWEED_PATH,
			config::TUMBLEWEED_HEALTH, config::TUMBLEWEED_CATEGORY));
		// case - there are no current obstacles
		if (level_entities_.empty()) {
			level_entities_.insert(std::make_unique<entities::tumbleweed>(*tumbleweed.get()));
		}
		else {
			auto insert = false;
			while (not insert) {
				auto upper = level_entities_.upper_bound(std::make_unique<entities::tumbleweed>(*tumbleweed.get())); 				// case - there is no lower bound
				// case - there is no lower
				if (upper == level_entities_.begin()) {
					// check the distance between two vectors
					auto distance_from_bound = Vector2Distance(tumbleweed->get_position(), (*upper)->get_position());
					insert = std::abs(distance_from_bound) >= config::MINIMUM_OBSTACLE_DISTANCE;
				}
				// case - there is no upper bound
				else if (upper == level_entities_.end()) {
					auto lower = --upper;
					auto distance_from_lower = Vector2Distance(tumbleweed->get_position(), (*lower)->get_position());

					// check distance, if too close. reroll the pos
					insert = std::abs(distance_from_lower) >= config::MINIMUM_OBSTACLE_DISTANCE;
				}
				// case - there is a lower and upper bound
				else {
					// get the lower bound 
					auto lower = --upper;
					// compare distance with both bounds 
					auto distance_from_lower = Vector2Distance(tumbleweed->get_position(), (*lower)->get_position());
					auto distance_from_upper = Vector2Distance(tumbleweed->get_position(), (*upper)->get_position());
					insert = std::abs(distance_from_lower) >= config::MINIMUM_OBSTACLE_DISTANCE
						and std::abs(distance_from_upper) >= config::MINIMUM_OBSTACLE_DISTANCE;
				}
				if (insert) {
					// insert the element
					level_entities_.insert(std::make_unique<entities::tumbleweed>(*tumbleweed.get()));
				}
				else {
					// otherwise reroll the posiition
					tumbleweed->set_pos(static_cast<float>(util::generate_random_num(config::OBSTACLE_RANGE_Y, config::OBSTACLE_RANGE_HEIGHT)),
						static_cast<float>(util::generate_random_num(config::OBSTACLE_RANGE_Y, config::OBSTACLE_RANGE_HEIGHT)));
				}
			}
		}
	}
	obstacles_to_generate_ -= num_tumbleweed;
}

void level::level::build_cacti(){
	int num_cacti = ceil(obstacles_to_generate_ * util::generate_random_num(0.3, 0.6));
	// if empty, just pick a random position and add the entity there

// otherwise pick a random distance that is at least, the minimum distance away
	obstacles_to_generate_ -= num_cacti;
}

void level::level::build_barrels(){
	int num_barrels = ceil(obstacles_to_generate_ * util::generate_random_num(0.3, 0.6));

	obstacles_to_generate_ -= num_barrels;
}

void level::level::build_wagons(){
	int num_wagons = ceil(obstacles_to_generate_ * util::generate_random_num(0.3, 0.6));

	obstacles_to_generate_ -= num_wagons;
}

void level::level::build_train() {
	return;
}

std::set<std::unique_ptr<entities::entity>>& level::level_builder::get_level_entities(){
	return level_entities_;
}
