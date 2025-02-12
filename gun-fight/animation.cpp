/*****************************************************************//**
 * \file   animation.cpp
 * \brief  cpp file for spritesheet animation
 * 
 * \author raffaele hughes
 * \date   February 2025
 *********************************************************************/
#include "animation.h"

Texture2D animation::get_sheet(){
	return animation_sheet_;
}

Rectangle animation::get_current_frame(){
	return frame_;
}

float animation::get_frame_width(){
	return frame_width_;
}

float animation::get_frame_height(){
	return frame_height_;
}

int animation::get_animation_length(){
	return animation_length_;
}

int animation::get_frame_num(){
	return current_frame_;
}

int animation::get_animation_num(){
	return current_anim_;
}

int animation::get_num_animations(){
	return num_animations_;
}

bool animation::get_play()
{
	return play_;
}

void animation::draw_frame(Vector2& pos){
	DrawTextureRec(animation_sheet_, frame_, pos, WHITE);
}

void animation::next_frame(){
	frame_.x += frame_width_;
	++current_frame_;
}

void animation::previous_frame(){
	frame_.y -= frame_width_;
	--current_frame_;
}

void animation::next_frame_loop(){
	if (current_frame_ == animation_length_) {
		select_frame(0);
	}
	else {
		frame_.x += frame_width_;
	}
	++current_frame_;
}

void animation::next_animation(){
	frame_.x = 0.0;
	frame_.y += frame_height_;
	++current_anim_;
	current_frame_ = 0;
}

void animation::previous_animation(){
	frame_.x = 0.0;
	frame_.y -= frame_height_;
	--current_anim_;
	current_frame_ = 0;
}

void animation::reset_animation(){
	frame_.x = 0.0;
	current_frame_ = 0;
}

void animation::select_animaiton(int index){
	frame_.y = frame_height_ * index;
}

void animation::select_frame(int frame){
	frame_.x = frame_width_ * frame;
	current_frame_ = frame;
}

void animation::end_frame(){
	frame_.x = frame_width_ * (animation_length_);
	frame_.y = frame_height_ * (num_animations_);
	current_frame_ = animation_length_;
}

void animation::default_frame(){
	frame_.x = 0.0;
	frame_.y = 0.0;
	current_frame_ = 0;
}

void animation::play_animation(){
	play_ = true;
}

void animation::pause_animation(){
	play_ = false;
}
