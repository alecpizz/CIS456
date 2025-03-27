#pragma once

#include <Mage/Mage.h>
#include "Components.h"

class DestructionNotificationSystem final : public Mage::System
{
public:
    void update(Mage::ComponentManager &componentManager, float deltaTime) override
    {
        for (auto e : get_entities())
        {
            if (e->is_destroyed())
            {
                auto d = componentManager.get_component<DestructionNotificationComponent>(*e);
                if (d->on_destroyed != nullptr)
                {
                    d->on_destroyed();
                }
            }
        }
    }
};