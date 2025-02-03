#include "entities.h"
#include <iostream>
#include <algorithm>
#include <cmath>
//--------------- ENTITY --------------------------------------------------------------------------------------------------------
float entities::entity::get_x() const {
	return position_.x;
}
float entities::entity::get_y() const{
	return position_.y;
}

// interpret spritesheets as a 2 dimensional array of images
// move between frames by adjusting the rectnagle to display them 
// goto a frame in the current animation row

// these would be called in update to simulate animation
void entities::entity::set_pos(float x, float y){
	position_ = Vector2{ x, y };
}

Vector2 entities::entity::get_position() {
	return position_;
}

Rectangle entities::entity::get_rectangle(){
	return Rectangle{ position_.x, position_.y, animation_.get_frame_width(), animation_.get_frame_height()};
}

animation entities::entity::get_animation(){
	return animation_;
}

const char* entities::entity::get_path() const {
	return path_;
}
bool entities::entity::get_remove() {
	return remove_;
}
void entities::entity::set_remove(bool b) {
	remove_ = b;
}
entities::entity& entities::entity::operator=(const entities::entity& other) {

	position_ = other.position_;
	path_ = other.path_;
	return *this;
}

void entities::entity::draw() {
	// TODO eventually chagne to animation
	animation_.draw_frame(position_);
}

bool entities::entity::operator==(const entities::entity& other) {
	return typeid(*this) == typeid(other) and position_.x == other.get_x() and position_.y == other.get_y();
}

bool entities::entity::operator<(entity& other){
	return Vector2Length(get_position()) < Vector2Length(other.get_position());
}


//--------------- GUNMAN --------------------------------------------------------------------------------------------------------
bool entities::gunman::operator==(const entities::entity& other) {
	if (typeid(*this) != typeid(other)) { return false; }
	const auto gunman_ptr = dynamic_cast<const entities::gunman*>(&other);
	if (gunman_ptr == nullptr) { return false; }
	return entities::entity::operator==(other) and health_ == gunman_ptr->get_health();
}
// accessors
int entities::gunman::get_health() const {
	return health_;
}
int entities::gunman::get_direction() const{
	return direction_;
}
// overloaded behaviour
bool entities::gunman::update(std::vector<std::shared_ptr<entity>>& entities) { // make this a pointer
	return health_ > 0;
}
bool entities::gunman::collide(entities::entity& other) {
	// collides with obstacle
	auto obstacle = dynamic_cast<entities::obstacle*>(&other);
	if (obstacle != nullptr) {
		// block movement
		return false; // the gunman cannot move
	}
	auto pickup = dynamic_cast<entities::pickup*>(&other);
	if (pickup != nullptr) {
		//wrap the gunman with the item 
		return true; // the gunman can move
	}
	// collides with pickup
	return true;
}
// unique behaviour
bool entities::gunman::move(Vector2& movement_vector, std::vector<std::shared_ptr<entities::entity>>& entities) {
	
	animation_.next_frame_loop();
	Vector2 new_pos = { position_.x + movement_vector.x, position_.y + movement_vector.y };
	// Create a rectangle for the proposed new position
	Rectangle proposed_rect = get_rectangle();
	proposed_rect.x = new_pos.x;
	proposed_rect.y = new_pos.y;

	// Check if any obstacle interrupts at the new position
	bool blocked = false;
	std::for_each(entities.begin(), entities.end(), [this, &blocked, &proposed_rect](std::shared_ptr<entities::entity>& e)
		{
			if (this != e.get() and CheckCollisionRecs(proposed_rect, e->get_rectangle())) {
				// if collide is false then do not move
				if (not this->collide(*e)) {
					blocked = true;
				}
			}
		});

	if (blocked) {
		return false;
	}
	if (new_pos.x >= config::PLAYABLE_X and new_pos.x + animation_.get_frame_width() <= config::PLAYABLE_WIDTH and
		new_pos.y >= config::PLAYABLE_Y and new_pos.y + animation_.get_frame_height() <= config::PLAYABLE_HEIGHT + config::PLAYABLE_Y) {
			position_ = new_pos;
			return true;
	}
	return false;
}
void entities::gunman::take_damage(int damage) {
	// return true if still alive, return false if not
	health_ -= damage;
}
void entities::gunman::reset(float x, float y) {
	position_.x = x;
	position_.y = y;
	health_ = config::GUNMAN_HEALTH;
}
// ------------------------------OBSTACLE------------------------------------------------
bool entities::obstacle::operator==(const entities::entity& other) {
	return true;
}
int entities::obstacle::get_penetration(){
	return penetration_;
}
int entities::obstacle::get_category(){
	return obstacle_category_;
}
int entities::obstacle::get_health(){
	return health_;
}

// obstacle -- other behaviour

bool entities::obstacle::update(std::vector<std::shared_ptr<entity>>& entities) {
	//TODO implement
	// do a health check
	if (health_ <= 0) {
		remove_ = true;
	}
	return health_ > 0;
}
bool entities::obstacle::collide(entities::entity& other) {
	//TODO implement
	return true;
}
void entities::obstacle::take_damage(int damage) {
	health_ -= damage;
}

bool entities::moveable_obstacle::update(std::vector<std::shared_ptr<entity>>& entities){
	bool alive = obstacle::update(entities);
	if (not alive) { return alive; }

	// then do movement
	move(entities);
	++frames_existed_;
	return true;
}

bool entities::moveable_obstacle::collide(entity& other){
	// if collision, change direction 
	auto projectile_ptr = dynamic_cast<entities::projectile*>(&other);
	if (projectile_ptr != nullptr) { return true; }
	else {
		return false;
	}
}
bool entities::moveable_obstacle::move(std::vector<std::shared_ptr<entity>>& entities){
	Vector2 new_pos = { position_.x + movement_speed_.x, position_.y + movement_speed_.y };

	// Create a rectangle for the proposed new position
	Rectangle proposed_rect = get_rectangle();
	proposed_rect.x = new_pos.x;
	proposed_rect.y = new_pos.y;

	// TODO:: check players and entities
	bool blocked = false;
	std::for_each(entities.begin(), entities.end(), [this, &blocked, &proposed_rect](std::shared_ptr<entities::entity>& e)
		{
			if (this != e.get() and CheckCollisionRecs(proposed_rect, e->get_rectangle())) {
				// if collide is false then do not move
				if (not this->collide(*e)) {
					blocked = true;
				}
			}
		});

	if (blocked) {
		change_direction();
	}
	// Check screen boundaries based on which half the gunman is in
	if (new_pos.x >= config::PLAYABLE_X and new_pos.x + animation_.get_frame_width() <= config::PLAYABLE_WIDTH and
		new_pos.y >= config::PLAYABLE_Y and new_pos.y + animation_.get_frame_height() <= config::PLAYABLE_HEIGHT + config::PLAYABLE_Y) {
		position_ = new_pos;
		return true;
	}
	else {
		change_direction();
	}
	return false;
}
Vector2 entities::moveable_obstacle::get_speed(){
	return movement_speed_;
}
void entities::tumbleweed::change_direction(){
	movement_speed_.x *= -1;
	position_.x += movement_speed_.x;
}

bool entities::tumbleweed::move(std::vector<std::shared_ptr<entity>>& entities){
	auto new_y = abs(sin(frames_existed_/15)) * config::TUMBLEWEED_AMPLITUDE + baseline_; // add the baseline not the y
	Vector2 new_pos{ position_.x + movement_speed_.x, new_y};
	// Create a rectangle for the proposed new position
	Rectangle proposed_rect = get_rectangle();
	proposed_rect.x = new_pos.x;
	proposed_rect.y = new_pos.y;

	// Check if any obstacle interrupts at the new position
	bool blocked = false;
	std::for_each(entities.begin(), entities.end(), [this, &blocked, &proposed_rect](std::shared_ptr<entities::entity>& e)
		{
			if (this != e.get() and CheckCollisionRecs(proposed_rect, e->get_rectangle())) {
				// if collide is false then do not move
				if (not this->collide(*e)) {
					blocked = true;
				}
			}
		});

	if (blocked) {
		change_direction();
	}
	// Check screen boundaries based on which half the gunman is in
	if (new_pos.x >= config::PLAYABLE_X and new_pos.x + animation_.get_frame_width() <= config::PLAYABLE_WIDTH and
		new_pos.y >= config::PLAYABLE_Y and new_pos.y + animation_.get_frame_height() <= config::PLAYABLE_HEIGHT + config::PLAYABLE_Y) {
		position_ = new_pos;
		return true;
	}
	else {
		change_direction();
	}
	return false;
}

bool entities::tumbleweed::update(std::vector<std::shared_ptr<entity>>& entities){
	// checks if alive, then moves 
	entities::moveable_obstacle::update(entities);
	if (frames_existed_ >= lifespan_) { 
		remove_ = true;
	}
	return true;
}

void entities::tumbleweed::draw(){
	// update the frame and maybe animation
	animation_.next_frame();
	if (animation_.get_frame_num() == static_cast<int>(config::TUMBLEWEED_ANIMATION_LENGTH)) {
		animation_.reset_animation();
	}
	if (frames_existed_ == lifespan_ - config::TUMBLEWEED_ANIMATION_LENGTH) {
		animation_.next_animation();
	}
	entities::entity::draw();
}

void entities::wagon::draw(){
	animation_.draw_frame(position_);
	animation_.next_frame_loop();
}

void entities::wagon::change_direction(){
	movement_speed_.y *= -1;
	position_.y += movement_speed_.y;
	// moving down
	if (movement_speed_.y > 0) {
		animation_ = animation(config::WAGON_DOWN_PATH, config::WAGON_DOWN_WIDTH, config::WAGON_DOWN_HEIGHT, config::WAGON_ANIMATION_LENGTH, config::WAGON_ANIMATIONS);
	}
	// moveing up
	else if (movement_speed_.y < 0) {
		animation_ = animation(config::WAGON_UP_PATH, config::WAGON_UP_WIDTH, config::WAGON_UP_HEIGHT, config::WAGON_ANIMATION_LENGTH, config::WAGON_ANIMATIONS);
	
	}
}

bool entities::projectile::operator==(const entities::entity& other) {
	if (typeid(*this) != typeid(other)) { return false; }
	const auto projectile_ptr = dynamic_cast<const entities::projectile*>(&other);
	if (projectile_ptr == nullptr) { return false; }
	return entities::entity::operator==(other)
		and speed_direction_.x == projectile_ptr->get_speed_direction().x and speed_direction_.y == projectile_ptr->get_speed_direction().y;
}
// projectile - accessors
Vector2 entities::projectile::get_speed_direction() const{
	return speed_direction_;
}
int entities::projectile::get_damage() {
	return damage_;
}
int entities::projectile::get_penetration() {
	return penetration_;
}
//projectile - other beahaviour
bool entities::projectile::penetrate(const int& obstacle_penetration) {
	return penetration_ >= obstacle_penetration;
}

bool entities::projectile::update(std::vector<std::shared_ptr<entity>>& entities) {
	// TODO collision both players and entities
	for (auto& e : entities) {
		if(CheckCollisionRecs(get_rectangle(), e->get_rectangle()) and this != e.get()){
			if (not collide(*e)) {
				remove_ = true;
				return false;
			}
		}
	}
	// then check the gunmen
	position_.x += (speed_direction_.x * speed_direction_.y);
	if (position_.x < config::PLAYABLE_X or position_.x > config::PLAYABLE_WIDTH) {
		remove_ = true;
		return false; // will remove if out of bounds 
	}
	return true;
}
bool entities::projectile::collide(entities::entity& other) {
	// if gunman
	auto gunman = dynamic_cast<entities::gunman*>(&other);
	if (gunman != nullptr and gunman->get_direction() != speed_direction_.y) {
		gunman->take_damage(damage_);
		return false; // cannot move
	}
	// if obstacle
	auto obstacle = dynamic_cast<entities::obstacle*>(&other);
	if (obstacle != nullptr) {
		obstacle->take_damage(damage_);
		// check penetration for tumbleweeds
		return penetrate(obstacle->get_penetration()); // a revolver can penetrate a tumbleweed but not a cactus
	}
	return true;
}
// --------- BULLET ----------------

bool entities::bullet::operator==(const entities::entity& other) {
	if (typeid(*this) != typeid(other)) { return false; }
	const auto bullet_ptr = dynamic_cast<const entities::bullet*>(&other);
	if (bullet_ptr == nullptr) { return false; }
	return entities::projectile::operator==(other);
}

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

void entities::cactus::take_damage(int damage){
	obstacle::take_damage(damage);
	if (health_ > 0) {
		animation_.next_frame();
	}
}

void entities::barrel::take_damage(int damage){
	obstacle::take_damage(damage);
	if (health_ > 0) {
		animation_.next_frame();
	}

}

// ----------------------- WEAPON LOADED STATE ------------------
bool entities::weapon::loaded_state::fire(entities::weapon* w) {
	if (w->cooldown_ == 0) {
		w->state_.reset(nullptr);
		w->state_ = std::make_unique<unloaded_state>(unloaded_state());
		w->animation_.play_animation(); // in draw check if you are the end of the animation
		w->reset_cooldown();

		return true;
	}
	return false;

}
bool entities::weapon::loaded_state::reload(entities::weapon* w) {
	return true;
}
std::unique_ptr<entities::weapon::weapon_state> entities::weapon::loaded_state::clone() {
	return std::make_unique<entities::weapon::loaded_state>(*this);
}

// ----------------------- WEAPON UNLOADED STATE ------------------
bool entities::weapon::unloaded_state::fire(entities::weapon* w) {
	return false;
}
bool entities::weapon::unloaded_state::reload(entities::weapon* w) {
	if (w->ammo_ == 0) { return false; }
	w->ammo_ -= 1;
	w->state_.reset(nullptr);
	w->state_ = std::make_unique<entities::weapon::loaded_state>(entities::weapon::loaded_state());
	w->animation_.play_animation();
	return true;
}

std::unique_ptr<entities::weapon::weapon_state> entities::weapon::unloaded_state::clone() {
	return std::make_unique<entities::weapon::unloaded_state>(*this);

}

// ----------------------- WEAPON ACCESSORS -------------------------
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

void entities::weapon::decrement_cooldown() {
	--cooldown_;
}
// ------------------- WEAPON OPERATOR OVERLOADS ------------------------------
entities::weapon& entities::weapon::operator=(const entities::weapon& other) {
	//TODO call the entity version of the operator=
	//entities::entity::operator=(other);
	ammo_ = other.ammo_;
	state_ = other.state_->clone();
	fire_rate_ = other.fire_rate_;
	cooldown_ = other.cooldown_;
	return *this;
}

//--------------------- WEAPON OTHER BEHAVIOUR ----------------------------------

//------------------- REVOLVER OVERLOADS ----------------------------------
std::shared_ptr<entities::projectile> entities::revolver::create_bullet(float x, float y, int direction){
	if (direction == 1) {
		return std::make_shared<entities::bullet>(entities::bullet(x, y, config::BULLET_LEFT, direction));;
	}
	else {
		return std::make_shared<entities::bullet>(entities::bullet(x, y, config::BULLET_RIGHT, direction));;
	}
}
bool entities::revolver::fire() {
	return state_->fire(this);
}
bool entities::revolver::reload() {
	return state_->reload(this);
}
void entities::revolver::replenish() {
	ammo_ = config::REVOLVER_AMMO;
	state_ = std::make_unique<loaded_state>(loaded_state());
	animation_.default_frame();
	cooldown_ = 0;

}
void entities::revolver::draw(int x, int y) {
	Vector2 pos = { x,y };
	animation_.draw_frame(pos);
	if (animation_.get_play()) {
		if (not animation_.get_frame_num() < config::REVOLVER_ANIMATION_LENGTH) {
			animation_.pause_animation();
			animation_.next_animation();
		}
		else {
			animation_.next_frame();
		}
	}
}
void entities::revolver::reset_cooldown() {
	cooldown_ = fire_rate_;
}
bool entities::revolver::update(std::vector<std::shared_ptr<entity>>& entities){
	return true;
}
bool entities::revolver::collide(entity& other){
	return false;
}