#pragma once
#include "Color.hpp"
namespace colours {
	constexpr Color night = { 10, 16, 13, 255 };
	constexpr Color fern_green = { 85, 111, 68, 255 };
	constexpr Color alice_blue = {};
	constexpr Color silver = {192, 192, 192, 255};
	constexpr Color crimson{217, 30, 54, 255};
	// add more colours here 
}

namespace config {

	// screen attributes
	const int SCREEN_HEIGHT = 1024;
	const int SCREEN_WIDTH = 1280;

	// gunman attributes
	const int GUNMAN_HEIGHT = 99;
	const int GUNMAN_WIDTH = 115;
	const float GUNMAN_SPEED = 2.5;

	// bullet attributes;
	const float BULLET_SPEED = 8.5;
}
