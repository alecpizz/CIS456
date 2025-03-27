#pragma once

#include <Mage/Mage.h>
#include "Components.h"

class Game ;

class DeathByYSystem final : public Mage::System
{
    Game* _game;
public:
    explicit DeathByYSystem(Game& game);
    void update(Mage::ComponentManager &componentManager, float deltaTime) override;
};