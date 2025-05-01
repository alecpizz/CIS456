#pragma once

#include <Mage/Mage.h>
#include "Components.h"
#include "Galaga.h"

namespace Galaga
{
    class SpriteRenderingSystem final : public Mage::System
    {
    public:
        explicit SpriteRenderingSystem(Mage::SpriteRenderer& sprite_renderer) : _sprite_renderer(&sprite_renderer)
        {
        }
        void update(Mage::ComponentManager& componentManager, float deltaTime) override
        {
            for (auto e : get_entities())
            {
                if (e->is_destroyed())
                {
                    continue;
                }
                auto s = componentManager.get_component<SpriteComponent>(*e);
                auto t = componentManager.get_component<Transform2DComponent>(*e);
                _sprite_renderer->render_sprite(*s->sprite, t->translation, t->scale, t->rotation, deltaTime);
            }
        }
    private:
        Mage::SpriteRenderer* _sprite_renderer = nullptr;
    };
}
