#include "level_builder.h"

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



	obstacles_to_generate_ -= num_tumbleweed;
}

void level::level::build_cacti(){
	int num_cacti = ceil(obstacles_to_generate_ + 0.6);

	obstacles_to_generate_ -= num_cacti;
}

void level::level::build_barrels(){

}

void level::level::build_wagons(){

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
