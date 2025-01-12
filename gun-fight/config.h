#pragma once
#include "raylib.h"
#include <map>
#include <random>
namespace colours {
	inline constexpr Color night = { 10, 16, 13, 255 };
	inline constexpr Color fern_green = { 85, 111, 68, 255 };
	inline constexpr Color alice_blue = {};
	inline constexpr Color silver = { 192, 192, 192, 255 };
	inline constexpr Color crimson{ 217, 30, 54, 255 };
	inline constexpr Color maize{ 244, 244, 77, 255 };
	inline constexpr Color redwood{173, 93, 78, 255};
	// add more colours here 
}

namespace config {

	// screen attributes
	inline const int SCREEN_HEIGHT = 1024;
	inline const int SCREEN_WIDTH = 1280;
	inline const int SCREEN_WIDTH_HALF = SCREEN_WIDTH / 2;
	inline const int SCREEN_HEIGHT_HALF = SCREEN_HEIGHT / 2;
	// gunman attributes
	inline const int GUNMAN_HEIGHT = 115;
	inline const int GUNMAN_WIDTH = 99;
	inline const float GUNMAN_SPEED = 2.5;
	inline const int GUNMAN_HEALTH = 1;
	inline const int GUNMAN_1_X = 50;
	inline const int GUNMAN_2_X = 1150;


	// need a set of movement_keys and directions
	inline static auto GUNMAN1_MOVEMENT = std::map<int, Vector2>{
		{KEY_W, Vector2{0,-GUNMAN_SPEED}},
		{KEY_A, Vector2{-GUNMAN_SPEED,0}},
		{KEY_S, Vector2{0,GUNMAN_SPEED}},
		{KEY_D, Vector2{GUNMAN_SPEED,0}}
	};
	inline static auto GUNMAN2_MOVEMENT = std::map<int, Vector2>{
		{KEY_UP, Vector2{0,-GUNMAN_SPEED}},
		{KEY_LEFT, Vector2{-GUNMAN_SPEED,0}},
		{KEY_DOWN, Vector2{0,GUNMAN_SPEED}},
		{KEY_RIGHT, Vector2{GUNMAN_SPEED,0}}
	};

	inline static auto GUNMAN1_FIRING = std::pair<int, int>{ KEY_F, KEY_R };
	inline static auto GUNMAN2_FIRING = std::pair<int, int>{ KEY_COMMA, KEY_PERIOD};


	//WEAPON ATTRIBUTES
	inline const int REVOLVER_AMMO = 5;
	inline const int REVOLVER_DAMAGE = 1;
	inline const int REVOLVER_PENETRATION = 0;
	inline const char* REVOLVER_ANIMATION_PATH = "sprites/revolver-chamber.png";
	inline const int REVOLVER_SHEET_WIDTH = 750;
	inline const int REVOLVER_SHEET_HEIGHT = 1650;
	inline const int REVOLVER_FRAME_WIDTH = 5;
	inline const int REVOLVER_FRAME_HEIGHT = 11;
	inline const int REVOLVER_FIRE_RATE = 30;
	
	// bullet attributes;
	inline const float BULLET_WIDTH = 20;
	inline const float BULLET_HEIGHT = 9;
	inline const float BULLET_SPEED = 8.5;

	// obstacle attributes
	inline const int TUMBLEWEED_HEALTH = 1;
	inline const int TUMBLEWEED_CATEGORY = 0;
	inline const char* TUMBLEWEED_PATH = "sprites/tumbleweed.png";
	inline const int TUMBLEWEED_PENETRATION = 0;
	inline const float TUMBLEWEED_WIDTH = 50.0;
	inline const float TUMBLEWEED_HEIGHT = 46.0;
	inline const int TUMBLEWEED_SPEED = 4;
	inline const int TUMBLEWEED_DURATION = 540; // how many frames the tumbleweed will last 

	inline const int CACTUS_HEALTH = 2;
	inline const int CACTUS_CATEGORY = 1;
	inline const char* CACTUS_PATH = "sprites/cactus.png";
	inline const int CACTUS_PENETRATION = 1;
	inline const float CACTUS_WIDTH = 90;
	inline const float CACTUS_HEIGHT = 150;


	inline const int BARREL_HEALTH = 3;
	inline const int BARREL_CATEGORY = 2;
	inline const char* BARREL_PATH = "sprites/barrel.png";
	inline const int BARREL_PENETRATION = 3;
	inline const float BARREL_WIDTH = 32;
	inline const float BARREL_HEIGHT = 32;

	inline const int WAGON_HEALTH = 3;
	inline const int WAGON_CATEGORY = 3;
	inline const char* WAGON_UP_PATH = "sprites/barrel.png";
	inline const char* WAGON_DOWN_PATH = "sprites/barrel.png";
	inline const int WAGON_PENETRATION = 3;
	inline const float WAGON_WIDTH = 0;
	inline const float WAGON_HEIGHT = 0;
	inline const float WAGON_SPEED = 7;

	inline const int TRAIN_HEALTH = 100;
	inline const int TRAIN_SPEED = 35;
	inline const int TRAIN_DELAY = 150; // the delay between trains travelling across the screen

	// obstacle bounds 
	inline const int OBSTACLE_RANGE_X = SCREEN_WIDTH_HALF - 200;
	inline const int OBSTACLE_RANGE_Y = 0;
	inline const int OBSTACLE_RANGE_WIDTH = 400;
	inline const int OBSTACLE_RANGE_HEIGHT = SCREEN_HEIGHT;
	inline const float MINIMUM_OBSTACLE_DISTANCE = 225; //subject to change

	
}
