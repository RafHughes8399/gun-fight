#pragma once
#include "raylib.h"
class animation {
public:
	~animation() = default;
	animation() = default;
	animation(const char* path, float frame_width, float frame_height, int animation_length, int num_animations)
		: animation_sheet_(LoadTexture(path)), frame_width_(frame_width), frame_height_(frame_height),
			animation_length_(animation_length), num_animations_(num_animations){
		frame_ = Rectangle{ 0.0, 0.0, frame_width_, frame_height_};
	}
	animation(const char* path, float frame_width, float frame_height)
		: animation_sheet_(LoadTexture(path)), frame_width_(frame_width), frame_height_(frame_height),
			animation_length_(0), num_animations_(0){
		frame_ = Rectangle{ 0.0, 0.0, frame_width_, frame_height_};
	}
	animation(const animation& other)
		: animation_sheet_(other.animation_sheet_), frame_(other.frame_), frame_width_(other.frame_width_),
		frame_height_(other.frame_height_), animation_length_(other.animation_length_),
		num_animations_(other.num_animations_), play_(other.play_) {
	};
	
	Texture2D get_sheet();
	Rectangle get_current_frame();
	float get_frame_width();
	float get_frame_height();
	int get_animation_length();
	int get_num_animations();
	int get_frame_num();
	int get_animation_num();
	bool get_play();

	void draw_frame(Vector2& pos); // draw the texture at the current frame
	void next_frame(); // go to the next frame (should wrap around the same row) in the current animation
	void previous_frame(); // go to the frame prior
	
	void next_animation(); // go to the next animation
	void previous_animation(); // go to the previous animation 
	void reset_animation();

	void select_animaiton(int index); // choose an animation index to position at
	void select_frame(int frame);

	void end_frame(); // goto the final frame
	void default_frame(); // go to the default frame pos
	void play_animation();
	void pause_animation();
private:
	Texture2D animation_sheet_;
	Rectangle frame_;
	float frame_width_;
	float frame_height_;
	int animation_length_; // number of frames in an animation
	int num_animations_; // number of tags in the aseprite essentially
	bool play_ = false;
	int current_frame_ = 0;
	int current_anim_ = 0;
};