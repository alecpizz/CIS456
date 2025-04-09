//
// Created by alecp on 4/1/2025.
//
#include "EnemySpawner.h"

#include "Galaga.h"

namespace Galaga
{
    EnemySpawner::EnemySpawner(Galaga *galaga, uint32_t rows, uint32_t cols) : _galaga(galaga), _rows(rows), _cols(cols)
    {
    }

    void EnemySpawner::update(Mage::ComponentManager &componentManager, float deltaTime)
    {
    }

    void EnemySpawner::spawn()
    {
        uint32_t enemy_count = 30;
        uint32_t width = 1;
        uint32_t height = 1;
        while (true)
        {
            if (width * height >= enemy_count)
            {
                break;
            }
            width++;
            if (width * height >= enemy_count)
            {
                break;
            }
            height++;
        }
        float buffer_x = 1.5f;
        float buffer_y = 1.5f;
        float enemy_width = 60;
        float enemy_height = 60;
        glm::vec2 origin = glm::vec2(_galaga->get_window()->get_width() / 2, _galaga->get_window()->get_height() / 2);
        float x_first = origin.x - (width * enemy_width / 2) - ((width - 1) * buffer_x / 2);
        float y_first = origin.y - (height * enemy_height / 2) - ((height - 1) * buffer_y / 2);

        //assume all entites have been destroyed...?
        _enemies.clear();
        for (uint32_t i = 0; i < enemy_count; i++)
        {
            uint32_t x = i % width;
            uint32_t y = i / width;
            float xPos = x_first + (x * (enemy_width * buffer_x));
            float yPos = y_first + (y * (enemy_height * buffer_y));
            glm::vec2 pos = glm::vec2(xPos, yPos);
            auto e = _galaga->get_entity_manager()->add_entity(69);
            _galaga->get_component_manager()->add_component(*e, Transform2DComponent{
                                                                .translation = pos,
                                                                .scale = {enemy_width, enemy_height}
                                                            });
            _galaga->get_component_manager()->add_component(*e, ColorComponent{
                                                                .color = Mage::Color::white
                                                            });
            //add enemy component here


            _enemies.push_back(e);
        }
    }
}
