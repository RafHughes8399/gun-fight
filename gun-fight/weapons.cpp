/*****************************************************************//**
 * \file   weapons.cpp
 * \brief  implementation file for weapons
 * 
 * \author raffa
 * \date   February 2025
 *********************************************************************/
#include "entities.h"


/** initialising static variables */

/**  firing when the weapon is loaded */
bool entities::weapon::loaded_state::fire(entities::weapon* w) {
	if (w->cooldown_ == 0) {
		w->state_.reset(nullptr);
		w->state_ = std::make_unique<unloaded_state>(unloaded_state());
		w->animation_.next_frame();
		w->reset_cooldown();

		return true;
	}
	return false;

}
/**  reloading when the weapon is loaded */
bool entities::weapon::loaded_state::reload(entities::weapon* w) {
	return true;
}
/**  clone the state, for weapon copying */
std::unique_ptr<entities::weapon::weapon_state> entities::weapon::loaded_state::clone() {
	return std::make_unique<entities::weapon::loaded_state>(*this);
}

/**  firing when the weapon is unloaded, unsuccessful */
bool entities::weapon::unloaded_state::fire(entities::weapon* w) {
	return false;
}
/**  reloading when the weapon is unloaded */
bool entities::weapon::unloaded_state::reload(entities::weapon* w) {
	if (w->ammo_ == 0) { 
		w->animation_.end_frame();
		return false; 
	}
	w->ammo_ -= 1;
	w->state_.reset(nullptr);
	w->state_ = std::make_unique<entities::weapon::loaded_state>(entities::weapon::loaded_state());
	w->animation_.next_frame();
	return true;
}

std::unique_ptr<entities::weapon::weapon_state> entities::weapon::unloaded_state::clone() {
	return std::make_unique<entities::weapon::unloaded_state>(*this);

}

int entities::weapon::get_ammo() {
	return ammo_;
}
bool entities::weapon::is_loaded() {
	auto state = state_.get();
	return dynamic_cast<entities::weapon::loaded_state*>(state) != nullptr;
}
int  entities::weapon::get_fire_rate() {
	return fire_rate_;
}
int entities::weapon::get_cooldown() {
	return cooldown_;
}

bool entities::weapon::is_empty()
{
	return (not is_loaded()) and ammo_ == 0;
}

void entities::weapon::decrement_cooldown() {
	--cooldown_;
}

entities::weapon& entities::weapon::operator=(const entities::weapon& other) {
	//TODO call the entity version of the operator=
	//entities::entity::operator=(other);
	ammo_ = other.ammo_;
	state_ = other.state_->clone();
	fire_rate_ = other.fire_rate_;
	cooldown_ = other.cooldown_;
	return *this;
}

void entities::weapon::draw(int x, int y) {
	Vector2 pos = { x,y };

	if (is_empty()) {
		animation_.end_frame();
	}
	animation_.draw_frame(pos);
}
void entities::weapon::reset_cooldown() {
	cooldown_ = fire_rate_;
}
/**  create revolver bullets when fired successfully */
std::shared_ptr<entities::projectile> entities::revolver::create_bullet(float x, float y, int direction) {
	if (direction == 1) {
		return std::make_shared<entities::bullet>(entities::bullet(x, y, config::BULLET_LEFT, direction));;
	}
	else {
		return std::make_shared<entities::bullet>(entities::bullet(x, y, config::BULLET_RIGHT, direction));;
	}
}
bool entities::revolver::fire() {
	if (state_->fire(this)) {
		Sound s = LoadSound(config::REVOLVER_FIRE_SOUND);
		PlaySound(s);
		return true;
	}
	return false;
}
bool entities::revolver::reload() {
	Sound s = LoadSound(config::REVOLVER_RELOAD_SOUND);
	PlaySound(s);
	return state_->reload(this);
}
void entities::revolver::replenish() {
	ammo_ = config::REVOLVER_AMMO;
	state_ = std::make_unique<loaded_state>(loaded_state());
	animation_.default_frame();
	cooldown_ = 0;
}
bool entities::revolver::update(std::vector<std::shared_ptr<entity>>& entities) {
	return true;
}
bool entities::revolver::collide(entity& other) {
	return false;
}
/**  rifle implementation */
std::shared_ptr<entities::projectile> entities::rifle::create_bullet(float x, float y, int direction){
	if (direction == 1) {
		return std::make_shared<entities::rifle_bullet>(entities::rifle_bullet(x, y, config::RIFLE_BULLET_LEFT, direction));
	}
	else {
		return std::make_shared<entities::rifle_bullet>(entities::rifle_bullet(x, y, config::RIFLE_BULLET_RIGHT, direction));
	}
}


// TODO find rifle sounds
bool entities::rifle::fire(){
	if (state_->fire(this)) {
		Sound s = LoadSound(config::REVOLVER_FIRE_SOUND);
		PlaySound(s);
		return true;
	}
	return false;
}

bool entities::rifle::reload(){
	Sound s = LoadSound(config::REVOLVER_RELOAD_SOUND);
	PlaySound(s);
	return state_->reload(this);
}

void entities::rifle::replenish(){
	ammo_ = config::RIFLE_AMMO;
	state_ = std::make_unique<loaded_state>(loaded_state());
	animation_.default_frame();
	cooldown_ = 0;
}

bool entities::rifle::update(std::vector<std::shared_ptr<entity>>& entities){
	return false;
}

bool entities::rifle::collide(entity& other){
	return false;
}
