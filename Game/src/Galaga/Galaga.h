#pragma once
#include <Mage/Mage.h>
#include "EnemySpawner.h"
#include "../ShapeRenderingSystem.h"

namespace Galaga
{
    class Galaga :
            public Mage::Application,
            public Mage::OnAppClosingEventListener
    {
    public:
        Galaga();
    private:
        void on_app_closing() override;
        std::unique_ptr<EnemySpawner> _enemy_spawner;
        std::unique_ptr<ShapeRenderingSystem> _shape_renderer;
    };
}
