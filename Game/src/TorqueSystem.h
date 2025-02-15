#pragma once

#include <Mage/Mage.h>
#include "Components.h"

class TorqueSystem final : public Mage::System
{
public:
    void update(Mage::ComponentManager& component_manager, float delta_time) override
    {
        for(auto& e : get_entities())
        {
            if(e->is_destroyed())
            {
                continue;
            }
            auto rb = component_manager.get_component<RigidBody2DComponent>(*e);
            auto tq = component_manager.get_component<TorqueComponent>(*e);
            rb->angular_velocity += tq->torque * delta_time;
        }
    }
};