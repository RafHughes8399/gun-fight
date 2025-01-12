#include "animation.h"

Texture2D animation::get_sheet()
{
	return Texture2D();
}

Rectangle animation::get_current_frame()
{
	return Rectangle();
}

int animation::get_animation_length()
{
	return 0;
}

int animation::get_num_animation()
{
	return 0;
}

bool animation::get_play()
{
	return false;
}

void animation::draw_frame()
{
}

void animation::next_frame()
{
}

void animation::previous_frame()
{
}

void animation::next_animation()
{
}

void animation::previous_animation()
{
}

void animation::select_animaiton(int index)
{
}

void animation::end_frame()
{
}

void animation::default_frame()
{
}

void animation::set_animation(int animation_row)
{
}

void animation::play_animation()
{
}

void animation::pause_animation()
{
}
