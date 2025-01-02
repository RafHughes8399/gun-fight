#pragma once
#include "Color.hpp"
#include "Vector2.hpp"
#include <map>
namespace colours {
	constexpr Color night = { 10, 16, 13, 255 };
	constexpr Color fern_green = { 85, 111, 68, 255 };
	constexpr Color alice_blue = {};
	constexpr Color silver = {192, 192, 192, 255};
	constexpr Color crimson{217, 30, 54, 255};
	constexpr Color maize{244, 244, 77, 255};
	// add more colours here 
}

namespace config {

	// screen attributes
	const int SCREEN_HEIGHT = 1024;
	const int SCREEN_WIDTH = 1280;
	const int SCREEN_WIDTH_HALF = SCREEN_WIDTH / 2;
	const int SCREEN_HEIGHT_HALF = SCREEN_HEIGHT / 2;
	// gunman attributes
	const int GUNMAN_HEIGHT = 99;
	const int GUNMAN_WIDTH = 115;
	const float GUNMAN_SPEED = 2.5;

	const int GUNMAN_HEALTH = 1;

	const int GUNMAN_1_X = 50;
	const int GUNMAN_2_X = 1150;


	// need a set of movement_keys and directions
	static auto GUNMAN1_MOVEMENT = std::map<int, Vector2>{
		{KEY_W, Vector2{0,-GUNMAN_SPEED}},
		{KEY_A, Vector2{-GUNMAN_SPEED,0}},
		{KEY_S, Vector2{0,GUNMAN_SPEED}},
		{KEY_D, Vector2{GUNMAN_SPEED,0}}
	};
	static auto GUNMAN2_MOVEMENT = std::map<int, Vector2>{
		{KEY_I, Vector2{0,-GUNMAN_SPEED}},
		{KEY_J, Vector2{-GUNMAN_SPEED,0}},
		{KEY_K, Vector2{0,GUNMAN_SPEED}},
		{KEY_L, Vector2{GUNMAN_SPEED,0}}
	};

	static auto GUNMAN1_FIRING = std::pair<int, int>{ KEY_F, KEY_R };
	static auto GUNMAN2_FIRING = std::pair<int, int>{ KEY_H, KEY_U};


	//WEAPON ATTRIBUTES
	const int REVOLVER_AMMO = 6;
	const int REVOLVER_DAMAGE = 1;

	// bullet attributes;
	const float BULLET_SPEED = 8.5;

	// obstacle attributes
	const int TUMBLEWEED_HEALTH = 1;
	const int TUMBLEWEED_SPEED = 4;
	const int TUMBLEWEED_DURATION = 540; // how many frames the tumbleweed will last 
	const int TUMBLEWEED_CATEGORY = 1;
	const char* TUMBLEWEED_PATH = "sprites/default.png";

	const int CACTUS_HEALTH = 2;
	const int CACTUS_CATEGORY = 1;

	const int BARREL_HEALTH = 3;
	const int BARREL_CATEGORY = 2;

	const int WAGON_HEALTH = 3;
	const int WAGON_SPEED = 7;
	const int WAGON_CATEGORY = 3;

	const int TRAIN_HEALTH = 100;
	const int TRAIN_SPEED = 35;
	const int TRAIN_DELAY = 150; // the delay between trains travelling across the screen

	// obstacle bounds 
	const int OBSTACLE_RANGE_X = SCREEN_WIDTH_HALF - 200;
	const int OBSTACLE_RANGE_Y = 0;
	
	const int OBSTACLE_RANGE_WIDTH = 400;
	const int OBSTACLE_RANGE_HEIGHT = SCREEN_HEIGHT;

	const float MIMIUM_DISTANCE = 55.5;
}
