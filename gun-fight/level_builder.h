#pragma once
#include <memory>
#include <vector>
#include "entities.h"
namespace level {

	// abstract base class for the bulding interface
	class level_builder {
	public:
		virtual ~level_builder() = default;
		level_builder(level_builder&& other) = default;
		level_builder& operator=(level_builder&& other) = default;

		level_builder() {};
		level_builder(const level_builder& other) {};
		
		
		virtual level_builder& operator=(const level_builder& other);
		virtual void build_tumbleweed() = 0;
		virtual void build_cacti() = 0;
		virtual void build_barrels() = 0;
		virtual void build_wagons() = 0;
		virtual void build_train() = 0;

		std::vector<std::unique_ptr<entities::entity>> get_level_entities();
		
	protected:
		// you can move the pointers to the game_entities after building
		std::vector<std::unique_ptr<entities::entity>> level_entities_;
	};
	// builder for rounds 1 - 5
	class level1_5 : public level_builder {
	public:
		// constructors
		level1_5() 
		: level_builder(){}

		level1_5(const level1_5& other) 
		: level_builder(other){}

		// methods
		void build_tumbleweed() override;
		void build_cacti() override;
		void build_barrels() override;
		void build_wagons() override;
		void build_train() override;

	private:
	};

	// builder for rounds 6 - 9
	class level6_9 : public level_builder {
	public: 
		// constructors
		level6_9()
			: level_builder() {
		}

		level6_9(const level6_9& other)
			: level_builder(other) {
		}

		// methods
		void build_tumbleweed() override;
		void build_cacti() override;
		void build_barrels() override;
		void build_wagons() override;
		void build_train() override;
	private:

	};

	// builder for rounds 11 - 15
	class level11_15 : public level_builder {
	public:
		// constructors
		level11_15()
			: level_builder() {
		}

		level11_15(const level11_15& other)
			: level_builder(other) {
		}

		// methods
		void build_tumbleweed() override;
		void build_cacti() override;
		void build_barrels() override;
		void build_wagons() override;
		void build_train() override;
	private:
	};

	// builder for rounds 16 - 19
	class level16_19 : public level_builder {
	public:
		// constructors
		level16_19()
			: level_builder() {
		}

		level16_19(const level16_19& other)
			: level_builder(other) {
		}

		// methods
		void build_tumbleweed() override;
		void build_cacti() override;
		void build_barrels() override;
		void build_wagons() override;
		void build_train() override;
	private:

	};
	// builder for rounds 21 - 25
	class level21_25 : public level_builder {
	public:
		// constructors
		level21_25()
			: level_builder() {
		}

		level21_25(const level21_25& other)
			: level_builder(other) {
		}

		// methods
		void build_tumbleweed() override;
		void build_cacti() override;
		void build_barrels() override;
		void build_wagons() override;
		void build_train() override;
	private:

	};

	class train_level : public level_builder {
	public:
		// constructors
		train_level()
			: level_builder() {
		}

		train_level(const train_level& other)
			: level_builder(other) {
		}

		// methods
		void build_tumbleweed() override;
		void build_cacti() override;
		void build_barrels() override;
		void build_wagons() override;
		void build_train() override;
	private:

	};

	
}

