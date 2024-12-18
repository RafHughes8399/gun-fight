#include <iostream>
#include <stdio.h>
#include "raylib.h"
int main() {
	const int width = 1280;
	const int height = 1024;
	const int half_w = width / 2;
	const int half_h = height / 2;
	InitWindow(width, height, "basic test window");
	SetTargetFPS(60);
	// custom colour definitions
	Color night = {10, 16, 13, 255};


	// initial textures
	Texture2D gunman1 = LoadTexture("sprites/gunman-1.png"); // gunamn 1 is left-facing
	Texture2D gunman2 = LoadTexture("sprites/gunman-2.png"); // gunamn 2 is right-facing

	// need a list of entities so they can be managed and updated

	// say a cactus has 3 health, and the image displayed is the full health one
	// it gets shot, now has 2 health, then the image displayed is the middle health one (use arrays ig)
	while (not WindowShouldClose()) {
		BeginDrawing();

		// set the background colour
		ClearBackground(night);
		// draw the gunmen
		DrawTexture(gunman1, 50, half_h, WHITE);
		DrawTexture(gunman2, 1000, half_h, WHITE);

		EndDrawing();
	}
	CloseWindow();
	return 1;
}
