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


        for (uint32_t i = 0; i < _rows; i++)
        {
            for (uint32_t j = 0; j < _cols; j++)
            {
            }
        }
    }

    void EnemySpawner::update(Mage::ComponentManager &componentManager, float deltaTime)
    {
    }
}
