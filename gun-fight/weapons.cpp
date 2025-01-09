#include "weapons.h"
#include <iostream>
#include <memory>
//-----------WEAPON-STATE-------------
bool wep::weapon::loaded_state::fire(wep::weapon* w){	
	// create projectile -- tentative, maybe outsourced by the game manager, because it would not exist out of the scope
	// change the state
	w->state_.reset(nullptr);
	w->state_ = std::make_unique<unloaded_state>(unloaded_state());
	return true;
}
bool wep::weapon::loaded_state::reload(wep::weapon* w) {
	// reduce ammo
	// do not change state

	// check ammo count
	if (w->ammo_ == 0) { return false; }
	w->ammo_ -= 1;
	return true;
}
std::unique_ptr<wep::weapon::weapon_state> wep::weapon::loaded_state::clone() {
	return std::make_unique<wep::weapon::loaded_state>(*this);
}
bool wep::weapon::unloaded_state::fire(wep::weapon* w) {
	return false;
}
bool wep::weapon::unloaded_state::reload(wep::weapon* w) {
	if (w->ammo_ == 0) { return false; }
	w->ammo_ -= 1;
	w->state_.reset(nullptr);
	w->state_ = std::make_unique <wep::weapon::loaded_state>(wep::weapon::loaded_state());
	return true;
}

std::unique_ptr<wep::weapon::weapon_state> wep::weapon::unloaded_state::clone() {
	return std::make_unique<wep::weapon::unloaded_state>(*this);

}
// WEAPON ACCESSORS AND MODIFIERS 
bool wep::weapon::is_loaded() {
	// attempt to cast to loaded_state, return the null ptr check
	auto state = state_.get();
	return dynamic_cast<wep::weapon::loaded_state*>(state) != nullptr;
}

int wep::weapon::get_penetration(){
	return penetration_;
}

// WEAPON OPERATOR OVERLOADS
wep::weapon& wep::weapon::operator=(const wep::weapon& other) {
	ammo_ = other.ammo_;
	damage_ = other.damage_;
	penetration_ = other.penetration_;
	state_ = other.state_->clone();

	return *this;
}

bool wep::weapon::penetrate(const int& obstacle_penetration){
	return penetration_ >= obstacle_penetration;
}

// ---------- REVOLVER -----------
bool wep::revolver::fire(){
	return state_->fire(this);
}

bool wep::revolver::reload() {
	return state_->reload(this);
}
void wep::revolver::replenish() {
	ammo_ = config::REVOLVER_AMMO;
	state_ = std::make_unique<loaded_state>();
}
void wep::revolver::draw(int x, int y) {
	std::cout << frame_rec_.x << " " << frame_rec_.y << " " << frame_rec_.width << " " << frame_rec_.height << std::endl;
	DrawTextureRec(animation_sheet_, frame_rec_, Vector2{ static_cast<float>(x), static_cast<float>(y) }, WHITE);
}
std::unique_ptr<wep::weapon> wep::revolver::clone() const {
	return std::make_unique<wep::revolver>(*this);
}

bool wep::revolver::operator==(const weapon& other) {
	return true;
}

