/*****************************************************************//**
 * \file   pickups.cpp
 * \brief  implementation file for pickups - objects that can be picked up  and used by the player
 * 
 * \author raffa
 * \date   February 2025
 *********************************************************************/
#include "entities.h"

/** state function implementation */
std::unique_ptr<entities::pickup::pickup_state> entities::pickup::on_ground::clone() {
	//TODO implement
	return std::make_unique<entities::pickup::on_ground>(*this);
}
std::unique_ptr<entities::pickup::pickup_state> entities::pickup::in_inventory::clone() {
	// TODO implement
	return std::make_unique<entities::pickup::in_inventory>(*this);
}

bool entities::pickup::on_ground::update(entities::pickup* p) {
	// TODO implement
	// do nothing so far
	return true;
}

bool entities::pickup::in_inventory::update(entities::pickup* p) {
	//TODO implement
	// check if aa key was pressed
	return true; // whether the item should be kept in the inventory or not
}
bool entities::pickup::operator==(const entities::entity& other) {
	return true;
}
/** superclass implementation */
bool entities::pickup::update(std::vector<std::shared_ptr<entity>>& entities) {
	//TODO implement
	return true;
}
bool entities::pickup::collide(entities::entity& other) {
	//TODO implement
	return true;
}
void entities::pickup::draw(float x, float y) {
	auto pos = Vector2{ x, y };
	animation_.draw_frame(pos);
}

/** empty pickup use */
void entities::empty_pickup::use(std::shared_ptr<gunman> gunman)
{
}

void entities::empty_pickup::use(std::shared_ptr<weapon> weapon)
{
}
void entities::empty_pickup::use(std::vector<std::shared_ptr<entities::entity>>& entities) {}


/** health pickup use */
void entities::health_pickup::use(std::shared_ptr<entities::gunman> gunman) {
	// can have a max health of 3
	if (gunman->get_health() < 3) {
		gunman->take_damage(-1);
	}
}

void entities::health_pickup::use(std::shared_ptr<entities::weapon> weapon) {
	return;
}


void entities::health_pickup::use(std::vector<std::shared_ptr<entity>>& entities){
	return;
}

void entities::rifle_pickup::use(std::shared_ptr<gunman> gunman){
	return;
}

void entities::rifle_pickup::use(std::shared_ptr<weapon> weapon){
	return;
}

void entities::rifle_pickup::use(std::vector<std::shared_ptr<entity>>& entities){
	return;
}

void entities::dynamite_pickup::use(std::shared_ptr<gunman> gunman){
	return;
}

void entities::dynamite_pickup::use(std::shared_ptr<weapon> weapon){
	return;
}

void entities::dynamite_pickup::use(std::vector<std::shared_ptr<entity>>& entities){
	return;
}

void entities::armour_pickup::use(std::shared_ptr<gunman> gunman){
	return;
}

void entities::armour_pickup::use(std::shared_ptr<weapon> weapon){
	return;
}

void entities::armour_pickup::use(std::vector<std::shared_ptr<entity>>& entities){
	return;
}

void entities::ammo_pickup::use(std::shared_ptr<gunman> gunman){
	return;
}
void entities::ammo_pickup::use(std::shared_ptr<weapon> weapon){
	return;
}
void entities::ammo_pickup::use(std::vector<std::shared_ptr<entity>>& entities){
	return;
}

void entities::strawman_pickup::use(std::shared_ptr<gunman> gunman){
	return;
}

void entities::strawman_pickup::use(std::shared_ptr<weapon> weapon){
	return;
}
void entities::strawman_pickup::use(std::vector<std::shared_ptr<entity>>& entities){
	return;
}