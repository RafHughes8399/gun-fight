#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>

#include "raylib.h"
#include "entities.h"
#include "weapon.h"
#include "config.h"
#include "utility.h"
#include "level_builder.h"
#include "game_manager.h"
#include "player.h"
#include "main_menu.h"
// ---------------- game variables --------------------


// ------------ function declarations --------------
static void init_game(game_manager& manager);
static void update_game(game_manager& manager);
static void draw_game(game_manager& manager);
static void unload_game();
static void update_draw_frame(game_manager& manager);
int main() {	
	SetTargetFPS(50);
	// initialise the window and the game
	InitWindow(config::SCREEN_WIDTH, config::SCREEN_HEIGHT, "gun_fight.exe");

	// make the gunmen and weapons
	auto gunman_1 = std::make_shared<entities::gunman>(entities::gunman(config::P1_START_X, config::P1_START_Y, config::P1_PATH, 1, 1));
	auto gunamn_centre_x = gunman_1->get_x() + config::GUNMAN_WIDTH / 2;
	auto weapon_x = gunamn_centre_x + ((config::GUNMAN_WIDTH / 2) + config::BULLET_WIDTH) * gunman_1->get_direction();
	auto weapon_1 = std::make_shared<entities::revolver>(entities::revolver(weapon_x, gunman_1->get_y() + 45, config::REVOLVER_PATH));
	auto player_1 = player(gunman_1, weapon_1, config::GUNMAN1_MOVEMENT, config::GUNMAN1_FIRING, 150);

	auto gunman_2 = std::make_shared<entities::gunman>(entities::gunman(config::P2_START_X, config::P2_START_Y, config::P2_PATH, 1, -1));
	gunamn_centre_x = gunman_2->get_x() + config::GUNMAN_WIDTH / 2;
	weapon_x = gunamn_centre_x + ((config::GUNMAN_WIDTH / 2) + config::BULLET_WIDTH) * gunman_2->get_direction();
	auto weapon_2 = std::make_shared<entities::revolver>(entities::revolver(weapon_x, gunman_2->get_y() + 45, config::REVOLVER_PATH));
	
	auto player_2 = player(gunman_2, weapon_2, config::GUNMAN2_MOVEMENT, config::GUNMAN2_FIRING, config::SCREEN_WIDTH - 150);

	auto manager = game_manager(player_1, player_2);

	auto menu = main_menu();
	auto exit = false;
	while (not WindowShouldClose() and not exit) {
	// draw the main menu
		auto button = menu.update();
		switch (button) {
			case 0: // play
				init_game(manager);
				while (not manager.game_over()) {
					if (manager.is_round_over()) {
						init_game(manager);
					}
					update_draw_frame(manager);
				}
				unload_game();
				//TODO return to main menu
				break;
			case 1: // controls
				exit = true;
				break;
			case 2: // exit
				exit = true;
				break;
			default:
				break;
				// do nothing
			}
		menu.draw();
	}
	CloseWindow();
	return 1;
}

// --------------------- game updating, drawing and initalisation--------------------------------
void init_game(game_manager& manager) {
	manager.build_level();
}

// update the game by one frame
void update_game(game_manager& manager) {
	// temp for quickly cycling through rounds to test environment generation
	if (IsKeyPressed(KEY_X)) {
		manager.end_round();
	}
	// update players, check they are alive, increase scores, end the round
	manager.update_players();

	// then update entnties
	manager.update_entities();

	// and remove them 
	manager.remove_entities();
	// then increase frame_count 
	manager.increment_frame_count();
}
void draw_game(game_manager& manager) {
	BeginDrawing();
	manager.draw_background();
	manager.draw_scores();
	// draw players, and their scores
	manager.draw_players();
	manager.draw_entities();
	EndDrawing();
}

void unload_game(){
}
void update_draw_frame(game_manager& manager) {
	update_game(manager);
	draw_game(manager);
}
