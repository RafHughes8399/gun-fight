#pragma once
#include "projectiles.h"
#include <memory>
namespace wep {
	static const int REVOLVER_AMMO = 6;
	static const int REVOLVER_DMG = 1;
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
		weapon(int ammo, int damage)
			: ammo_(ammo), damage_(damage), state_(std::make_unique<loaded_state>(loaded_state())){};
		weapon(const weapon& other)
			: ammo_(other.ammo_), damage_(other.damage_), state_(other.state_->clone()) {
		};
		// accessors and modifiers
		int get_ammo() { return ammo_; }
		int get_damage() {return damage_;}
		bool is_loaded();

		// operator overloads
		weapon& operator=(const weapon& other);

		// other behaviours
		virtual bool fire() = 0;
		virtual bool reload() = 0;
		virtual std::unique_ptr<weapon> clone() const = 0;
		friend class weapon_state;		
	protected:
		int ammo_;
		int damage_;
		std::unique_ptr<weapon_state> state_; // if you wanted a get_state return a raw pointer
		//std::unique_ptr<proj::projectile> bullet_type_;
	};
	//TODO 
	class revolver : public weapon {
	public:
		revolver()
			: weapon(REVOLVER_AMMO, REVOLVER_DMG) {
		};
		revolver(const revolver& other)
			: weapon(other) {};
		bool fire() override;
		bool reload() override;
		std::unique_ptr<weapon> clone() const override;
	private:

	};

}