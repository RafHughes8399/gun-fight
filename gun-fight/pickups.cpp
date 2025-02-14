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
void entities::empty_pickup::use(std::shared_ptr<gunman>& gunman, std::shared_ptr<weapon>& weapon, std::vector<std::shared_ptr<entity>>& entities) {
	return;
}

/** increase player health to a max of two */
void entities::health_pickup::use(std::shared_ptr<gunman>& gunman, std::shared_ptr<weapon>& weapon, std::vector<std::shared_ptr<entity>>& entities) {
	// can have a max health of 2
	if (gunman->get_health() < 2) {
		gunman->take_damage(-1);
	}
}
/** change the player's weapon to a rifle */
void entities::rifle_pickup::use(std::shared_ptr<gunman>& gunman, std::shared_ptr<weapon>& weapon, std::vector<std::shared_ptr<entity>>& entities) {
		// replace the weapon and the gunman animation
	weapon = std::make_shared<entities::rifle>(entities::rifle(weapon->get_x(), weapon->get_y(), config::RIFLE_PATH));
	if (gunman->get_direction() == 1) {
		gunman->set_animation(animation(config::P1_RIFLE_PATH, config::GUNMAN_WIDTH, config::GUNMAN_HEIGHT, config::GUNMAN_ANIMAITON_LENGTH, config::GUNMAN_ANIMATIONS));
	}
	else {
		gunman->set_animation(animation(config::P2_RIFLE_PATH, config::GUNMAN_WIDTH, config::GUNMAN_HEIGHT, config::GUNMAN_ANIMAITON_LENGTH, config::GUNMAN_ANIMATIONS));
	}
	return;
}

/**  change the player's weapon to dynamite */
void entities::dynamite_pickup::use(std::shared_ptr<gunman>& gunman, std::shared_ptr<weapon>& weapon, std::vector<std::shared_ptr<entity>>& entities) {
	return;
}

/**  give the player armour if not wearing */
void entities::armour_pickup::use(std::shared_ptr<gunman>& gunman, std::shared_ptr<weapon>& weapon, std::vector<std::shared_ptr<entity>>& entities) {
	if (gunman->get_armour() < 1) {
		gunman->increase_armour(1);
	}
	return;
}

/** replenish the player's ammo */
void entities::ammo_pickup::use(std::shared_ptr<gunman>& gunman, std::shared_ptr<weapon>& weapon, std::vector<std::shared_ptr<entity>>& entities) {
	weapon->replenish();
	return;
}

/**  create a 1 health strawman obstacle in front of the player  */
void entities::strawman_pickup::use(std::shared_ptr<gunman>& gunman, std::shared_ptr<weapon>& weapon, std::vector<std::shared_ptr<entity>>& entities) {
	// left facing gunman
	if (gunman->get_direction() == 1) {
		float x = gunman->get_x() + (gunman->get_animation().get_frame_width() * 1.5);
		entities.push_back(std::make_shared<entities::strawman>(strawman(x, gunman->get_y(), config::STRAWMAN_LEFT_PATH, gunman->get_direction())));
	}

	// right facing gunman
	else {
		float x = gunman->get_x() - (gunman->get_animation().get_frame_width() / 2);
		entities.push_back(std::make_shared<entities::strawman>(strawman(x, gunman->get_y(), config::STRAWMAN_RIGHT_PATH, gunman->get_direction())));
	}
	return;
}
