#pragma once
#include <random>
#include <type_traits>
#include "entities.h"
namespace util{
	template<typename N>
	inline N generate_random_num(N min, N max) {
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_real_distribution<N> dis(min, max);
		return dis(gen);
	}
	inline int generate_random_int(int min, int max) {
		static std::random_device rd;
		static std::mt19937 gen(rd());
		std::uniform_int_distribution<int> dis(min, max);
		return dis(gen);
	}
	inline auto cmp = [](auto& a, auto& b) {
		return Vector2Length(a->get_position()) < Vector2Length(b->get_position());
		};
}