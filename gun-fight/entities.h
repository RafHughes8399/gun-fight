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
#include "Vector2.hpp" // the vector 2 struct from raylib, trying a more selective inclusion strategy
#include <vector>
#include <string>
#include <memory>
namespace entities {
	static const float DEFAULT_X = 100.0;
	static const float DEFAULT_Y = 100.0;
	
	static const int HEIGHT = 50;
	static const int WIDTH = 50;
	static const std::string DEFAULT_PATH = "sprites/default.png";
	class entity {
	public:
		// inheritance overhead
		virtual ~entity() = default;
		entity(entity&& other) = default;
		entity& operator=(entity&& other) = default;

		// default entitiy constructor (at default position, uses default entity image)
		entity()
			:position_({DEFAULT_X, DEFAULT_Y}), height_(HEIGHT), width_(WIDTH), path_(DEFAULT_PATH)
		{};
		entity(float x, float y, int height, int width, const char* path)
			: position_({ x, y }), height_(height), width_(width), path_(path) {};

		// copy constructor
		entity(const entity& other)
			: position_(other.position_), height_(other.height_), 
			width_(other.width_), path_(other.path_) {};
		// accessors and modifiers
		float get_x();
		float get_y();
		Vector2 get_position();
		int get_height();
		int get_width();
		std::string get_path();
		// operator overloads
		entity& operator=(const entity& other);
		// other behaviours
		void draw(); // all entities would be drawn the same, with the same raylib method??
		virtual void update() = 0; // all entities would however, update differently
		// RAYLIB PROVIDES BASIC COLLISION
		virtual bool collide(const entity& other) = 0; // likewise with collision i think better handled by the next level of inheritance
	private:
		Vector2 position_; // x, y position coords using float, necessary for drawing
		int height_; // entity height, necessary for collision
		int width_; // entity width, necessary for collision
		std::string path_; // path to image file

		// position, image src file - i think in the form of const char * let me check,
		// image height and width
	};
	
	// a stationary entity that blocks projectiles
	class obstacle : public entity {
	public:
		// overload the default constructor
		obstacle()
			: entity() {};
		// overload the custom constructor
		obstacle(float x, float y, int height, int width, const char* path, int health)
			: entity(x, y, height, width, path), health_(health) {};
		// overload the copy constructor 
		obstacle(const obstacle& other)
			:entity(other), health_(other.health_) {};
		// overload the virtual methods
		void update() override;
		bool collide(const entity& other) override;

		// unqiue accessors and modifiers
		int get_health();
		void die(); // destroys the obstacle when it dies 
		void take_damage(int& damage); // returns true if health > 0
		//
	private:
		//TODO revisit
		int health_;
		// an obstacle has health
	};

	// an entity that can move and collide with others
	class projectile : public entity {
	public:
		projectile()
			: entity() {};

		projectile(float x, float y, int height, int width, const char* path, float speed, float direction)
			: entity(x, y, height, width, path), speed_direction_({ speed, direction }) {};

		projectile(const projectile& other)
			: entity(other), speed_direction_(other.speed_direction_) {};

		// overload collide and update
		void update() override; // this is where projectile movement will occur
		bool collide(const entity& other) override;

		// unique accessors and modifiers
		Vector2 get_speed_direction();
		// TODO unique behaviours if any
	private:
		// a projectile has movement, maybe a movement function?
		// it has speed
		// it has direction
		// speed and direction is a vector, so maybe use a
		Vector2 speed_direction_; // add this to the position each tick to get movement
	};

	class pickup : public entity {
	public:
		pickup()
			: entity() {};
		pickup(float x, float y, int height, int width, const char* path)
			: entity(x, y, height, width, path) {};
		pickup(const pickup& other)
			:entity(other) {};

		void update() override;
		bool collide(const entity& other) override; // maybe specify to be a gunman 
		// needs more chin scratching, this is not part of the MVP, more like DLC 0
		// leave for now 
		// will have more unique behaviour in regard to interacting with the player
		// maybe an overload of collision to handle the gunman
	private:
	};
}
