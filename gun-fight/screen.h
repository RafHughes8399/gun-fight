/*****************************************************************//**
 * \file   screen.h
 * \brief  header file for screens in the game
 * 
 * \author raffa
 * \date   February 2025
 *********************************************************************/
#pragma once
#include "raylib.h"
#include "animation.h"
#include "config.h"
#include "button.h"
#include <vector>
#include <utility>
#include <memory>

/**  strategy for drawing each screen */
class draw_strategy {
public:
    virtual ~draw_strategy() = default;
    draw_strategy() = default;
    draw_strategy(draw_strategy&& other) = default;
    draw_strategy& operator=(draw_strategy&& other) = default;
    draw_strategy(const draw_strategy& other) = default;

    virtual void draw(animation& background, std::vector<button>& buttons) = 0;
    virtual std::unique_ptr<draw_strategy> clone() = 0;
};

/**  strategy for drawing the main menu */
class main_menu_strategy : public draw_strategy {
public:
    ~main_menu_strategy() override = default;
    main_menu_strategy()
        : draw_strategy() {
    };
    main_menu_strategy(const main_menu_strategy& other)
        : draw_strategy(other) {
    };
    main_menu_strategy(main_menu_strategy&& other) = default;
    main_menu_strategy& operator=(main_menu_strategy&& other) = default;
    void draw(animation& background, std::vector<button>& buttons) override;
    virtual std::unique_ptr<draw_strategy> clone() override;

    
};
/**  strategy for drawing the return button on the keyboard control screen */
class return_strategy : public draw_strategy {
public:
    ~return_strategy() override = default;
    return_strategy()
        : draw_strategy() {
    }
        return_strategy(const return_strategy& other)
        : draw_strategy(other) {
    };
    return_strategy(return_strategy&& other) = default;
    return_strategy& operator=(return_strategy&& other) = default;
    void draw(animation& background, std::vector<button>& buttons) override;
    virtual std::unique_ptr<draw_strategy> clone() override;

};

/**  definition for the screen class, contains buttons, the background and the draw strategy */
class screen {
public:
    ~screen() = default;
    template <typename InputIt>
    screen(const char* path, float width, float height, int anim_length, int anims, int button_offset, InputIt first, InputIt last, std::unique_ptr<draw_strategy> draw_strat)
        : background_(animation(path, width, height, anim_length, anims)), buttons_(first, last), button_offset_(button_offset), draw_strategy_(std::move(draw_strat)) {
    }
    screen(const screen& other)
        : background_(other.background_), buttons_(other.buttons_), button_offset_(other.button_offset_) {
		draw_strategy_ = other.draw_strategy_->clone();
    }
    int update();
    void draw();
private:
    animation background_;
    std::vector<button> buttons_;
    int button_offset_;
    std::unique_ptr<draw_strategy> draw_strategy_;
};