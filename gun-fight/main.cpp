#include <iostream>
#include <vector>
#include <memory>
#include <utility>

#include "raylib.h"
#include "entities.h"
const int width = 1280;
const int height = 1024;
const int half_w = width / 2;
const int half_h = height / 2;
	
// game handling methods init
static void init_game();
static void update_game();
static void draw_game();
static void unload_game();
static void update_draw_frame();

// one instance shared between everything
static std::vector <std::unique_ptr<entities::entity>> game_entities = {};
static std::unique_ptr<entities::gunman> gunman1;
static std::unique_ptr<entities::gunman> gunman2;

namespace colours {
	constexpr Color night = { 10, 16, 13, 255 };
	// add more colours here 
}

int main() {	
	
	SetTargetFPS(60);
	// initialise the window and the game
	InitWindow(width, height, "gun fight yo");
	init_game();
	// custom colour definitions


	// need a list of entities so they can be managed and updated
	// i have the image path in the object
	// i need to load the texture and then draw it, but only need to load once (for now)

	// i need some way to update the height and width
	auto gt1 = LoadTexture(gunman1->get_path());
	auto gt2 = LoadTexture(gunman2->get_path());

	// say a cactus has 3 health, and the image displayed is the full health one
	// it gets shot, now has 2 health, then the image displayed is the middle health one (use arrays ig)
	
	
	// each loop iteration is a frame
	// each frame must be drawn and updated
	while (not WindowShouldClose()) {
		update_draw_frame();
	}
	unload_game();
	CloseWindow();
	return 1;
}

// initialise game variables

// includes a frame counter
// a game over check
// the gunmen and the entities

// setup everything for frame 0
void init_game() {
	gunman1 = std::make_unique<entities::gunman>(entities::gunman(50, half_h, 10, 10,"sprites/gunman-1.png", 1));
	gunman2 = std::make_unique<entities::gunman>(entities::gunman(1000, half_h, 10, 10,"sprites/gunman-2.png", 1));

	// populate the level
}

// update the game by one frame
void update_game() {
	// check for control inputs 
	// gunman movement and firing

}

void draw_game() {
	BeginDrawing();
	ClearBackground(colours::night);
	// draw gunmen
	gunman1->draw();
	gunman2->draw();
	// draw every entity
	for (std::unique_ptr<entities::entity>& e : game_entities) {
		e->draw();
	}
	EndDrawing();
}

void unload_game() {
}
void update_draw_frame() {
	update_game();
	draw_game();
}
