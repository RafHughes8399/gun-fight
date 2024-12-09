#pragma once
// spawn a projectile from a weapon when fired. Exact projectile depends on a weapon
namespace proj {
	class projectile {
	public:
		virtual ~projectile() = default;
		projectile(projectile&& other) = default;
		projectile& operator=(projectile&& other) = default;
		projectile(){};
	private:

	};

	class bullet : public projectile {
	public:
		bullet()
			:projectile() {
		};
	private:

	};
}
