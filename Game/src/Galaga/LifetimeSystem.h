#pragma once

#include <Mage/Mage.h>
#include "Components.h"

namespace Galaga
{
    class LifetimeSystem final : public Mage::System
    {
    public:
        void update(Mage::ComponentManager& componentManager, float deltaTime) override
        {
            for (auto e : get_entities())
            {
                if (e->is_destroyed())
                {
                    continue;
                }
                auto l = componentManager.get_component<LifetimeComponent>(*e);

                l->remaining -= deltaTime;
                if (l->remaining <= 0.0f)
                {
                    e->destroy();
                    if (l->on_destroyed != nullptr)
                    {
                        l->on_destroyed();
                    }
                }
            }
        }
    };
}
