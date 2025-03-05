#pragma once

#include <Mage/Mage.h>
#include "Components.h"

class SpriteRenderingSystem final : public Mage::System
{
public:
    explicit SpriteRenderingSystem(Mage::SpriteRenderer& sprite_renderer) : _sprite_renderer(&sprite_renderer)
    {
    }
    void update(Mage::ComponentManager &componentManager, float deltaTime) override
    {
        for(auto e : get_entities())
        {
            if(e->is_destroyed())
            {
                continue;
            }
            auto s = componentManager.get_component<SpriteComponent>(*e);
        }
    }
private:
    Mage::SpriteRenderer* _sprite_renderer = nullptr;
};