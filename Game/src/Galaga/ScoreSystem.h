#pragma once

#include <Mage/Mage.h>
#include "Components.h"

namespace Galaga {

	class Galaga;

	class ScoreSystem final :
		public Mage::System,
		public Mage::OnKeyDownEventListener
	{
	public:
		explicit ScoreSystem(Galaga* galaga);

		void initialize();

		void update(Mage::ComponentManager& component_manager, float delta_time) override;

		void set_player_entity(Mage::Entity* player_entity);

		void sort_scoreboard();

		std::string fill_score(uint32_t score, size_t width) const;

		void on_key_down(Mage::Key key, uint16_t key_modifiers, uint8_t repeat_count) override;

	private:
		Galaga* _game;
		std::unique_ptr<Mage::Font> _font = nullptr;
		Mage::Entity* _player_entity = nullptr;
		bool _show_scoreboard = false;
	};
}