#pragma once
#include "projectiles.h"
#include <memory>
namespace wep{
class weapon {
	public:
		virtual ~weapon() = default;
		weapon(weapon&& other) = default;
		weapon& operator=(weapon&& other) = default;
		weapon(int ammo, int damage, proj::projectile p)
			: ammo_(ammo), damage_(damage), 
			load_state_(std::make_unique<weapon_state>(loaded_state())),
			projectile_type_(std::make_unique<proj::projectile>(p))
			{};
		weapon(const weapon& other);

		int get_ammo() const;
		int get_damage() const;
		bool change_state();
		std::unique_ptr<proj::projectile> get_projectile_type();

		virtual bool fire() = 0;
		virtual bool reload() = 0;

	private:
		int ammo_;
		const int damage_;
		std::unique_ptr<weapon_state> load_state_;
		std::unique_ptr<proj::projectile> projectile_type_;
	
	};
	//TODO 
	class revolver : public weapon {
	public:
		bool fire() override;
		bool reload() override;
	private:	

	};

	// TODO: implement subclasses
	class shotgun : public weapon {
	public:
	private:
	};
	class rifle : public weapon {
	public:
	private:
	};
	class dynamite : public weapon {
	public:
	private:
	};


	class weapon_state {
	public:
		virtual ~weapon_state() = default;
		weapon_state(weapon_state&& other) = default;
		weapon_state& operator=(weapon_state&& other) = default;
		weapon_state();
		virtual bool fire() = 0;
		virtual bool reload() = 0;
	protected:
		const std::unique_ptr<weapon> weapon_;
	};

	class loaded_state : public weapon_state {
	public:
		loaded_state() {};
		bool fire() override;
		bool reload() override;
	private:
	};

	class unloaded_state : public weapon_state { 
	public:
		unloaded_state();
		bool fire() override;
		bool reload() override;	
	private:
	};
}
