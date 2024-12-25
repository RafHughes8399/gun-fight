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
static bool game_over = false;

static std::vector<std::unique_ptr<entities::entity>> game_entities = {};
static std::vector<std::unique_ptr<entities::projectile>> game_projectiles = {};
static std::vector<std::unique_ptr<entities::obstacle>> game_obstacles = {};
static std::vector<std::unique_ptr<entities::pickup>> game_pickups = {};
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
	InitWindow(width, height, "gun fight yo");
	init_game();
	while (not WindowShouldClose() and not game_over) {
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

	// populate the level - start with static then move onto randmon generatio
	
	// update other tracking variables
	frame_count = 0;
	game_over = false;
}

// update the game by one frame
void update_game() {
	// check for control inputs 
	// gunman movement and firing

	// player controls - gunman 1
	Vector2 movement;
	if (IsKeyDown(KEY_W)) {
		movement = { 0, config::GUNMAN_SPEED * -1 };
		gunman1->move(movement, width, height);
	}
	if (IsKeyDown(KEY_A)) {
		movement = { config::GUNMAN_SPEED * -1, 0 };
		gunman1->move(movement, width, height);
	}
	if (IsKeyDown(KEY_S)) {
		movement = { 0, config::GUNMAN_SPEED };
		gunman1->move(movement, width, height);
	}
	if (IsKeyDown(KEY_D)) {
		movement = { config::GUNMAN_SPEED, 0 };
		gunman1->move(movement, width, height);
	}
	if (IsKeyPressed(KEY_R)) {
		gunman1->get_weapon()->reload();
	}

	if (IsKeyPressed(KEY_F) and not is_moving(1)) {
		if (gunman1->get_weapon()->fire()) {
			game_projectiles.push_back(std::make_unique<entities::bullet>(entities::bullet(gunman1->get_x() + 95, gunman1->get_y() + 45, "sprites/bullet-1.png", 1, gunman1->get_weapon())));
		}
	}
	// player controls - gunman 2
	if (IsKeyDown(KEY_I)) {
		movement = { 0,config::GUNMAN_SPEED * -1 };
		gunman2->move(movement, width, height);
	}
	if (IsKeyDown(KEY_J)) {
		movement = { config::GUNMAN_SPEED * -1, 0 };
		gunman2->move(movement, width, height);
	}
	if (IsKeyDown(KEY_K)) {
		movement = { 0, config::GUNMAN_SPEED };
		gunman2->move(movement, width, height);
	}
	if (IsKeyDown(KEY_L)) {
		movement = { config::GUNMAN_SPEED, 0 };
		gunman2->move(movement, width, height);
	}
	if (IsKeyPressed(KEY_U)) {
		gunman2->get_weapon()->reload();
	}

	if (IsKeyPressed(KEY_H) and not is_moving(2)) {
		if (gunman2->get_weapon()->fire()) {
			game_projectiles.push_back(std::make_unique<entities::bullet>(entities::bullet(gunman2->get_x() - 20, gunman2->get_y() + 45, "sprites/bullet-2.png", -1, gunman1->get_weapon())));
		}
	}
	// update entities 
	gunman1->update();
	gunman2->update();

	// pickups
	for (auto it = game_pickups.begin(); it != game_pickups.end();) {
		if (not (*it)->update()) {
			it = game_pickups.erase(it);
		}
		else {
			++it; // DO NOT DELETE THIS LINE YOU SILLY GOOBER 
		}
	}	
	// obstacles
	for (auto it = game_obstacles.begin(); it != game_obstacles.end();) {
		if (not (*it)->update()) {
			it = game_obstacles.erase(it);
		}
		else {
			++it; // DO NOT DELETE THIS LINE YOU SILLY GOOBER 
		}
	}
	for (auto it = game_projectiles.begin(); it != game_projectiles.end();) {
		if (not (*it)->update()) {
			it = game_projectiles.erase(it);
		}
		else {
			++it; // DO NOT DELETE THIS LINE YOU SILLY GOOBER 
		}
	}
	// do collision from the perspective of the gunman
	// if gunman collides with bullet call bullet.collide(gunman)
	// this should handle projectiles, obstacles and pickups 
	// gunmen projectile collision
	for (auto it = game_projectiles.begin(); it != game_projectiles.end();) {
		auto e = (*it).get();
		bool remove_proj = false;
		if (CheckCollisionRecs(gunman1->get_rectangle(), e->get_rectangle())) {
			remove_proj = true;
			if (not e->collide(*gunman1)) {
				game_over = true;
			}
			else {
				continue;
			
			}
		}
		
		else if (CheckCollisionRecs(gunman2->get_rectangle(), e->get_rectangle())) {
			remove_proj = true;
			if (not e->collide(*gunman2)) {
				game_over = true;
			}
			else {
				continue;
			}
		}
		// projectile obstacle collision
		else {
			for (auto it2 = game_obstacles.begin(); it2 != game_obstacles.end();) {
				auto e2 = (*it2).get();
				bool remove_obs = false;
				if (CheckCollisionRecs(e->get_rectangle(), e2->get_rectangle())) {
					if (not e->collide(*e2)) {
						remove_proj = true;
						// update the texture, i think should happen in the take_damage method though 
					}
					else {
						remove_obs = true;
					}
				}
				if (remove_obs) {
					it2 = game_obstacles.erase(it2);
				}
				else {
					++it2;
				}
			}
		}
		if (remove_proj) {
			it = game_projectiles.erase(it);
			continue;
		}
		else {
			++it;
			continue;
		}
	}

	// projectile obstacle collision
	
	// gunmen obstacle collision (gunman is an obstacle
	// 
	// gunmen pickup collision
	// 
}
void draw_game() {
	BeginDrawing();
	ClearBackground(colours::night);
	// draw gunmen
	gunman1->draw();
	gunman2->draw();
	// draw every entity
	for (std::unique_ptr<entities::projectile>& e : game_projectiles) {
		e->draw();
	}
	for (std::unique_ptr<entities::pickup>& e : game_pickups) {
		e->draw();
	}for (std::unique_ptr<entities::obstacle>& e : game_obstacles) {
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

bool is_moving(int gunman) {
	if (gunman == 1) {
		return IsKeyDown(KEY_W) or IsKeyDown(KEY_A) or IsKeyDown(KEY_S) or IsKeyDown(KEY_D);
	}
	else if(gunman == 2){
		return IsKeyDown(KEY_I) or IsKeyDown(KEY_J) or IsKeyDown(KEY_K) or IsKeyDown(KEY_L);	}
	else { return false; }
}