#include <iostream>
#include <vector>
#include <memory>
#include <utility>
#include <algorithm>

#include "raylib.h"
#include "entities.h"
#include "weapons.h"
#include "config.h"

// ---------------- game variables --------------------
static int frame_count = 0;
static int round_count = 0;
static bool round_over = false;

static std::vector<std::unique_ptr<entities::entity>> game_entities = {};
static std::unique_ptr<entities::gunman> gunman1;
static std::unique_ptr<entities::gunman> gunman2;

// ------------ function declarations --------------
static void init_game();
static void update_game();
static void draw_game();
static void unload_game();
static void update_draw_frame();
bool is_moving(int gunman);
int main() {	
	
	SetTargetFPS(60);
	// initialise the window and the game
	InitWindow(config::SCREEN_WIDTH, config::SCREEN_HEIGHT, "gun fight yo");
	init_game();
	while (not WindowShouldClose()) {
		update_draw_frame();
		++frame_count;

		if (round_over) {
			init_game();
		}
	}
	unload_game();
	CloseWindow();
	return 1;
}

// --------------------- game updating, drawing and initalisation--------------------------------
void init_game() {
	game_entities.clear();
	gunman1 = std::make_unique<entities::gunman>(entities::gunman(50, config::SCREEN_HEIGHT_HALF,"sprites/gunman-1.png", 1, config::GUNMAN1_MOVEMENT, config::GUNMAN1_FIRING));
	gunman2 = std::make_unique<entities::gunman>(entities::gunman(1150, config::SCREEN_HEIGHT_HALF, "sprites/gunman-2.png", 1, config::GUNMAN2_MOVEMENT, config::GUNMAN2_FIRING));

	
	// populate the level - start with static then move onto randmon generatio
	
	// update other tracking variables
	frame_count = 0;
	round_over = false;
}

// update the game by one frame
void update_game() {
	gunman1->update(game_entities);
	gunman2->update(game_entities);
	std::remove_if(game_entities.begin(), game_entities.end(), [](std::unique_ptr<entities::entity>& entity) {
		return entity->update(game_entities);
		});

}
void draw_game() {
	BeginDrawing();
	ClearBackground(colours::night);
	// draw gunmen
	gunman1->draw();
	gunman2->draw();
	// draw every entity
	for (auto& e : game_entities) {
		e->draw();
	}
	DrawText(std::to_string(gunman1->get_score()).c_str(), (config::SCREEN_WIDTH / 4), 50, 36, colours::maize);
	DrawText(std::to_string(gunman2->get_score()).c_str(), (config::SCREEN_WIDTH * 3 / 4), 50, 36, colours::maize);
	EndDrawing();
}

void unload_game() {
}
void update_draw_frame() {
	update_game();
	draw_game();
}
