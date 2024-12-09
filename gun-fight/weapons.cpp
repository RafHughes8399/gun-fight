#include "weapons.h"

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
	w->ammo_ -= 1;
	return true;
}

bool wep::weapon::unloaded_state::fire(wep::weapon* w) {
	return false;
}
bool wep::weapon::unloaded_state::reload(wep::weapon* w) {
	// reduce ammo
	// change state
	w->ammo_ -= 1;
	w->state_.reset(nullptr);
	w->state_ = std::make_unique <wep::weapon::loaded_state>(wep::weapon::loaded_state());
	return true;
}
//----------WEAPON-----------
bool wep::weapon::is_loaded() {
	// attempt to cast to loaded_state, return the null ptr check
	auto state = state_.get();
	return dynamic_cast<wep::weapon::loaded_state*>(state) != nullptr;
}
bool wep::revolver::fire(){
	return state_->fire(this);
}

bool wep::revolver::reload() {
	return state_->reload(this);
}


