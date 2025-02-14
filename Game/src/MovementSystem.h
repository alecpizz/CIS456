#pragma once

#include <Mage/Mage.h>
#include "Components.h"

class MovementSystem final : public Mage::System
{
private:
    float _yMax;
public:
    explicit MovementSystem(float yVal) :_yMax(yVal)
    {

    }
    void update(Mage::ComponentManager &component_manager, float delta_time) override
    {
        for (auto e: get_entities())
        {
            if (e->is_destroyed())
            {
                continue;
            }
            auto rb = component_manager.get_component<RigidBody2DComponent>(*e);
            auto t = component_manager.get_component<Transform2DComponent>(*e);
            t->translation += rb->velocity;
        }
    }
};