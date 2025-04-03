#pragma once

#include <Mage/Mage.h>
#include "Components.h"

namespace Galaga
{
    class MovementSystem final : public Mage::System
    {
    public:
        void update(Mage::ComponentManager& component_manager, float delta_time) override
        {
            for (auto e : get_entities())
            {
                if (e->is_destroyed())
                {
                    continue;
                }
                auto rb = component_manager.get_component<RigidBody2DComponent>(*e);
                auto t = component_manager.get_component<Transform2DComponent>(*e);
                t->prev_translation = t->translation;
                t->translation += rb->velocity * delta_time;
                t->rotation += rb->angular_velocity * delta_time;
            }
        }
    };
}
