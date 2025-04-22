#include "ScoreSystem.h"
#include "Galaga.h"

namespace Galaga {

	ScoreSystem::ScoreSystem(Galaga* galaga) : _game(galaga)
	{
		_font = std::make_unique<Mage::Font>("res/fonts/Joystix-Monospace.ttf");
	}

	void ScoreSystem::update(Mage::ComponentManager& component_manager, float delta_time)
	{
		
		auto score = component_manager.get_component<ScoreComponent>(*_player_entity);
		std::string txt = "Score: " + std::to_string(score->current);
		_game->get_text_renderer()->render_text(*_font, txt.c_str(),
			10.0f, static_cast<float>(_font->get_line_height()) - 5.0f,
			0.5f, Mage::Color::white);
	}

	void ScoreSystem::set_player_entity(Mage::Entity* player_entity)
	{
		_player_entity = player_entity;
		if (_player_entity)
			LOG_INFO("Player entity set");
		else
			LOG_ERROR("Player entity not set");
	}

}