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
#include "weapons.h"
#include "config.h"
#include <vector>
#include <string>
#include <memory>
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
			: position_({ x, y }), path_(path) {
			
			texture_ = LoadTexture(path_);
		};

		// copy constructor
		entity(const entity& other)
			: position_(other.position_), path_(other.path_), texture_(other.texture_) {};
		// accessors and modifiers
		float get_x() const;
		float get_y() const;
		Vector2 get_position();
		Rectangle get_rectangle();
		const char* get_path() const;
		// operator overloads
		entity& operator=(const entity& other); // TODO: should be virtual
		virtual bool operator==(const entity& other);
		// other behaviours
		void draw(); // all entities would be drawn the same, with the same raylib method??
		virtual bool update() = 0; // all entities would however, update differently

		virtual bool collide(std::unique_ptr<entity>& other) = 0; // likewise with collision i think better handled by the next level of inheritance
	protected:
		Vector2 position_; // x, y position coords using float, necessary for drawing
		Texture2D texture_;
		const char* path_; 
	};
	
	// player-controlled entity
	class gunman : public entity {
	public:
		// constructors
		gunman()
			: entity() {};
		// gunman with revolver
		gunman(float x, float y, const char* path, int health)
			: entity(x, y, path), gun_(std::make_unique<wep::revolver>(wep::revolver())), health_(health) {
		};
		gunman(const gunman& other)
			:entity(other), gun_(other.gun_->clone()), health_(other.health_) {};
		// unique accessors and modifiers
		wep::weapon* get_weapon() const;
		int get_health() const;
		// operator overloads 
		bool operator==(const entity& other) override;
		// behaviour overloads
		bool update() override;
		bool collide(std::unique_ptr<entity>& other) override;


		//unique behaviour 
		bool move(Vector2& movement_vector, const int& screen_width, const int& screen_height);
	private:
		std::unique_ptr<wep::weapon> gun_;
		int health_;
	};

	// a stationary entity that blocks projectiles
	class obstacle : public entity {
	public:
		// overload the default constructor
		obstacle()
			: entity() {};
		// overload the custom constructor
		obstacle(float x, float y, const char* path, int health)
			: entity(x, y, path), health_(health) {};
		// overload the copy constructor 
		obstacle(const obstacle& other)
			:entity(other), health_(other.health_) {};
		// overload the virtual methods
		bool update() override;
		bool collide(std::unique_ptr<entity>& other) override;

		// operator overloads
		bool operator==(const entity& other) override;

		// unqiue accessors and modifiers
		int get_health();
		void die(); // destroys the obstacle when it dies 
		void take_damage(int& damage); // returns true if health > 0
	protected:
		//TODO revisit
		int health_;
		// an obstacle has health
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
		bool update() override; // this is where projectile movement will occur
		bool collide(std::unique_ptr<entity>& other) override;

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
		bool update() override;
		bool collide(std::unique_ptr<entity>& other) override;

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

		bool update() override;
		bool collide(std::unique_ptr<entity>& other) override; // maybe specify to be a gunman 
		// needs more chin scratching, this is not part of the MVP, more like DLC 0
		// leave for now 
		// will have more unique behaviour in regard to interacting with the player
		// maybe an overload of collision to handle the gunman

		// operator overloads
		bool operator==(const entity& other) override;
	protected:
	};
}
