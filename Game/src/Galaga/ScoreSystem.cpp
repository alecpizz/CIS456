#include "ScoreSystem.h"
#include "Galaga.h"

namespace Galaga {

	ScoreSystem::ScoreSystem(Galaga* galaga) : _game(galaga)
	{
		_font = std::make_unique<Mage::Font>("res/fonts/Joystix-Monospace.ttf");
		//_scoreboard_path = "res/scoreboard.txt";
	}

	void ScoreSystem::initialize()
	{
		_game->get_event_manager()->add_on_key_down_event_listener(this);
	}


	void ScoreSystem::update(Mage::ComponentManager& component_manager, float delta_time)
	{
		auto score = component_manager.get_component<ScoreComponent>(*_player_entity);
		_game->get_text_renderer()->render_text(*_font, "SCORE",
			10.0f, static_cast<float>(_font->get_line_height()) - 5.0f,
			0.5f, Mage::Color::red);
		_game->get_text_renderer()->render_text(*_font, std::to_string(score->current).c_str(),
			10.0f, static_cast<float>(_font->get_line_height()) + 15.0f,
			0.5f, Mage::Color::white);

		if (_show_scoreboard)
		{
			_game->get_text_renderer()->render_text(*_font, "TOP 5",
				static_cast<float>(_game->get_window()->get_width()) - 110.0f,
				static_cast<float>(_font->get_line_height()) - 5.0,
				0.5f, Mage::Color::red);
			std::vector<std::string> names = { "1 ", "2 ", "3 ", "4 ", "5 " };
			for (size_t s = 0; s < score->highest.size(); s++)
			{
				_game->get_text_renderer()->render_text(*_font, (names[s] + fill_score(score->highest[s], 6)).c_str(),
					static_cast<float>(_game->get_window()->get_width()) - 170.0f,
					static_cast<float>(_font->get_line_height()) + 15 + s * 20,
					0.5f, Mage::Color::white);
			}
		}
	}

	void ScoreSystem::set_player_entity(Mage::Entity* player_entity)
	{
		_player_entity = player_entity;
		if (_player_entity)
			LOG_INFO("Player entity set");
		else
			LOG_ERROR("Player entity not set");
	}

	void ScoreSystem::sort_scoreboard()
	{
		auto score = _game->get_component_manager()->get_component<ScoreComponent>(*_player_entity);
		std::sort(score->highest.begin(), score->highest.end(), std::greater<uint32_t>());
	}

	void ScoreSystem::on_key_down(Mage::Key key, uint16_t key_modifiers, uint8_t repeat_count)
	{
		if (repeat_count > 0) return;
		if (key == Mage::Key::Tab)
			_show_scoreboard = !_show_scoreboard;

		if (key == Mage::Key::R)
		{
			auto score = _game->get_component_manager()->get_component<ScoreComponent>(*_player_entity);
			if (score->current > score->highest[4])
			{
				score->highest[4] = score->current;
				sort_scoreboard();
			}
			score->current = 0;
		}
	}

	std::string ScoreSystem::fill_score(uint32_t score, size_t width) const
	{
		std::string str = std::to_string(score);
		while (str.length() < width)
			str = " " + str;
		return str;
	}
}