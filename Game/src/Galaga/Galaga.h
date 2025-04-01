#pragma once
#include <Mage/Mage.h>
#include "StarfighterSystem.h"

namespace Galaga
{
    class Galaga :
            public Mage::Application,
            public Mage::OnAppClosingEventListener
    {
    public:
        Galaga();

    private:
        std::unique_ptr<StarfighterSystem> _starfighter_system = nullptr;

        void on_app_closing() override;
    };
}
