#pragma once
#include <Mage/Mage.h>

namespace Galaga
{
    class Galaga;
    class EnemySpawner final : public Mage::System
    {
    public:
        EnemySpawner(Galaga* galaga, uint32_t rows, uint32_t cols);
        void update(Mage::ComponentManager &componentManager, float deltaTime) override;
    private:
        Galaga* _galaga;
        uint32_t _rows, _cols;
    };
}
