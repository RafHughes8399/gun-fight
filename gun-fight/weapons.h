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

			weapon_state() {};
			virtual bool fire(weapon* w) = 0;
			virtual bool reload(weapon* w) = 0;
		protected:
		};

		class loaded_state : public weapon_state {
		public:
			loaded_state() {};
			bool fire(weapon* w) override;
			bool reload(weapon* w) override;

		private:

		};

		class unloaded_state : public weapon_state {
		public:
			unloaded_state() {};
			bool fire(weapon* w) override;
			bool reload(weapon* w) override;
		private:

		};

		virtual ~weapon() = default;
		weapon(weapon&& other) = default;
		weapon& operator=(weapon&& other) = default;
		weapon(int ammo, int damage)
			: ammo_(ammo), damage_(damage), state_(std::make_unique<loaded_state>(loaded_state())){};

		int get_ammo() { return ammo_; }
		int get_damage() {return damage_;}
		bool is_loaded();
		virtual bool fire() = 0;
		virtual bool reload() = 0;

		friend class weapon_state;
	protected:
		int ammo_;
		const int damage_;
		std::unique_ptr<weapon_state> state_; // if you wanted a get_state return a raw pointer
		std::unique_ptr<proj::projectile> bullet_type_;
	};
	//TODO 
	class revolver : public weapon {
	public:
		revolver()
			: weapon(REVOLVER_AMMO, REVOLVER_DMG) {
		};
		bool fire() override;
		bool reload() override;
	private:

	};

}