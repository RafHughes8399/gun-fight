/*****************************************************************//**
 * \file   main.cpp
 * \brief  main game loop logic, the executable file
 * 
 * \author raffa
 * \date   February 2025
 *********************************************************************/
#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>
#include "raylib.h"
#include "entities.h"
#include "config.h"
#include "utility.h"
#include "level_builder.h"
#include "game_manager.h"
#include "player.h"
#include "screen.h"
#include "button.h"

static void init_game(game_manager& manager);
static void update_game(game_manager& manager);
static void draw_game(game_manager& manager);
static void unload_game();
static void update_draw_frame(game_manager& manager);
int main() {	
	/**  initalise the window */
	SetTargetFPS(60);
	InitWindow(config::SCREEN_WIDTH, config::SCREEN_HEIGHT, "gun_fight.exe");
	InitAudioDevice();
	/** make the gunman and weapon for both players */
	auto gunman_1 = std::make_shared<entities::gunman>(entities::gunman(config::P1_START_X, config::P1_START_Y, config::P1_PATH, 1, 1));
	auto gunamn_centre_x = gunman_1->get_x() + config::GUNMAN_WIDTH / 2;
	auto weapon_x = gunamn_centre_x + ((config::GUNMAN_WIDTH / 2) + config::BULLET_WIDTH) * gunman_1->get_direction();
	auto weapon_1 = std::make_shared<entities::revolver>(entities::revolver(weapon_x, gunman_1->get_y() + 45, config::REVOLVER_PATH));
	auto player_1 = player(gunman_1, weapon_1, config::GUNMAN1_MOVEMENT, config::GUNMAN1_FIRING, config::P1_ITEM_KEY,  150, config::P1_WIN_PATH);

	auto gunman_2 = std::make_shared<entities::gunman>(entities::gunman(config::P2_START_X, config::P2_START_Y, config::P2_PATH, 1, -1));
	gunamn_centre_x = gunman_2->get_x() + config::GUNMAN_WIDTH / 2;
	weapon_x = gunamn_centre_x + ((config::GUNMAN_WIDTH / 2) + config::BULLET_WIDTH) * gunman_2->get_direction();
	auto weapon_2 = std::make_shared<entities::revolver>(entities::revolver(weapon_x, gunman_2->get_y() + 45, config::REVOLVER_PATH));
	
	auto player_2 = player(gunman_2, weapon_2, config::GUNMAN2_MOVEMENT, config::GUNMAN2_FIRING, config::P2_ITEM_KEY, config::SCREEN_WIDTH - 150, config::P2_WIN_PATH);

	/**  create the game manager */
	auto manager = game_manager(player_1, player_2);
	/**  create the main menu buttons TODO add credits button */
	auto menu_buttons = std::vector<button>{
		button(config::PLAY_PATH, config::BUTTON_WIDTH, config::BUTTON_HEIGHT, config::SCREEN_WIDTH_HALF - (config::BUTTON_WIDTH / 2), config::BUTTONS_START_Y),
		button(config::CONTROLS_PATH, config::BUTTON_WIDTH, config::BUTTON_HEIGHT, config::SCREEN_WIDTH_HALF - (config::BUTTON_WIDTH / 2), config::BUTTONS_START_Y + config::BUTTON_HEIGHT + 50),
		button(config::QUIT_PATH, config::BUTTON_WIDTH, config::BUTTON_HEIGHT, config::SCREEN_WIDTH_HALF - (config::BUTTON_WIDTH / 2), config::BUTTONS_START_Y + (config::BUTTON_HEIGHT + 50) * 2)
	};
	/**  create the return button for the keyboard controls screen */
	auto control_buttons = std::vector<button>{
		button(config::RETURN_PATH, config::RETURN_WIDTH, config::RETURN_HEIGHT, 10, 10)
	};
	/**  create the main menu and controls screens */
	auto main_menu = screen(config::MENU_PATH, config::SCREEN_WIDTH, config::SCREEN_HEIGHT, 1, 0, 50, menu_buttons.begin(), menu_buttons.end(), std::make_unique<main_menu_strategy>(main_menu_strategy()));
	auto control_screen  = screen(config::CONTROL_SCREEN_PATH, config::SCREEN_WIDTH, config::SCREEN_HEIGHT, 1, 0, 0, control_buttons.begin(), control_buttons.end(), std::make_unique<return_strategy>(return_strategy()));
	/**  prevents abrupt exiting of the game with an exception */
	auto exit = false;
	/**  main game loop */
	while (not WindowShouldClose() and not exit) {
		/** check if a button on the main menu is prsesed */
		auto button = main_menu.update();
		switch (button) {
			/**  the play button */
			case 0: {
				/**  primary gameplay loop */
				/**  initialise the game  */
				init_game(manager);
				while (not WindowShouldClose() and not manager.game_over()) {
					if (manager.is_round_over()) {
						/**  reset the round if finished */
						manager.post_round();
						init_game(manager);
					}
					/**  update and draw the current frame */
					update_draw_frame(manager);
				}
				if (WindowShouldClose()) {
					break;
					CloseWindow();
				}
				/**
				 * after the game is over, play a game over screen
				 * go back to the main menu. 
				 */
				else {
					auto start_time = GetTime();
					// wait a second 
					WaitTime(1);
					manager.play_voiceline();
					while (GetTime() - start_time < 4.05) {
						BeginDrawing();
						manager.draw_background();
						manager.draw_players();
						manager.draw_win();
						EndDrawing();
					}
					manager.reset_scores();
					unload_game();
				}
				break;
			} 
			/**  keyboard controls screen */
			case 1: {// controls
				while (not WindowShouldClose()) {
					/**  check if the return button is pressed */
					control_screen.draw();
					auto button = control_screen.update();
					if (button == 0) {
						break;
					}
				}
				break;
			}
			/**  quit button */
			case 2: { // exit
				exit = true;
				break;
			}
			default: {
				break;
			}
		}
		main_menu.draw();
	}
	CloseAudioDevice();
	CloseWindow();
	return 1;
}

// --------------------- game updating, drawing and initalisation--------------------------------

void init_game(game_manager& manager) {
	manager.build_level();
	manager.pre_round();
}

// update the game by one frame
void update_game(game_manager& manager) {
	// temp for quickly cycling through rounds to test environment generation
	if (IsKeyPressed(KEY_X)) {
		manager.end_round();
	}

	// update players, check they are alive, increase scores, end the round
	manager.update_players();

	// check and spawn items if enough time has elapsed
	manager.spawn_items();
	// then update entnties
	manager.update_entities();

	// and remove them 
	manager.remove_entities();
	// then increase frame_count 
	manager.increment_frame_count();
}
void draw_game(game_manager& manager) {
	BeginDrawing();
	manager.draw_game();
	EndDrawing();
}

void unload_game(){
}
void update_draw_frame(game_manager& manager) {
	update_game(manager);
	draw_game(manager);
}
