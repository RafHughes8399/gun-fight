#pragma once
#include "entities.h"
#include <random>
#include <set>
namespace level {

	// abstract base class for the bulding interface
	class level_builder {
	public:
		virtual ~level_builder() = default;
		level_builder(level_builder&& other) = default;
		level_builder& operator=(level_builder&& other) = default;

		level_builder(int level_category, int obstacles_to_generate)
			: level_category_(level_category), obstacles_to_generate_(obstacles_to_generate) {};
		virtual void build_level() = 0;
		virtual void build_tumbleweed() = 0;
		virtual void build_cacti() = 0;
		virtual void build_barrels() = 0;
		virtual void build_wagons() = 0;
		virtual void build_train() = 0;
		std::set<std::unique_ptr<entities::entity>>& get_level_entities();

	protected:
		// you can move the pointers to the game_entities after building
		std::set<std::unique_ptr<entities::entity>> level_entities_ = {};
		int level_category_;
		int obstacles_to_generate_;
	};

	class train_level : public level_builder {
	public:
		train_level(int level_category, int obstacles_to_generate)
			: level_builder(level_category, obstacles_to_generate) {};
		void build_level() override;
		void build_tumbleweed() override;
		void build_cacti() override;
		void build_barrels() override;
		void build_wagons() override;
		void build_train() override;
	private:

	};

	class level : public level_builder {
	public:
		level(int level_category, int obstacles_to_generate)
			: level_builder(level_category, obstacles_to_generate) {};
		void build_level() override;
		void build_tumbleweed() override;
		void build_cacti() override;
		void build_barrels() override;
		void build_wagons() override;
		void build_train() override;
	private:

	};
}