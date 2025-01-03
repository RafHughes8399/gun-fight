#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>

#include "raylib.h"
#include "entities.h"
#include "weapons.h"
#include "config.h"
#include "utility.h"
#include "level_builder.h"
// ---------------- game variables --------------------
static int frame_count = 0;
static int round_count = 0;
static bool round_over = false;

static std::vector<std::unique_ptr<entities::entity>> game_entities = {};
static entities::gunman* gunman1;
static entities::gunman* gunman2;
// ------------ function declarations --------------
static void init_game();
static void update_game();
static void draw_game();
static void unload_game();
static void update_draw_frame();
static void build_level();
static void clear_environment();
int main() {	
	SetTargetFPS(60);
	// initialise the window and the game
	InitWindow(config::SCREEN_WIDTH, config::SCREEN_HEIGHT, "gun_fight.exe");
	game_entities.push_back(std::make_unique<entities::gunman>(entities::gunman(50, config::SCREEN_HEIGHT_HALF, "sprites/gunman-1.png", 1, config::GUNMAN1_MOVEMENT, config::GUNMAN1_FIRING)));
	gunman1 = static_cast<entities::gunman*>(game_entities.back().get());
	game_entities.push_back(std::make_unique<entities::gunman>(entities::gunman(1150, config::SCREEN_HEIGHT_HALF, "sprites/gunman-2.png", 1, config::GUNMAN2_MOVEMENT, config::GUNMAN2_FIRING)));
	gunman2 = static_cast<entities::gunman*>(game_entities.back().get());
	
	init_game();
	while (not WindowShouldClose()) {
		if (round_over) {
			init_game();
		}
		update_draw_frame();
	}
	unload_game();
	CloseWindow();
	return 1;
}

// --------------------- game updating, drawing and initalisation--------------------------------
void init_game() {
	// clear the environment, keep the gunmen
	// oh because it doesn't clear the envuronment
	clear_environment();
	build_level();
	// reset both gunmen, health, ammo, position, items, 
	gunman1->reset(config::GUNMAN_1_X, config::SCREEN_HEIGHT_HALF);
	gunman2->reset(config::GUNMAN_2_X, config::SCREEN_HEIGHT_HALF);
	// populate the level - start with static then move onto randmon generatio
	
	// update other tracking variables
	frame_count = 0;
	++round_count;
	round_over = false;

}

// update the game by one frame
void update_game() {
	// temp for quickly cycling through rounds to test environemnt generation
	if (IsKeyPressed(KEY_X)) {
		round_over = true;
	}
	if (not gunman1->update(game_entities)) {
		round_over = true;
		gunman2->win_point();
	}	
	if (not gunman2->update(game_entities)) {
		round_over = true;
		gunman1->win_point();
	}

	for (auto& e : game_entities) {
		// if e is not a gunman
		auto gunman_ptr = dynamic_cast<entities::gunman*>(e.get());
		if (gunman_ptr == nullptr) {
			e->update(game_entities);
		}
	}

	auto new_end = std::remove_if(game_entities.begin(), game_entities.end(), [](auto& e) {
		return e->get_remove();
		});
	game_entities.erase(new_end, game_entities.end());
}
void draw_game() {
	BeginDrawing();
	ClearBackground(colours::night);
	std::for_each(game_entities.begin(), game_entities.end(), [](auto& e) {e->draw();});
	DrawText(std::to_string(gunman1->get_score()).c_str(), (config::SCREEN_WIDTH / 4), 50, 36, colours::maize);
	DrawText(std::to_string(gunman2->get_score()).c_str(), (config::SCREEN_WIDTH * 3 / 4), 50, 36, colours::maize);

	DrawRectangleLines(config::SCREEN_WIDTH_HALF - 200, 0, 400, config::SCREEN_HEIGHT, colours::maize);
	EndDrawing();
	++frame_count;
}

void unload_game() {

}
void update_draw_frame() {
	update_game();
	draw_game();
}

// helper functions 
static void clear_environment() {
	auto new_end = std::remove_if(game_entities.begin(), game_entities.end(), [](auto& e) {
		auto g_ptr = dynamic_cast<entities::gunman*>(e.get());
		return g_ptr == nullptr;
		});
	game_entities.erase(new_end, game_entities.end());
	return;
}

static void build_level() {
	// get a random number for level category
	//TODO check if should be train level

	auto level_category = static_cast<int>(util::generate_random_num(1, 3));
	auto obstacles_to_generate = 7; // TODO put in formula, might need tweaking
	auto builder = std::make_unique<level::level>(level::level(level_category, obstacles_to_generate));
	builder->build_level();

	// transfer the obstacles
	// iterator invalidation
	//for (auto& entity : builder->get_level_entities()) {
	//	game_entities.push_back(std::move(entity));
	//}
	// generate the level
	// 
	// put the new entities in the current list
	// 	

}