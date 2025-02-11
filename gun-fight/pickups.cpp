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
	// check if key pressed, if so, use it, might need some other arguements
	return true;
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

/** health pickup implementation */
void entities::health_pickup::use(entities::gunman& gunman) {
	gunman.take_damage(-1);
}

/**  empty pickup implementation */
void entities::empty_pickup::use(entities::gunman& gunman) {
	return;
}



void entities::rifle_pickup::use(gunman& gunman){
	return;
}

void entities::dynamite_pickup::use(gunman& gunman) {
	return;
}
void entities::armour_pickup::use(gunman& gunman){
	return;
}
void entities::ammo_pickup::use(gunman& gunman){
	return;
}
