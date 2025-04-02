//
// Created by alecp on 4/1/2025.
//
#include "EnemySpawner.h"

#include "Galaga.h"

namespace Galaga
{
    EnemySpawner::EnemySpawner(Galaga *galaga, uint32_t rows, uint32_t cols) : _galaga(galaga), _rows(rows), _cols(cols)
    {
        auto window_width = _galaga->get_window()->get_width();
        auto window_height = _galaga->get_window()->get_height();

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
        uint32_t buffer = 10;
        uint32_t enemy_width = 20;
        uint32_t enemy_height = 20;
        for (uint32_t i = 0; i < enemy_count; i++)
        {
            uint32_t x = i % width;
            uint32_t y = i / width;
            uint32_t xPos = x * enemy_width + buffer;
            uint32_t yPos = y * enemy_width + buffer;
            LOG_INFO("ENEMY POS (%d, %d)", xPos, yPos);
        }
    }

    void EnemySpawner::update(Mage::ComponentManager &componentManager, float deltaTime)
    {
    }
}
