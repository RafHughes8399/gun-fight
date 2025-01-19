#include "entities.h"
//-----------WEAPON-STATE-------------
bool entities::weapon::loaded_state::fire(entities::weapon* w){	
	if (w->cooldown_ == 0) {
		w->state_.reset(nullptr);
		w->state_ = std::make_unique<unloaded_state>(unloaded_state());
		w->play_frames_ = config::REVOLVER_FRAME_WIDTH;
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
bool entities::weapon::unloaded_state::fire(entities::weapon* w) {
	return false;
}
bool entities::weapon::unloaded_state::reload(entities::weapon* w) {
	if (w->ammo_ == 0) { return false; }
	w->ammo_ -= 1;
	w->state_.reset(nullptr);
	w->state_ = std::make_unique <entities::weapon::loaded_state>(entities::weapon::loaded_state());
	w->play_frames_ = config::REVOLVER_FRAME_WIDTH;
	return true;
}

std::unique_ptr<entities::weapon::weapon_state> entities::weapon::unloaded_state::clone() {
	return std::make_unique<entities::weapon::unloaded_state>(*this);

}
int entities::weapon::get_ammo(){
	return ammo_;
}
int entities::weapon::get_damage(){
	return damage_;
}
// WEAPON ACCESSORS AND MODIFIERS 
bool entities::weapon::is_loaded() {
	// attempt to cast to loaded_state, return the null ptr check
	auto state = state_.get();
	return dynamic_cast<entities::weapon::loaded_state*>(state) != nullptr;
}

int entities::weapon::get_penetration(){
	return penetration_;
}

int entities::weapon::get_frame(){
	return current_frame_;
}

int entities::weapon::get_cooldown(){
	return cooldown_;
}

void entities::weapon::decrement_cooldown(){
	--cooldown_;
}

// WEAPON OPERATOR OVERLOADS
entities::weapon& entities::weapon::operator=(const entities::weapon& other) {
	ammo_ = other.ammo_;
	damage_ = other.damage_;
	penetration_ = other.penetration_;
	state_ = other.state_->clone();
	return *this;
}

bool entities::weapon::penetrate(const int& obstacle_penetration){
	return penetration_ >= obstacle_penetration;
}


// ---------- REVOLVER -----------
std::unique_ptr<entities::projectile> entities::revolver::create_bullet(float x, float y, int direction) {
	if (direction == 1) {
		return std::make_unique<entities::bullet>(entities::bullet(x + config::GUNMAN_WIDTH, y + 45, "sprites/bullet-1.png", direction, this));
	}
	else {
		return std::make_unique<entities::bullet>(entities::bullet(x, y - config::BULLET_WIDTH + 45, "sprites/bullet-2.png", direction, this));
	}
}
bool entities::revolver::fire(){
	return state_->fire(this);
}

bool entities::revolver::reload() {
	return state_->reload(this);
}
void entities::revolver::replenish() {
	ammo_ = config::REVOLVER_AMMO;
	state_ = std::make_unique<loaded_state>();
	frame_rec_ = Rectangle{ 0.0, 0.0, static_cast<float>(config::REVOLVER_SHEET_WIDTH / config::REVOLVER_FRAME_WIDTH) , static_cast<float>(config::REVOLVER_SHEET_HEIGHT / config::REVOLVER_FRAME_HEIGHT) };
	// reset the frame rectangle
}
void entities::revolver::draw(int x, int y) {
	if (play_frames_ > 0) {
		// progress to the next frame in the animation
		frame_rec_.x += static_cast<float>(config::REVOLVER_SHEET_WIDTH / config::REVOLVER_FRAME_WIDTH);
		if (play_frames_ == 1) {
			// go back to the beginning of the row
			frame_rec_.x = 0.0;
			frame_rec_.y += static_cast<float>(config::REVOLVER_SHEET_HEIGHT / config::REVOLVER_FRAME_HEIGHT);
		}
		--play_frames_;
	}
	DrawTextureRec(animation_sheet_, frame_rec_, Vector2{ static_cast<float>(x), static_cast<float>(y) }, WHITE);
}
void entities::revolver::reset_cooldown(){
	cooldown_ = config::REVOLVER_FIRE_RATE;
}
std::unique_ptr<entities::weapon> entities::revolver::clone() const {
	return std::make_unique<entities::revolver>(*this);
}

bool entities::revolver::operator==(const weapon& other) {
	return true;
}

