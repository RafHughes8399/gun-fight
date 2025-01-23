#pragma once
// entities is the paretn class for all objects within the game apart from the gunmen, they operate
// independently since they are player-managed

// the entity parent class handles rendering, updating and collision of an entity


// sub classes of entities describe different types of entity

// so an entity has the following
// a position 
// a width and height
// an image source
// 
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
	class entity {
	public:
		// inheritance overhead
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
		// accessors and modifiers
		bool get_remove();
		void set_remove(bool b);
		float get_x() const;
		float get_y() const;

		void set_pos(float x, float y);
		Vector2 get_position();
		Rectangle get_rectangle();
		const char* get_path() const;
		// operator overloads
		entity& operator=(const entity& other);
		virtual bool operator==(const entity& other);
		bool operator<(entity& other);
		// other behaviours
		virtual void draw(); // all entities would be drawn the same, with the same raylib method??
		virtual bool update(std::vector<std::shared_ptr<entity>>& entities) = 0;

		virtual bool collide(entity& other) = 0; // likewise with collision i think better handled by the next level of inheritance
	protected:
		Vector2 position_; // x, y position coords using float, necessary for drawing
		animation animation_ = animation();
		const char* path_;
		bool remove_ = false;
	};

	// a stationary entity that blocks projectiles
	class obstacle : public entity {
	public:

		// overload the custom constructor
		obstacle(float x, float y, const char* path, int health, int category, int penetration)
			: entity(x, y, path), health_(health), obstacle_category_(category), penetration_(penetration) {
		};
		// overload the copy constructor 
		obstacle(const obstacle& other)
			:entity(other), health_(other.health_), obstacle_category_(other.obstacle_category_), penetration_(other.penetration_) {
		};
		// overload the virtual methods
		bool update(std::vector<std::shared_ptr<entity>>& entities) override;
		bool collide(entity& other) override;
		bool operator==(const entity& other) override;

		// unqiue accessors and modifiers
		int get_penetration();
		int get_category();
		int get_health();
		virtual void take_damage(int damage); // returns true if health > 0
	protected:
		int health_;
		int obstacle_category_;
		int penetration_;
		// an obstacle has health
	};
	class moveable_obstacle : public obstacle {
	public:
		moveable_obstacle(float x, float y, const char* path, int health, int category, int penetration, float movement_x, float movement_y)
			: obstacle(x, y, path, health, category, penetration), movement_speed_(Vector2{ movement_x, movement_y }) {
		}
		moveable_obstacle(const moveable_obstacle& other)
			: obstacle(other), movement_speed_(other.movement_speed_), frames_existed_(other.frames_existed_) {
		};
		Vector2 get_speed();
		bool update(std::vector<std::shared_ptr<entity>>& entities) override;
		bool collide(entity& other) override;
		virtual bool move(std::vector<std::shared_ptr<entity>>& entities);
		virtual void change_direction() = 0;
	protected:
		Vector2 movement_speed_;
		int frames_existed_ = 0;

	};
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
	class wagon : public moveable_obstacle {
	public:
		wagon(float x, float y, float movement_x, float movement_y)
			: moveable_obstacle(x, y, config::WAGON_UP_PATH, config::WAGON_HEALTH, config::WAGON_CATEGORY, config::WAGON_PENETRATION, movement_x, movement_y) {
		};
		wagon(const wagon& other)
			: moveable_obstacle(other) {
		};
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
		void change_direction() override;
		bool move(std::vector<std::shared_ptr<entity>>& entities) override;
		bool update(std::vector<std::shared_ptr<entity>>& entities) override;
		void draw() override;
	private:
		float baseline_;
		int lifespan_;
	};	

	class pickup : public entity {
	public:
		pickup(float x, float y, const char* path)
			: entity(x, y, path) {
		};
		pickup(const pickup& other)
			:entity(other) {
		};

		bool update(std::vector<std::shared_ptr<entity>>& entities) override;
		bool collide(entity& other) override;

		bool operator==(const entity& other) override;

	protected:
	};
	class empty_pickup : public pickup {
	public:
		empty_pickup(float x, float y, const char* path)
			: pickup(x, y, path) {
		};
	private:
	};
	class rifle_pickup : public pickup {
	public:
		rifle_pickup(float x, float y, const char* path)
			: pickup(x, y, path) {
		}
	private:
	};
	class dynamite_pickup : public pickup {
	public:
		dynamite_pickup(float x, float y, const char* path)
			: pickup(x, y, path) {
		}

	private:
	};
	class armour_pickup : public pickup {
	public:
		armour_pickup(float x, float y, const char* path)
			: pickup(x, y, path) {
		}

	private:

	};
	class ammo_pickup : public pickup {
	public:
		ammo_pickup(float x, float y, const char* path)
			: pickup(x, y, path) {
		}

	private:
	};
	
	class projectile : public entity {
	public:
		projectile(float x, float y, const char* path, float speed, float direction, int damage, int penetration)
			: entity(x, y, path), speed_direction_({ speed, direction }), damage_(damage), penetration_(penetration) {
		};

		projectile(const projectile& other)
			: entity(other), speed_direction_(other.speed_direction_), damage_(other.damage_), penetration_(other.penetration_) {
		};

		int get_damage();
		int get_penetration();
		// overload collide and update
		bool update(std::vector<std::shared_ptr<entity>>& entities) override; // this is where projectile movement will occur
		bool collide(entity& other) override;

		// operator overloads
		bool operator==(const entity& other) override;

		// unique accessors and modifiers
		Vector2 get_speed_direction() const;
		bool penetrate(const int& obstacle_penetration);

	protected:
		int damage_;
		int penetration_;
		Vector2 speed_direction_; // add this to the position each tick to get movement
	};

	class bullet : public projectile {
	public:
		bullet(float x, float y, const char* path, float direction)
			: projectile(x, y, path, config::BULLET_SPEED, direction, config::REVOLVER_DAMAGE, config::REVOLVER_PENETRATION) {
			animation_ = animation(path, config::BULLET_WIDTH, config::BULLET_HEIGHT);
		};

		bullet(const bullet& other)
			: projectile(other) {
		};

		// overloads
		// operator overloads
		bool operator==(const entity& other) override;
		// unique accessors 
	private:
	};

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
	class weapon : public entity {
	public:
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
		weapon(float x, float y, const char* path, int ammo, int fire_rate)
			:entity(x, y, path), ammo_(ammo), fire_rate_(fire_rate) {
		};
		weapon(const weapon& other)
			: entity(other), ammo_(other.ammo_),
			fire_rate_(other.fire_rate_), state_(other.state_->clone()) {
		};

		int get_ammo();
		bool is_loaded();
		int get_fire_rate();
		int get_cooldown();
		void decrement_cooldown();
		virtual void reset_cooldown() = 0;

		weapon& operator=(const weapon& other);
		bool operator==(const weapon& other);

		virtual bool fire() = 0;
		virtual bool reload() = 0;
		virtual void replenish() = 0;
		virtual std::shared_ptr<entities::projectile> create_bullet(float x, float y, int direction) = 0;
		virtual void draw(int x, int y) = 0;

	protected:
		int ammo_;
		int cooldown_ = 0;
		int fire_rate_;
		std::unique_ptr<weapon_state> state_ = std::make_unique<loaded_state>(loaded_state());
	};

	class revolver : public weapon {
	public:
		revolver(float x, float y, const char* path)
			: weapon(x, y, path, config::REVOLVER_AMMO, config::REVOLVER_FIRE_RATE) {
			animation_ = animation(config::REVOLVER_PATH, config::REVOLVER_WIDTH, config::REVOLVER_HEIGHT,
				config::REVOLVER_ANIMATION_LENGTH, config::REVOLVER_ANIMATIONS);
		};
		revolver(const revolver& other)
			: weapon(other) {
		};
		std::shared_ptr<entities::projectile> create_bullet(float x, float y, int direction) override;
		bool fire() override;
		bool reload() override;
		void replenish() override;
		void draw(int x, int y) override;
		void reset_cooldown() override;

		bool update(std::vector<std::shared_ptr<entity>>& entities) override;
		bool collide(entity& other) override;
	private:
	};

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
		void draw(int x, int y) override;
		void reset_cooldown() override;

		bool update(std::vector<std::shared_ptr<entity>>& entities) override;
		bool collide(entity& other) override;
	private:

	};

	class dynamite : public weapon {
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
		void draw(int x, int y) override;
		void reset_cooldown() override;

		bool update(std::vector<std::shared_ptr<entity>>& entities) override;
		bool collide(entity& other) override;
	};
	// player-controlled entity
	class gunman : public entity {
	public:
		// constructors
		// gunman with revolver
		gunman(float x, float y, const char* path, int health, int direction)
			: entity(x, y, path), health_(health), direction_(direction) {
			animation_ = animation(path, config::GUNMAN_WIDTH, config::GUNMAN_HEIGHT, 0, 0);
		};
		gunman(const gunman& other)
			:entity(other), health_(other.health_), direction_(other.direction_) {
		};
		// unique accessors and modifiers
		int get_health() const;
		int get_direction() const;
		// operator overloads 
		bool operator==(const entity& other) override;
		// behaviour overloads
		bool update(std::vector<std::shared_ptr<entity>>& entities) override;
		bool collide(entity& other) override;

		//unique behaviour 
		void take_damage(int damage);
		bool move(Vector2& movement_vector, std::vector<std::shared_ptr<entity>>& entities);
		void reset(float x, float y);

	private:
		int health_;
		const int direction_; // left facing is 1, right facing is -1 

	};

}
