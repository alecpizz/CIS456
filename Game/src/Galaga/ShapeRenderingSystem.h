#pragma once

#include <Mage/Mage.h>
#include "Components.h"

namespace Galaga
{
    class ShapeRenderingSystem final : public Mage::System
    {
    private:
        Mage::ShapeRenderer* _renderer;
        Mage::Shape _shape;
    public:
        explicit ShapeRenderingSystem(Mage::ShapeRenderer& shape_renderer) : _renderer(&shape_renderer)
        {

        }

        void update(Mage::ComponentManager& component_manager, float delta_time) override
        {
            for (auto e : get_entities())
            {
                if (e->is_destroyed())
                {
                    continue;
                }
                auto s = component_manager.get_component<ColorComponent>(*e);
                auto t = component_manager.get_component<Transform2DComponent>(*e);

                _shape.set_color(s->color);
                _renderer->render_shape(_shape, t->translation, t->scale, t->rotation, delta_time);
            }
        }
    };
}
