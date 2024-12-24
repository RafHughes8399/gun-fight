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
			game_entities.push_back(std::make_unique<entities::bullet>(entities::bullet(gunman1->get_x() + 95, gunman1->get_y() + 45, "sprites/bullet-1.png", 1, gunman1->get_weapon())));
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
			game_entities.push_back(std::make_unique<entities::bullet>(entities::bullet(gunman2->get_x() - 20, gunman2->get_y() + 45, "sprites/bullet-2.png", -1, gunman1->get_weapon())));
		}
	}
	// update entities 

	gunman1->update();
	gunman2->update();

	for (auto it = game_entities.begin(); it != game_entities.end();) {

		if (not (*it)->update()) {
			it = game_entities.erase(it);
		}
		else {
			++it; // DO NOT DELETE THIS LINE YOU SILLY GOOBER 
		}
	}


	// check collision, there is some iterator invalidation


	// restructure the collision method, pass the gunmen and entities via reference

	for (auto it1 = game_entities.begin(); it1 != game_entities.end();) {
		auto e = it1->get();
		bool alive = true;
		if (CheckCollisionRecs(e->get_rectangle(), gunman1->get_rectangle())) {
			alive = e->collide(*gunman1);
			it1 = game_entities.erase(it1);
			if (not alive) {
				game_over = true;
			}
			continue;
		}
		if (CheckCollisionRecs(e->get_rectangle(), gunman2->get_rectangle())) {
			alive = e->collide(*gunman2);
			it1 = game_entities.erase(it1);
			if (not alive) {
				game_over = true;
			}
		}
		for (auto it2 = game_entities.begin(); it2 != game_entities.end();) {
			auto e2 = it2->get();
			if (e == e2) { ++it2; }
			else if(CheckCollisionRecs(e->get_rectangle(), e2->get_rectangle())){
				alive = e->collide(*e2); // texture of obstacles would be updated here
				it1 = game_entities.erase(it1);
				if (not alive) {
					it2 = game_entities.erase(it2);
					break;
				}
				
			}
		}
		++it1;
	}
	// maybe the other way around
	for (auto proj = projectiles.begin(); proj != projectiles.end();){
		if (CheckCollisionRecs((*proj)->get_rectangle(), gunman1->get_rectangle())) {
			if (not (*proj)->collide(*gunman1)) {
				// update the gunman texture to dead,
				proj = projectiles.erase(proj);
				game_over = true;
				continue;
				// change the game states (game_over, etc)
			}
			else {
				proj = projectiles.erase(proj);
				continue;
			}
		}
		// same thing
		else if (CheckCollisionRecs((*proj)->get_rectangle(), gunman2->get_rectangle())) {
			if (not (*proj)->collide(*gunman2)) {
				proj = projectiles.erase(proj);
				std::cout << projectiles.size() << " and " << game_entities.size() << std::endl;
				game_over = true;
				continue;
			}
			else {
				// still destroy the bullet and increment
				proj = projectiles.erase(proj);
				continue;
			}
		}
		else {
			for (auto obs = obstacles.begin(); obs != obstacles.end();) {			
				if (CheckCollisionRecs((*proj)->get_rectangle(), (*obs)->get_rectangle())) {
					if (not (*proj)->collide(**obs)) {
						// update the obstacle texture
						proj = projectiles.erase(proj);
						obs++;
						continue;
						// destroy the bullet
					}
					else{
						// destroy the obstacle
						obs = obstacles.erase(obs);
						proj = projectiles.erase(proj);
						continue;
						// destroy the bullet
					}
				}
			}
		}
		++proj;
	}
	// then check gunman collision with pickups and obstacles
	// TODO: pending pickup implementation
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

bool is_moving(int gunman) {
	if (gunman == 1) {
		return IsKeyDown(KEY_W) or IsKeyDown(KEY_A) or IsKeyDown(KEY_S) or IsKeyDown(KEY_D);
	}
	else if(gunman == 2){
		return IsKeyDown(KEY_I) or IsKeyDown(KEY_J) or IsKeyDown(KEY_K) or IsKeyDown(KEY_L);	}
	else { return false; }
}