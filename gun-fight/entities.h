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
#include "weapons.h"
#include "config.h"
#include <vector>
#include <string>
#include <memory>
#include <compare>
namespace entities {
	static const float DEFAULT_X = 100.0;
	static const float DEFAULT_Y = 100.0;
	static const int HEIGHT = 50;
	static const int WIDTH = 50;
	static const char* DEFAULT_PATH = "sprites/default.png";
	class entity {
	public:
		// inheritance overhead
		virtual ~entity() = default;
		entity(entity&& other) = default;
		entity& operator=(entity&& other) = default;

		// default entitiy constructor (at default position, uses default entity image)
		entity()
			:position_({DEFAULT_X, DEFAULT_Y}), path_(DEFAULT_PATH)
		{
			texture_ = LoadTexture(path_);
		};
		entity(float x, float y, const char* path)
			: position_({ x, y }), path_(path), remove_(false) {
			
			texture_ = LoadTexture(path_);
		};

		// copy constructor
		entity(const entity& other)
			: position_(other.position_), path_(other.path_), texture_(other.texture_), remove_(other.remove_) {};
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
		virtual bool update(std::vector<std::unique_ptr<entity>>& entities) = 0;

		virtual bool collide(entity& other) = 0; // likewise with collision i think better handled by the next level of inheritance
	protected:
		Vector2 position_; // x, y position coords using float, necessary for drawing
		Texture2D texture_;
		const char* path_; 
		bool remove_;
	};
	
	// player-controlled entity
	class gunman : public entity {
	public:
		// constructors
		// gunman with revolver
		gunman(float x, float y, const char* path, int health, std::map<int, Vector2>& movement, std::pair<int, int>& fire_reload)
			: entity(x, y, path), gun_(std::make_unique<wep::revolver>(wep::revolver())), 
			health_(health), score_(0), movement_(movement), fire_reload_(fire_reload){
		};
		gunman(const gunman& other)
			:entity(other), gun_(other.gun_->clone()), health_(other.health_), 
			score_(other.score_), movement_(other.movement_), fire_reload_(other.fire_reload_){};
		// unique accessors and modifiers
		wep::weapon* get_weapon() const;
		int get_health() const;
		int get_score() const;
		// operator overloads 
		bool operator==(const entity& other) override;
		// behaviour overloads
		bool update(std::vector<std::unique_ptr<entity>>& entities) override;
		bool collide(entity&  other) override;

		//unique behaviour 
		void win_point();
		void take_damage(int damage);
		bool move(Vector2& movement_vector, std::vector<std::unique_ptr<entity>>& entities);
		void reset(float x, float y);
	private:
		std::unique_ptr<wep::weapon> gun_;
		int health_;
		int score_;
		std::map<int, Vector2>& movement_;
		std::pair<int, int>& fire_reload_;
	};

	// a stationary entity that blocks projectiles
	class obstacle : public entity {
	public:
		// overload the default constructor
		obstacle()
			: entity() {};
		// overload the custom constructor
		obstacle(float x, float y, const char* path, int health, int category, int penetration)
			: entity(x, y, path), health_(health), obstacle_category_(category), penetration_(penetration){};
		// overload the copy constructor 
		obstacle(const obstacle& other)
			:entity(other), health_(other.health_), obstacle_category_(other.obstacle_category_), penetration_(other.penetration_) {};
		// overload the virtual methods
		bool update(std::vector<std::unique_ptr<entity>>& entities) override;
		bool collide(entity& other) override;
		bool operator==(const entity& other) override;

		// unqiue accessors and modifiers
		int get_penetration();
		int get_category();
		int get_health();
		void take_damage(int damage); // returns true if health > 0
	protected:
		//TODO revisit
		int health_;
		int obstacle_category_;
		int penetration_;
		// an obstacle has health
	};
	class moveable_obstacle : public obstacle {
	public:
		moveable_obstacle()
			: obstacle() {}
		moveable_obstacle(float x, float y, const char* path, int health, int category, int penetration, float movement_x, float movement_y)
			: obstacle(x, y, path, health, category, penetration), movement_speed_(Vector2 {movement_x, movement_y}) {
		}
		moveable_obstacle(const moveable_obstacle& other)
			: obstacle(other), movement_speed_(other.movement_speed_) {};
		Vector2 get_speed();
		bool update(std::vector<std::unique_ptr<entity>>& entities) override;
		bool collide(entity& other) override;
		bool move(std::vector<std::unique_ptr<entity>>& entities);
		virtual void change_direction() = 0;
	protected:
		Vector2 movement_speed_;


	};
	class cactus : public obstacle {
	public:
		cactus()
			: obstacle() {
		};
		cactus(float x, float y)
			: obstacle(x, y, config::CACTUS_PATH, config::CACTUS_HEALTH, config::CACTUS_CATEGORY, config::CACTUS_PENETRATION) {
		};
		cactus(const cactus& other)
			: obstacle(other) {
		};
	private:

	};	
	class barrel : public obstacle {
	public:
		barrel()
			: obstacle() {
		};
		barrel(float x, float y)
			: obstacle(x, y, config::BARREL_PATH, config::BARREL_HEALTH, config::BARREL_CATEGORY, config::BARREL_PENETRATION) {
		};
		barrel(const barrel& other)
			: obstacle(other) {
		};
	private:

	};	
	class wagon : public moveable_obstacle {
	public:
		wagon()
			: moveable_obstacle() {
		};
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
		tumbleweed()
			: moveable_obstacle() {
		};
		tumbleweed(float x, float y)
			: moveable_obstacle(x, y, config::TUMBLEWEED_PATH, config::TUMBLEWEED_HEALTH, config::TUMBLEWEED_CATEGORY, config::TUMBLEWEED_PENETRATION, config::TUMBLEWEED_SPEED, 0.0) {
		};
		tumbleweed(const tumbleweed& other)
			: moveable_obstacle(other) {
		};
		void change_direction() override;
	private:

	};

	//TODO	implement
	class train : public obstacle {
	public:
	private:

		};
	// an entity that can move and collide with others
	class projectile : public entity {
	public:
		projectile()
			: entity() {};

		projectile(float x, float y, const char* path, float speed, float direction, wep::weapon* weapon)
			: entity(x, y, path), speed_direction_({ speed, direction }), weapon_(weapon) {};

		projectile(const projectile& other)
			: entity(other), speed_direction_(other.speed_direction_), weapon_(other.weapon_) {};

		// overload collide and update
		bool update(std::vector<std::unique_ptr<entity>>& entities) override; // this is where projectile movement will occur
		bool collide(entity& other) override;

		// operator overloads
		bool operator==(const entity& other) override;

		// unique accessors and modifiers
		Vector2 get_speed_direction() const;
		wep::weapon* get_weapon() const;
		// TODO unique behaviours if any
	protected:
		// a projectile has movement, maybe a movement function?
		// it has speed
		// it has direction
		// speed and direction is a vector, so maybe use a
		Vector2 speed_direction_; // add this to the position each tick to get movement
		wep::weapon* weapon_;

	};

	class bullet : public projectile {
	public:
		bullet()
			: projectile() {};
		bullet(float x, float y, const char* path, float direction, wep::weapon* weapon)
			: projectile(x, y, path, config::BULLET_SPEED, direction, weapon) {};

		bullet(const bullet& other)
			: projectile(other) {};
		
		// overloads
		// operator overloads
		bool operator==(const entity& other) override;
		// unique accessors 
	private:
	};

	class pickup : public entity {
	public:
		pickup()
			: entity() {};
		pickup(float x, float y, const char* path)
			: entity(x, y, path) {};
		pickup(const pickup& other)
			:entity(other) {};

		bool update(std::vector<std::unique_ptr<entity>>& entities) override;
		bool collide(entity& other) override; // maybe specify to be a gunman 
		// needs more chin scratching, this is not part of the MVP, more like DLC 0
		// leave for now 
		// will have more unique behaviour in regard to interacting with the player
		// maybe an overload of collision to handle the gunman

		// operator overloads
		bool operator==(const entity& other) override;
	protected:
	};
}
