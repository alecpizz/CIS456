#pragma once

#include <random>
#include <Mage/Mage.h>
#include "Components.h"

class Game ;

class EnemySpawningSystem final : public Mage::System
{
    Game* _game = nullptr;
    float _last_spawn = 0.0f;
    std::unique_ptr<Mage::Sprite> _enemy_sprite;
    std::map<uint64_t, std::unique_ptr<Mage::Sprite>> _enemy_instances;
public:
    explicit EnemySpawningSystem(Game& game);

    void update(Mage::ComponentManager &componentManager, float deltaTime) override;
};