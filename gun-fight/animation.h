#pragma once
#include "raylib.h"
class animation {
public:
	~animation() = default;
	animation();
	animation(const char* path_, float frame_width, float frame_height);
	animation(const animation& other);
	
	Texture2D get_sheet();
	Rectangle get_current_frame();
	int get_animation_length();
	int get_num_animation();
	bool get_play();

	void draw_frame(); // draw the texture at the current frame
	void next_frame(); // go to the next frame (should wrap around the same row) in the current animation
	void previous_frame(); // go to the frame prior
	
	void next_animation(); // go to the next animation
	void previous_animation(); // go to the previous animation 

	void select_animaiton(int index); // choose an animation index to position at
	
	void end_frame(); // goto the final frame
	void default_frame(); // go to the default frame pos
	void set_animation(int animation_row);
	void play_animation();
	void pause_animation();
private:
	Texture2D animation_sheet;
	Rectangle frame_;
	float frame_width_;
	float frame_length_;
	int animation_length_; // number of frames in an animation
	int num_animations;; // number of tags in the aseprite essentially
	bool play_ = false;
};