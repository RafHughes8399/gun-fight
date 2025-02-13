/*****************************************************************//**
 * \file   config.h
 * \brief  config file that defines constant values for the game 
 * defines things like movement speed, screen dimension, controls, 
 * and other attributes for entities within the game
 * 
 * \author raffa
 * \date   February 2025
 *********************************************************************/
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
	// menu, background and hud paths
	inline const char* DEFAULT_PATH = "sprites/default.png";
	inline const char* CONTROL_SCREEN_PATH = "sprites/control-screen.png";
	inline const char* BACKGROUND_PATH = "sprites/background.png";
	inline const char* MENU_PATH = "sprites/background-full.png";
	inline const char* HUD_HEAD_PATH = "sprites/hud-header.png";
	inline const char* HUD_FOOT_PATH = "sprites/hud-footer.png";
	inline const char* HEART_PATH = "sprites/heart.png";
	
	// button attributes
	inline const char* PLAY_PATH = "sprites/play.png";
	inline const char* CONTROLS_PATH = "sprites/controls.png";
	inline const char* QUIT_PATH = "sprites/quit.png";
	inline const char* BUTTON_SOUND_PATH = "sounds/button-press.wav";
	inline const float BUTTON_WIDTH = 300;
	inline const float BUTTON_HEIGHT = 100.0;
	inline const float BUTTONS_START_Y = 350;
	
	inline const char* RETURN_PATH = "sprites/return.png";
	inline const float RETURN_WIDTH = 40;
	inline const float RETURN_HEIGHT = 40;

	inline const char* DRAW_PATH = "sprites/draw.png";
	inline const float DRAW_WIDTH = 300;
	inline const float DRAW_HEIGHT = 100.0;

	// heart attributes
	inline const float HEART_WIDTH = 60;
	inline const float HEART_HEIGHT = 60;
	inline const float HEART_SPACING = 10;

	// screen attributes
	inline const int SCREEN_HEIGHT = 1024;
	inline const int SCREEN_WIDTH = 1280;
	inline const int SCREEN_WIDTH_HALF = SCREEN_WIDTH / 2;
	inline const int SCREEN_HEIGHT_HALF = SCREEN_HEIGHT / 2;

	// bounds with hud 
	inline const int HUD_WIDTH = SCREEN_WIDTH;
	inline const int HUD_HEIGHT = 175;
	inline const int PLAYABLE_X = 0;
	inline const int PLAYABLE_Y = 50;
	inline const int PLAYABLE_WIDTH = SCREEN_WIDTH;
	inline const int PLAYABLE_HEIGHT = SCREEN_HEIGHT - HUD_HEIGHT - PLAYABLE_Y;

	// gunman attributes
	inline const float GUNMAN_SPEED = 2.2;
	inline const int GUNMAN_HEALTH = 1;
	inline const float GUNMAN_ANIMAITON_LENGTH = 15;
	inline const float GUNMAN_ANIMATIONS = 2;

	inline const char* P1_PATH = "sprites/gunman-revolver-left.png";
	inline const char* P2_PATH = "sprites/gunman-revolver-right.png";
	inline const float GUNMAN_HEIGHT = 111;
	inline const float GUNMAN_WIDTH = 52;

	inline const char* P1_DEAD_PATH = "sprites/gunman-dead-left.png";
	inline const char* P2_DEAD_PATH = "sprites/gunman-dead-right.png";
	
	inline const float GUNMAN_DEAD_WIDTH = GUNMAN_HEIGHT;
	inline const float GUNMAN_DEAD_HEIGHT = GUNMAN_WIDTH;

	// player attributes
	inline const float P1_START_X = 5;
	inline const float P1_START_Y = SCREEN_HEIGHT_HALF;	
	inline const float P2_START_X = SCREEN_WIDTH - GUNMAN_WIDTH - 5;
	inline const float P2_START_Y = SCREEN_HEIGHT_HALF;
	inline const int MAX_SCORE = 4;

	inline const char* P1_WIN_PATH = "sprites/p1-win.png";
	inline const char* P2_WIN_PATH = "sprites/p2-win.png";
	inline const float WIN_WIDTH = 650;
	inline const float WIN_HEIGHT= 100;
	
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


	inline static int P1_ITEM_KEY = KEY_E;
	inline static int P2_ITEM_KEY = KEY_SEMICOLON;
	//revolver attributes
	inline const int REVOLVER_AMMO = 5;
	inline const int REVOLVER_DAMAGE = 1;
	inline const int REVOLVER_PENETRATION = 0;
	inline const char* REVOLVER_PATH = "sprites/revolver-chamber.png";
	inline const float REVOLVER_ANIMATION_LENGTH = 5;
	inline const float REVOLVER_ANIMATIONS = 11;
	inline const float REVOLVER_WIDTH = 150;
	inline const float REVOLVER_HEIGHT = 150;
	inline const int REVOLVER_FIRE_RATE = 30;
	
	// bullet attributes;
	inline const float BULLET_WIDTH = 25;
	inline const float BULLET_HEIGHT = 12;
	inline const float BULLET_SPEED = 14;
	inline const char* BULLET_LEFT = "sprites/bullet-1.png";
	inline const char* BULLET_RIGHT = "sprites/bullet-2.png";
	// rifle attributes TODO: put in values 
	inline const int RIFLE_AMMO = 1;
	inline const int RIFLE_DAMAGE = 1;
	inline const int RIFLE_PENETRATION = 1;
	inline const char* RIFLE_PATH = "";
	inline const float RIFLE_ANIMATION_LENGTH = 1;
	inline const float RIFLE_ANIMATIONS = 1;
	inline const float RIFLE_WIDTH = 1;
	inline const float RIFLE_HEIGHT = 1;
	inline const int RIFLE_FIRE_RATE = 1;

	// rifle bullet attributes
	inline const float RIFLE_BULLET_SPEED = 0.0;
	inline const float RIFLE_BULLET_WIDTH = 0.0;
	inline const float RIFLE_BULLET_HEIGHT = 0.0;

	// dynamite attributes
	inline const int DYNAMITE_AMMO = 1;
	inline const int DYNAMITE_DAMAGE = 1;
	inline const int DYNAMITE_PENETRATION = 1;
	inline const char* DYNAMITE_PATH = "";
	inline const float DYNAMITE_ANIMATION_LENGTH = 1;
	inline const float DYNAMITE_ANIMATIONS = 1;
	inline const float DYNAMITE_WIDTH = 1;
	inline const float DYNAMITE_HEIGHT = 1;
	inline const int DYNAMITE_FIRE_RATE = 1;

	// dynamite stick attributes
	inline const float DYNAMITE_SPEED = 0.0;
	inline const float DYNAMITE_STICK_WIDTH = 0.0;
	inline const float DYNAMITE_STICK_HEIGHT = 0.0;
	
	inline const float DYNAMITE_DET_RADIUS = 0.0;
	inline const float DYNAMITE_TIMER = 0.0; 

	// obstacle attributes

	// tumbleweed 
	inline const int TUMBLEWEED_HEALTH = 1;
	inline const int TUMBLEWEED_CATEGORY = 0;
	inline const char* TUMBLEWEED_PATH = "sprites/tumbleweed.png";
	inline const int TUMBLEWEED_PENETRATION = 0;
	inline const float TUMBLEWEED_WIDTH = 64;
	inline const float TUMBLEWEED_HEIGHT = 64;
	inline const float TUMBLEWEED_ANIMATION_LENGTH = 19;
	inline const float TUMBLEWEED_ANIMATIONS = 2;
	inline const int TUMBLEWEED_SPEED = 4;
	inline const int TUMBLEWEED_LIFESPAN_LOWER = 300;
	inline const int TUMBLEWEED_LIFESPAN_UPPER = 450; // how many frames the tumbleweed will last, incorporate into the update method
	inline const int TUMBLEWEED_AMPLITUDE = 25;


	// cactus 

	inline const int CACTUS_HEALTH = 2;
	inline const int CACTUS_CATEGORY = 1;
	inline const char* CACTUS_PATH = "sprites/cactus.png";
	inline const int CACTUS_PENETRATION = 1;
	inline const float CACTUS_WIDTH = 90;
	inline const float CACTUS_HEIGHT = 150;
	inline const int CACTUS_ANIMATION_LENGTH = 2;
	inline const int CACTUS_ANIMATIONS = 1;


	// barrel
	inline const int BARREL_HEALTH = 3;
	inline const int BARREL_CATEGORY = 2;
	inline const char* BARREL_PATH = "sprites/barrel.png";
	inline const int BARREL_PENETRATION = 3;
	inline const float BARREL_WIDTH = 67;
	inline const float BARREL_HEIGHT = 91;
	inline const int BARREL_ANIMATION_LENGTH = 3;
	inline const int BARREL_ANIMATIONS = 1;


	// wagon
	inline const int WAGON_HEALTH = 5;
	inline const int WAGON_CATEGORY = 3;
	inline const char* WAGON_UP_PATH = "sprites/wagon-up.png";
	inline const char* WAGON_DOWN_PATH = "sprites/wagon-down.png";
	inline const int WAGON_PENETRATION = 3;
	inline const float WAGON_UP_WIDTH = 114;
	inline const float WAGON_UP_HEIGHT = 134;
	inline const float WAGON_DOWN_WIDTH = 114;
	inline const float WAGON_DOWN_HEIGHT = 170;
	inline const float WAGON_SPEED = 3.2;
	inline const int WAGON_ANIMATION_LENGTH = 10;
	inline const int WAGON_ANIMATIONS = 1;


	// train 
	inline const int TRAIN_HEALTH = 100;
	inline const int TRAIN_SPEED = 35;
	inline const int TRAIN_DELAY = 150; // the delay between trains travelling across the screen


	// strawman
	
	inline const int STRAWMAN_HEALTH = 1;
	inline const int STRAWMAN_CATEGORY = -1; // so it cannot be accidentally generated 
	inline const char*  STRAWMAN_LEFT_PATH = "sprites/strawman-left.png";
	inline const char*  STRAWMAN_RIGHT_PATH = "sprites/strawman-right.png";
	inline const int STRAWMAN_PENETRATION = 3;
	inline const float STRAWMAN_WIDTH = GUNMAN_WIDTH;
	inline const float STRAWMAN_HEIGHT = GUNMAN_HEIGHT;

	// obstacle generation bounds 
	inline const int OBSTACLE_RANGE_X = SCREEN_WIDTH_HALF - 200;
	inline const int OBSTACLE_RANGE_Y = PLAYABLE_Y;
	inline const int OBSTACLE_RANGE_WIDTH = 400;
	inline const int OBSTACLE_RANGE_HEIGHT = PLAYABLE_HEIGHT;
	inline const float MINIMUM_OBSTACLE_DISTANCE = 110; //subject to change

	// score numbers animation info
	inline const char* SCORE_PATH = "sprites/numbers.png";
	inline const float SCORE_WIDTH = 40;
	inline const float SCORE_HEIGHT = 40;
	inline const float SCORES_LENGTH = 11;
	inline const float	SCORES_ANIMATIONS = 1;

	//item spawn bounds, don't forget to subtract the item width and height when spawning
	inline const float P1_ITEM_SPAWN_X = 0;
	inline const float P1_ITEM_SPAWN_WIDTH = OBSTACLE_RANGE_X;
	inline const float P1_ITEM_SPAWN_Y = PLAYABLE_Y;
	inline const float P1_ITEM_SPAWN_HEIGHT = PLAYABLE_HEIGHT;
	
	inline const float P2_ITEM_SPAWN_X = OBSTACLE_RANGE_X + OBSTACLE_RANGE_WIDTH;
	inline const float P2_ITEM_SPAWN_WIDTH = SCREEN_WIDTH - P2_ITEM_SPAWN_X;
	inline const float P2_ITEM_SPAWN_Y = PLAYABLE_Y;
	inline const float P2_ITEM_SPAWN_HEIGHT = PLAYABLE_HEIGHT;

	// item pickup attributes
	inline const float ITEM_WIDTH = 50;
	inline const float ITEM_HEIGHT = 50;
	inline const char* HEALTH_PICKUP_PATH = "sprites/health-pickup.png";
	inline const char* ARMOUR_PICKUP_PATH = "sprites/armour-pickup.png";
	inline const char* RIFLE_PICKUP_PATH = "sprites/rifle-pickup.png";
	inline const char* DYNAMITE_PICKUP_PATH = "sprites/dynamite-pickup.png";
	inline const char* AMMO_PICKUP_PATH = "sprites/ammo-pickup.png";
	inline const double ITEM_SPAWN_DELAY = 10.5; // in seconds, 10.5 for testing purposes, should be longer in reality
	
	enum item_codes : int{
		HEALTH = 0,
		ARMOUR = 1,
		AMMO = 2,
		RIFLE = 3,
		DYNAMITE = 4
		// add more as needed
	};


	// sounds
	inline const char* DEATH_SOUND = "sounds/player-death.wav";
	inline const char* REVOLVER_FIRE_SOUND = "sounds/revolver-shoot.wav";
	inline const char* REVOLVER_RELOAD_SOUND = "sounds/revolver-reload.wav";
	inline const char* BULLET_HIT_SOUND = "sounds/bullet-hit.wav";
	inline std::vector<const char*> VOICE_LINES = {
		"sounds/this-town.wav",
		"sounds/fish-in-a-barrel.wav",
		"sounds/blindfolded.wav", 
		"sounds/slowest-shooter",
		"sounds/got-bullets.wav",
		 "sounds/for-free.wav",
		 "sounds/challenge.wav",
		 "sounds/hats-off.wav",
		 "sounds/one-handed.wav",
		 "sounds/card-counting.wav",
		 "sounds/hangman.wav"
	};
	
}
