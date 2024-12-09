#pragma once
#include <vector>
#include <memory>
#include "weapons.h"
namespace gm {
class gunman {
public:
	//destructor
	~gunman() = default;

	// create a gunman at (0,0), with a revolver)
	gunman();
	// move
	gunman(const gunman &&other);
	// copy
	gunman(gunman& other);
	// make starter gunman 

private:
	std::vector<int> position_ = std::vector<int>{};
	std::unique_ptr<wep::weapon> gun_; // make this a pointer 
	int health_;
};
}