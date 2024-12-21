#include <iostream>
#include <vector>
#include <memory>
#include <utility>

#include "raylib.h"
#include "entities.h"
#include "weapons.h"
#include "config.h"

// ---------------- game variables --------------------
const int width = 1280;
const int height = 1024;
const int half_w = width / 2;
const int half_h = height / 2;
static int frame_count = 0;
static bool round_over = false;

static std::vector <std::unique_ptr<entities::entity>> game_entities = {};
static std::unique_ptr<entities::gunman> gunman1;
static std::unique_ptr<entities::gunman> gunman2;

// ------------ function declarations --------------
static void init_game();
static void update_game();
static void draw_game();
static void unload_game();
static void update_draw_frame();
std::vector <entities::entity*>  get_gunmen_and_obstacles(std::vector < std::unique_ptr<entities::entity>>& e);

int main() {	
	
	SetTargetFPS(60);
	// initialise the window and the game
	InitWindow(width, height, "gun fight yo");
	init_game();
	while (not WindowShouldClose()) {
		update_draw_frame();
	}
	unload_game();
	CloseWindow();
	return 1;
}

// --------------------- game updating, drawing and initalisation--------------------------------
void init_game() {
	gunman1 = std::make_unique<entities::gunman>(entities::gunman(50, half_h,"sprites/gunman-1.png", 1));
	gunman2 = std::make_unique<entities::gunman>(entities::gunman(1150, half_h, "sprites/gunman-2.png", 1));

	// populate the level

	// update other tracking variables
	frame_count = 0;
}

// update the game by one frame
void update_game() {
	// check for control inputs 
	// gunman movement and firing

	// player controls - gunman 1
	Vector2 movement;
	if (IsKeyDown(KEY_W)) {
		movement = {0, config::GUNMAN_SPEED * -1};
		gunman1->move(movement, width, height);
	}
	if (IsKeyDown(KEY_A)) {
		movement = { config::GUNMAN_SPEED * -1, 0};
		gunman1->move(movement, width, height);
	}	
	if (IsKeyDown(KEY_S)) {
		movement = {0, config::GUNMAN_SPEED};
		gunman1->move(movement, width, height);
	}	
	if (IsKeyDown(KEY_D)) {
		movement = {config::GUNMAN_SPEED, 0};
		gunman1->move(movement, width, height);
	}
	if (IsKeyPressed(KEY_R)) {
		gunman1->get_weapon()->reload();
	}

	if (IsKeyPressed(KEY_F)) {
		if (gunman1->get_weapon()->fire()) {
			game_entities.push_back(std::make_unique<entities::bullet>(entities::bullet(gunman1->get_x(), gunman1->get_y() + 50, "sprites/bullet-1.png", 1, gunman1->get_weapon())));
		}
	}
	// player controls - gunman 2
	if (IsKeyDown(KEY_I)) {
		movement = {0,config::GUNMAN_SPEED * -1};
		gunman2->move(movement, width, height);
	}
	if (IsKeyDown(KEY_J)) {
		movement = {config::GUNMAN_SPEED * -1, 0};
		gunman2->move(movement, width, height);
	}	
	if (IsKeyDown(KEY_K)) {
		movement = {0, config::GUNMAN_SPEED};
		gunman2->move(movement, width, height);
	}	
	if (IsKeyDown(KEY_L)) {
		movement = {config::GUNMAN_SPEED, 0};
		gunman2->move(movement, width, height);
	}
	if (IsKeyPressed(KEY_U)) {
		gunman2->get_weapon()->reload();
	}

	if (IsKeyPressed(KEY_H)) {
		if (gunman2->get_weapon()->fire()) {
			game_entities.push_back(std::make_unique<entities::bullet>(entities::bullet(gunman2->get_x(), gunman2->get_y() + 50, "sprites/bullet-2.png", -1, gunman1->get_weapon())));
		}
	}	

	gunman1->update();
	gunman2->update();
	for (auto it = game_entities.begin(); it != game_entities.end();) {
		if (not (*it)->update()) {
			it = game_entities.erase(it);
		}
		else {
			// check collision
			for (auto it2 = game_entities.begin(); it2 != game_entities.end(); ++it2) {
				if (it != it2) {
					if (CheckCollisionRecs((*it)->get_rectangle(), (*it2)->get_rectangle())) {
						(*it)->collide(*it2);
					}
				}
				else {
					std::cout << "same iterator" << std::endl;
				}
			}
			++it;
		}
	}
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


// --------------------- other helper functions --------------------------------
std::vector <entities::entity*> get_gunmen_and_obstacles(std::vector < std::unique_ptr<entities::entity>>& e) {
	auto gunmen_obstacles = std::vector<entities::entity*>{};
	for (std::unique_ptr<entities::entity>& current : e) {
		// try to dynamic cast to a gunman or obstacle
		entities::gunman* gunman = dynamic_cast<entities::gunman*>(current.get());
		if (gunman != nullptr) { 
			gunmen_obstacles.push_back(gunman); 
			continue;
		}
		entities::obstacle* obstacle = dynamic_cast<entities::obstacle*>(current.get());
		if (obstacle != nullptr) {
			gunmen_obstacles.push_back(obstacle);
			continue;
		}
	}
	return gunmen_obstacles;
}
