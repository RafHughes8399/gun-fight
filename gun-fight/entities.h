/*****************************************************************//**
 * \file   entities.h
 * \brief  header file for entities - all objects that exist in the game. Defines the inheritance 
 * relationships between different types of entities in the game
 * 
 * \author raffa
 * \date   February 2025
 *********************************************************************/
#pragma once
#include "raylib.h"
#include "raymath.h"
#include "animation.h"
#include "config.h"
#include "utility.h"
#include <vector>
#include <string>
#include <memory>
#include <compare>
#include <iostream>

namespace entities {
	/**  entity super class, contains posiiton, animation  */
	class entity {
	public:
		/** constructors and destructors*/
		virtual ~entity() = default;
		entity(entity&& other) = default;
		entity& operator=(entity&& other) = default;
		// default entitiy constructor (at default position, uses default entity image)
		entity(float x, float y, const char* path)
			: position_({ x, y }), path_(path) {
		};
		// copy constructor
		entity(const entity& other)
			: position_(other.position_), path_(other.path_), remove_(other.remove_), animation_(other.animation_) {
		};
		
		/**  accessors */
		bool get_remove();
		void set_remove(bool b);
		float get_x() const;
		float get_y() const;
		const char* get_path() const;

		Vector2 get_position();
		Rectangle get_rectangle();
		animation get_animation();

		/**  modifiers */
		void set_animation(animation anim);
		void set_pos(float x, float y);
		
		/**  operator overloads */
		entity& operator=(const entity& other);
		virtual bool operator==(const entity& other);
		bool operator<(entity& other);
		
		/**  other behaivours */
		virtual void draw(); 
		virtual bool update(std::vector<std::shared_ptr<entity>>& entities) = 0;
		virtual bool collide(entity& other) = 0;
	protected:
		Vector2 position_; // x, y position coords using float, necessary for drawing
		animation animation_ = animation();
		const char* path_;
		bool remove_ = false; // should the entity be removed from the game
	};






	/**  class definition for the gunman, the character controlled by the player */
	class gunman : public entity {
	public:
		/**  constructors and destructors */
		gunman(float x, float y, const char* path, int health, int direction)
			: entity(x, y, path), health_(health), direction_(direction), armour_(0) {
			animation_ = animation(path, config::GUNMAN_WIDTH, config::GUNMAN_HEIGHT, config::GUNMAN_ANIMAITON_LENGTH, config::GUNMAN_ANIMATIONS);
		};
		gunman(const gunman& other)
			:entity(other), health_(other.health_), direction_(other.direction_), armour_(0) {
		};
		/**  unique accessors and behaviours */
		int get_health() const;
		int get_direction() const;
		void increase_health(int value); 
		void take_damage(int damage);
		bool move(Vector2& movement_vector, std::vector<std::shared_ptr<entity>>& entities);
		void reset(float x, float y);
		int get_armour();
		void increase_armour(int value);
		bool is_armoured();
		/**  operator overloads */
		bool operator==(const entity& other) override;

		/**  entity overridden methods */
		bool update(std::vector<std::shared_ptr<entity>>& entities) override;
		bool collide(entity& other) override;


	private:
		int health_;
		const int direction_; // left facing is 1, right facing is -1 
		int armour_;
	};






	/**  definition of super class for obstacles within the game */
	class obstacle : public entity {
	public:
		/**  constructors and destructors */
		obstacle(float x, float y, const char* path, int health, int category, int penetration)
			: entity(x, y, path), health_(health), obstacle_category_(category), penetration_(penetration) {
		};
		// overload the copy constructor 
		obstacle(const obstacle& other)
			:entity(other), health_(other.health_), obstacle_category_(other.obstacle_category_), penetration_(other.penetration_) {
		};
		/** method overrides */
		bool update(std::vector<std::shared_ptr<entity>>& entities) override;
		bool collide(entity& other) override;
		bool operator==(const entity& other) override;

		/**  unique accessrs and other behaviour */
		int get_penetration();
		int get_category();
		int get_health();
		virtual void take_damage(int damage); // returns true if health > 0
	protected:
		int health_;
		int obstacle_category_; // for randomized obstacle generation in each level
		int penetration_;
	};

	/**  definition of super class for moveable obstacles */
	class moveable_obstacle : public obstacle {
	public:
		/** constructors and destructors */
		moveable_obstacle(float x, float y, const char* path, int health, int category, int penetration, float movement_x, float movement_y)
			: obstacle(x, y, path, health, category, penetration), movement_speed_(Vector2{ movement_x, movement_y }) {
		}
		moveable_obstacle(const moveable_obstacle& other)
			: obstacle(other), movement_speed_(other.movement_speed_), frames_existed_(other.frames_existed_) {
		};
		/**  virtual behaviour */
		virtual bool move(std::vector<std::shared_ptr<entity>>& entities);
		/**  overriden behaviours */
		bool update(std::vector<std::shared_ptr<entity>>& entities) override;
		bool collide(entity& other) override;

		/**  unique accessors and other behaviour */
		Vector2 get_speed();
		virtual void change_direction() = 0;
		
	protected:
		Vector2 movement_speed_;
		int frames_existed_ = 0; // for tumbleweed lifespan

	};
	/**  definition of cactus obstacle class */
	class cactus : public obstacle {
	public:
		cactus(float x, float y)
			: obstacle(x, y, config::CACTUS_PATH, config::CACTUS_HEALTH, config::CACTUS_CATEGORY, config::CACTUS_PENETRATION) {
			animation_ = animation(path_, config::CACTUS_WIDTH, config::CACTUS_HEIGHT, config::CACTUS_ANIMATION_LENGTH, config::CACTUS_ANIMATIONS);
		};
		cactus(const cactus& other)
			: obstacle(other) {
		};
		void take_damage(int damage) override;
	private:
	};
	/**  definition of barrel obstacle class */
	class barrel : public obstacle {
	public:
		barrel(float x, float y)
			: obstacle(x, y, config::BARREL_PATH, config::BARREL_HEALTH, config::BARREL_CATEGORY, config::BARREL_PENETRATION) {
			animation_ = animation(path_, config::BARREL_WIDTH, config::BARREL_HEIGHT, config::BARREL_ANIMATION_LENGTH, config::BARREL_ANIMATIONS);
		};
		barrel(const barrel& other)
			: obstacle(other) {
		};
		void take_damage(int damage) override;
	private:
		
	};

	class strawman : public obstacle {
	public:
		strawman(float x, float y, const char* path, int direction)
			: obstacle(x, y, path, config::STRAWMAN_HEALTH, config::STRAWMAN_CATEGORY, config::STRAWMAN_PENETRATION), direction_(direction) {
			animation_ = animation(path, config::STRAWMAN_WIDTH, config::STRAWMAN_HEIGHT);
		};
		strawman(const strawman& other)
			: obstacle(other), direction_(other.direction_) {
		};

		int get_direction();
	private:
		int direction_;
	};
	/** definition of wagon obstacle class, can move vertically on the y-axis */
	class wagon : public moveable_obstacle {
	public:
		wagon(float x, float y, float movement_x, float movement_y)
			: moveable_obstacle(x, y, config::WAGON_UP_PATH, config::WAGON_HEALTH, config::WAGON_CATEGORY, config::WAGON_PENETRATION, movement_x, movement_y) {

			// animation_ = animation(); depends on direction
			animation_ = animation(config::WAGON_DOWN_PATH, config::WAGON_DOWN_WIDTH, config::WAGON_DOWN_HEIGHT, config::WAGON_ANIMATION_LENGTH, config::WAGON_ANIMATIONS);
		};
		wagon(const wagon& other)
			: moveable_obstacle(other) {
		};

		/**  overridden behaviours  */
		void draw() override;
		void change_direction() override;
	private:

	};
	class tumbleweed : public moveable_obstacle {
	public:
		tumbleweed(float x, float y)
			: moveable_obstacle(x, y, config::TUMBLEWEED_PATH, config::TUMBLEWEED_HEALTH, config::TUMBLEWEED_CATEGORY, config::TUMBLEWEED_PENETRATION, config::TUMBLEWEED_SPEED, 0.0),
			baseline_(y), lifespan_(util::generate_random_int(config::TUMBLEWEED_LIFESPAN_LOWER, config::TUMBLEWEED_LIFESPAN_UPPER)) {
			animation_ = animation(path_, config::TUMBLEWEED_WIDTH, config::TUMBLEWEED_HEIGHT, config::TUMBLEWEED_ANIMATION_LENGTH, config::TUMBLEWEED_ANIMATIONS);
		};
		tumbleweed(const tumbleweed& other)
			: moveable_obstacle(other), baseline_(other.baseline_), lifespan_(other.lifespan_) {
		};

		/**  overridden behaviours  */
		bool move(std::vector<std::shared_ptr<entity>>& entities) override;
		void change_direction() override;
		bool update(std::vector<std::shared_ptr<entity>>& entities) override;
		void draw() override;
	private:
		float baseline_; // for sine wave movement
		int lifespan_;
	};


	/** super class definition for projectiles in the game */
	class projectile : public entity {
	public:
		/**  constructors and destructors  */
		projectile(float x, float y, const char* path, float speed, float direction, int damage, int penetration)
			: entity(x, y, path), speed_direction_({ speed, direction }), damage_(damage), penetration_(penetration) {
		};

		projectile(const projectile& other)
			: entity(other), speed_direction_(other.speed_direction_), damage_(other.damage_), penetration_(other.penetration_) {
		};

		/**  accessors  and unique behaviour*/
		int get_damage();
		int get_penetration();
		Vector2 get_speed_direction() const;
		bool penetrate(const int& obstacle_penetration);

		/**  overridden behaviours */
		bool update(std::vector<std::shared_ptr<entity>>& entities) override; // this is where projectile movement will occur
		bool collide(entity& other) override;

		/**  operator overloads */
		bool operator==(const entity& other) override;

	protected:
		int damage_;
		int penetration_;
		Vector2 speed_direction_;
	};
	/** class definition for revolver bullets */
	class bullet : public projectile {
	public:
		/**  constructors and destructors */
		bullet(float x, float y, const char* path, float direction)
			: projectile(x, y, path, config::BULLET_SPEED, direction, config::REVOLVER_DAMAGE, config::REVOLVER_PENETRATION) {
			animation_ = animation(path, config::BULLET_WIDTH, config::BULLET_HEIGHT);
		};

		bullet(const bullet& other)
			: projectile(other) {
		};

		bool operator==(const entity& other) override;
	private:
	};
	/**  class definition for rifle bullets - TODO implement */
	class rifle_bullet : public projectile {
	public:
		rifle_bullet(float x, float y, const char* path, float direction)
			: projectile(x, y, path, config::RIFLE_BULLET_SPEED, direction, config::RIFLE_DAMAGE, config::RIFLE_PENETRATION) {
			animation_ = animation(path, config::RIFLE_BULLET_WIDTH, config::RIFLE_BULLET_HEIGHT); // TODO fill in
		};
		rifle_bullet(const rifle_bullet& other)
			: projectile(other) {
		};

	private:
	};
	/**  class definition for dynamite sticks = TODO implement */
	class dynamite_stick : public projectile {
	public:
		dynamite_stick(float x, float y, const char* path, float direction, int throw_power)
			: projectile(x, y, path, config::DYNAMITE_SPEED, direction, config::DYNAMITE_DAMAGE, config::DYNAMITE_PENETRATION), throw_power_(throw_power) {
			animation_ = animation(path, config::DYNAMITE_STICK_WIDTH,
				config::DYNAMITE_STICK_HEIGHT, 1, 1); // placeholder, replace when animation is done
		}
		dynamite_stick(const dynamite_stick& other)
			: projectile(other), throw_power_(other.throw_power_) {
		};
	private:
		// need a detonation radius and timer 
		float det_radius_ = config::DYNAMITE_DET_RADIUS;
		int det_timer_ = config::DYNAMITE_TIMER; // in frames 
		int throw_power_; // essentially a speed coefficient that affects movement
	};





	/**  super class definition for weapons */
	class weapon : public entity {
	public:
		/**
		 * weapon state pattern for firing and reloading, state reflects whether the weapon is currently
		 loaded or unloaded.
		 */
		class weapon_state {
		public:
			virtual ~weapon_state() = default;
			weapon_state() = default;
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

		/**  constructors and destructors */
		weapon(float x, float y, const char* path, int ammo, int fire_rate)
			:entity(x, y, path), ammo_(ammo), fire_rate_(fire_rate) {
		};
		weapon(const weapon& other)
			: entity(other), ammo_(other.ammo_),
			fire_rate_(other.fire_rate_), state_(other.state_->clone()) {
		};

		/**  unique accessors and behaviours */
		int get_ammo();
		bool is_loaded();
		int get_fire_rate();
		int get_cooldown();
		bool is_empty();
		void decrement_cooldown();
		void reset_cooldown();

		/**  operator overloads */
		weapon& operator=(const weapon& other);
		bool operator==(const weapon& other);

		/**  virtual definitions */
		virtual bool fire() = 0;
		virtual bool reload() = 0;
		virtual void replenish() = 0;
		virtual std::shared_ptr<entities::projectile> create_bullet(float x, float y, int direction) = 0;
		void draw(int x, int y);

	protected:
		int ammo_;
		int cooldown_ = 0;
		int fire_rate_;
		std::unique_ptr<weapon_state> state_ = std::make_unique<loaded_state>(loaded_state());
	};
	/**  class definition for revolver, the default weapon*/
	class revolver : public weapon {
	public:
		/** constructors and destructors */
		revolver(float x, float y, const char* path)
			: weapon(x, y, path, config::REVOLVER_AMMO, config::REVOLVER_FIRE_RATE) {
			animation_ = animation(config::REVOLVER_PATH, config::REVOLVER_WIDTH, config::REVOLVER_HEIGHT,
				config::REVOLVER_ANIMATION_LENGTH, config::REVOLVER_ANIMATIONS);
		};
		revolver(const revolver& other)
			: weapon(other) {
		};
		/**  weapon overridden behaviuors  */
		std::shared_ptr<entities::projectile> create_bullet(float x, float y, int direction) override;
		bool fire() override;
		bool reload() override;
		void replenish() override;

		/**  entitiy overridden behaivours */
		bool update(std::vector<std::shared_ptr<entity>>& entities) override;
		bool collide(entity& other) override;

	private:

	};
	/**  class definition for rifle weapon, TODO implement */
	class rifle : public weapon {
	public:
		rifle(float x, float y, const char* path)
			: weapon(x, y, path, config::RIFLE_AMMO, config::RIFLE_FIRE_RATE) {
			animation_ = animation(config::RIFLE_PATH, config::RIFLE_WIDTH, config::RIFLE_HEIGHT,
				config::RIFLE_ANIMATION_LENGTH, config::RIFLE_ANIMATIONS);
		};
		rifle(const rifle& other)
			: weapon(other) {
		};
		std::shared_ptr<entities::projectile> create_bullet(float x, float y, int direction) override;
		bool fire() override;
		bool reload() override;
		void replenish() override;

		bool update(std::vector<std::shared_ptr<entity>>& entities) override;
		bool collide(entity& other) override;

	private:
	};
	/**  class definition for dynamite weapon TODO implement */
	class dynamite : public weapon {
	public:
		dynamite(float x, float y, const char* path)
			: weapon(x, y, path, config::DYNAMITE_AMMO, config::DYNAMITE_FIRE_RATE) {
			animation_ = animation(config::DYNAMITE_PATH, config::DYNAMITE_WIDTH, config::DYNAMITE_HEIGHT,
				config::DYNAMITE_ANIMATION_LENGTH, config::DYNAMITE_ANIMATIONS);
		};
		dynamite(const dynamite& other)
			: weapon(other) {
		};
		std::shared_ptr<entities::projectile> create_bullet(float x, float y, int direction) override;
		bool fire() override;
		bool reload() override;
		void replenish() override;
		bool update(std::vector<std::shared_ptr<entity>>& entities) override;
		bool collide(entity& other) override;
	private:
		static std::pair<Sound, Sound> fire_reload_sounds_;
	};







	/**  superclass definition for items that can be picked up and used by the player */
	class pickup : public entity {
	public:
		/**
		 * pickup state - alters update behaviour based on whether the item is on the ground or
		 * picked up by the player
		 */
		class pickup_state {
		public:
			pickup_state() = default;
			~pickup_state() = default;
			pickup_state(const pickup_state& other) = default;
			virtual std::unique_ptr<pickup_state>  clone() = 0;
			virtual bool update(pickup* p) = 0;
		private:
		};
		class on_ground : public pickup_state {
		public:
			~on_ground() = default;
			on_ground() = default;
			on_ground(const on_ground& other) = default;
			std::unique_ptr<pickup_state> clone() override;
			bool update(pickup* p) override;
		private:
		};
		class in_inventory : public pickup_state {
		public:
			~in_inventory() = default;
			in_inventory() = default;
			in_inventory(const in_inventory& other) = default;
			std::unique_ptr<pickup_state> clone() override;	
			bool update(pickup* p) override;

		private:

		};

		pickup(float x, float y, const char* path)
			: entity(x, y, path), state_(std::make_unique<on_ground>(on_ground())) {
			animation_ = animation(path, config::ITEM_WIDTH, config::ITEM_HEIGHT);
		};
		pickup(const pickup& other)
			:entity(other), state_(other.state_->clone()) {
		};
		bool update(std::vector<std::shared_ptr<entity>>& entities) override;
		bool collide(entity& other) override;
		void draw(float x, float y);
		virtual void use(std::shared_ptr<gunman>& gunman, std::shared_ptr<weapon>& weapon, std::vector<std::shared_ptr<entity>>& entities) = 0; // for health changes

		bool operator==(const entity& other) override;

	protected:
		std::unique_ptr<pickup_state> state_;
	};
	class health_pickup : public pickup {
	public:
		health_pickup(float x, float y, const char* path)
			: pickup(x, y, path) {
		}
		void use(std::shared_ptr<gunman>& gunman, std::shared_ptr<weapon>& weapon, std::vector<std::shared_ptr<entity>>& entities) override; // for health changes
	private:
	};
	class empty_pickup : public pickup {
	public:
		empty_pickup(float x, float y, const char* path)
			: pickup(x, y, path) {
		};
		void use(std::shared_ptr<gunman>& gunman, std::shared_ptr<weapon>& weapon, std::vector<std::shared_ptr<entity>>& entities) override; // for health changes
	private:
	};
	class rifle_pickup : public pickup {
	public:
		rifle_pickup(float x, float y, const char* path)
			: pickup(x, y, path) {
		}
		void use(std::shared_ptr<gunman>& gunman, std::shared_ptr<weapon>& weapon, std::vector<std::shared_ptr<entity>>& entities) override; // for health changes
	private:
	};
	class dynamite_pickup : public pickup {
	public:
		dynamite_pickup(float x, float y, const char* path)
			: pickup(x, y, path) {
		}
		void use(std::shared_ptr<gunman>& gunman, std::shared_ptr<weapon>& weapon, std::vector<std::shared_ptr<entity>>& entities) override; // for health changes
	private:
	};
	class armour_pickup : public pickup {
	public:
		armour_pickup(float x, float y, const char* path)
			: pickup(x, y, path) {
		}
		void use(std::shared_ptr<gunman>& gunman, std::shared_ptr<weapon>& weapon, std::vector<std::shared_ptr<entity>>& entities) override; // for health changes
	private:

	};
	class ammo_pickup : public pickup {
	public:
		ammo_pickup(float x, float y, const char* path)
			: pickup(x, y, path) {
		}
		void use(std::shared_ptr<gunman>& gunman, std::shared_ptr<weapon>& weapon, std::vector<std::shared_ptr<entity>>& entities) override; // for health changes
	private:
	};

	class strawman_pickup :public pickup {
	public:
		strawman_pickup(float x, float y, const char* path)
			: pickup(x, y, path) {
		};
	private:
		void use(std::shared_ptr<gunman>& gunman, std::shared_ptr<weapon>& weapon, std::vector<std::shared_ptr<entity>>& entities) override; // for health changes
	};

		

}
