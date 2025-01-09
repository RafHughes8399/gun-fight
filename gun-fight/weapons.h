#pragma once
#include "projectiles.h"
#include "config.h"
#include "raylib.h"
#include <memory>
#include <utility>
namespace wep {
	class weapon {
	public:

		class weapon_state {
		public:
			virtual ~weapon_state() = default;
			weapon_state(weapon_state&& other) = default;
			weapon_state& operator=(weapon_state&& other) = default;

			weapon_state(const weapon_state& other) {};
			weapon_state() {};
			virtual bool fire(weapon* w) = 0;
			virtual bool reload(weapon* w) = 0;
			virtual std::unique_ptr<weapon_state> clone() = 0;
		protected:
		};

		class loaded_state : public weapon_state {
		public:
			loaded_state() {};
			loaded_state(const loaded_state& other) {};
			bool fire(weapon* w) override;
			bool reload(weapon* w) override;
			std::unique_ptr<weapon_state> clone() override;

		private:

		};

		class unloaded_state : public weapon_state {
		public:
			unloaded_state() {};
			unloaded_state(const unloaded_state& other) {};
			bool fire(weapon* w) override;
			bool reload(weapon* w) override;
			std::unique_ptr<weapon_state> clone() override;
		private:

		};

		virtual ~weapon() = default;
		weapon(weapon&& other) = default;
		weapon& operator=(weapon&& other) = default;
		weapon(int ammo, int damage, int penetration, const char* path, float x, float y, float width, float height)
			: ammo_(ammo), damage_(damage), penetration_(penetration), state_(std::make_unique<loaded_state>(loaded_state())),
			animation_sheet_(LoadTexture(path)), current_frame_(0), frame_rec_(Rectangle{x, y, width, height}){};
		weapon(const weapon& other)
			: ammo_(other.ammo_), damage_(other.damage_), penetration_(other.penetration_), state_(other.state_->clone())
			, animation_sheet_(other.animation_sheet_), current_frame_(other.current_frame_), frame_rec_(other.frame_rec_) {

		};
		// accessors and modifiers
		int get_ammo() { return ammo_; }
		int get_damage() {return damage_;}
		bool is_loaded();
		int get_penetration();
		int get_frame() { return current_frame_; }
		// operator overloads
		weapon& operator=(const weapon& other);
		virtual bool operator==(const weapon& other) = 0;
		// other behaviours
		bool penetrate(const int& obstacle_penetration);
		virtual bool fire() = 0;
		virtual bool reload() = 0;
		virtual void replenish() = 0;
		virtual std::unique_ptr<weapon> clone() const = 0;
		virtual void draw(int x, int y) = 0;
		friend class weapon_state;		

	protected:
		int ammo_;
		int damage_;
		int penetration_;
		std::unique_ptr<weapon_state> state_; // if you wanted a get_state return a raw pointer
		//std::unique_ptr<proj::projectile> bullet_type_;
		int current_frame_;
		Texture2D animation_sheet_;
		Rectangle frame_rec_;
	};
	//TODO 
	class revolver : public weapon {
	public:
		revolver()
			: weapon(config::REVOLVER_AMMO, config::REVOLVER_DAMAGE, config::REVOLVER_PENETRATION, config::REVOLVER_ANIMATION_PATH, 0.0, 0.0, 
				static_cast<float>(config::REVOLVER_SHEET_WIDTH / config::REVOLVER_FRAME_WIDTH), 
				static_cast<float>(config::REVOLVER_SHEET_HEIGHT / config::REVOLVER_FRAME_HEIGHT)){
		};
		revolver(const revolver& other)
			: weapon(other) {};
		bool operator==(const weapon& other) override;
		bool fire() override;
		bool reload() override;
		void replenish() override;
		void draw(int x, int y) override;
		std::unique_ptr<weapon> clone() const override;
	private:

	};

}

// ok we use a sprite sheet
// each animation is 5 frames
// if the frame count is 60, then 5 frames is 1/12 of a secon


// ok so you load the png file in a texutre

// and you a drawing portions of the sprite sheet

// set the frame count and the frame speed
