#include "entities.h"
bool entities::pickup::operator==(const entities::entity& other) {
	return true;
}
// pickup - other behaviour
bool entities::pickup::update(std::vector<std::shared_ptr<entity>>& entities) {
	//TODO implement
	return true;
}
bool entities::pickup::collide(entities::entity& other) {
	//TODO implement
	return true;
}