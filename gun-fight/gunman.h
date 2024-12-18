#pragma once
#include <vector>
#include <memory>
#include <algorithm>
#include "weapons.h"
#include "raylib.h"
namespace gm{
	static const float ORIGIN_X = 100;
	static const float ORIGIN_Y = 100;
	static const int HEALTH = 1;
	class gunman {
	public:
		//destructor
		~gunman() = default;
		// default gunman
		gunman()
			: position_({ ORIGIN_X, ORIGIN_Y }), gun_(std::make_unique<wep::revolver>(wep::revolver())), 
			health_(HEALTH){};
		// gunman at specific position
		gunman(float x, float y)
			: position_({x,y}), gun_(std::make_unique<wep::revolver>(wep::revolver())),
			health_(HEALTH){};
	// move
		gunman(gunman&& other)
			: position_(std::move(other.position_)), gun_(other.gun_.release()), health_(std::move(other.health_)) { };
	// copy
		gunman(const gunman& other)
			: position_(other.position_), gun_(other.gun_->clone()), health_(other.health_) {
			// dynamic cast to get the correct pointer type;
		};
	// accessors and modifiers
	std::vector<float> get_position();
	wep::weapon* get_weapon();
	int get_health();

	// operator overloads
	gunman& operator=(const gunman& other);
	gunman& operator=(gunman&& other);

	// other behaviour
	Vector2 to_vec2();
	bool move(std::vector<float>&  movement_vector);
private:
	std::vector<float> position_;
	std::unique_ptr<wep::weapon> gun_;
	int health_;
};
}