#pragma once
#include <random>
namespace util{
	inline double generate_random_num(double min, double max) {
		std::random_device rd;
		std::mt19937 gen(rd());
	
		std::uniform_real_distribution<double> dis(min, max);
		return dis(gen);
	}
}