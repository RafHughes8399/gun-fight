#include "gunman.h"
#include "weapons.h"
#include <memory>
#include <vector>

// accessors and modifiers
std::vector<float> gm::gunman::get_position() {
	return position_;
}

wep::weapon* gm::gunman::get_weapon() {
	return gun_.get();
}

int gm::gunman::get_health() {
	return health_;
}
// operator overloads
gm::gunman& gm::gunman::operator=(const gm::gunman& other) {
	position_ = other.position_;
	gun_ = other.gun_->clone();
	health_ = other.health_;
	return *this;

}

gm::gunman& gm::gunman::operator=(gm::gunman&& other) {
	position_ = std::exchange(other.position_, {});
	gun_ = std::exchange(other.gun_, nullptr);
	health_ = std::exchange(other.health_, 0);
	return *this;
}


// other behaviour
bool gm::gunman::move(std::vector<float>& move_vector) {
	if (move_vector.size() != 2) { return false; }
	position_[0] += move_vector[0];
	position_[1] += move_vector[1];
	// TODO: boundary check 
	return true;
}
Vector2 gm::gunman::to_vec2() {
	return { position_[0], position_[1] };
}
