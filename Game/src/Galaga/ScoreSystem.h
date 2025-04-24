#pragma once

#include <Mage/Mage.h>
#include "Components.h"

namespace Galaga {

	class Galaga;

	class ScoreSystem : public Mage::System
	{
	public:
		explicit ScoreSystem(Galaga* galaga);

		void update(Mage::ComponentManager& component_manager, float delta_time) override;

		void set_player_entity(Mage::Entity* player_entity);

	private:
		Galaga* _game;
		std::unique_ptr<Mage::Font> _font = nullptr;
		Mage::Entity* _player_entity = nullptr;
	};
}