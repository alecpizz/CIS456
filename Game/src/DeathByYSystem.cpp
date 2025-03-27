#include "DeathByYSystem.h"
#include "Game.h"

DeathByYSystem::DeathByYSystem(Game &game) : _game(&game)
{
}

void DeathByYSystem::update(Mage::ComponentManager &componentManager, float deltaTime)
{
    for (auto e: get_entities())
    {
        if (e->is_destroyed())
        {
            continue;
        }
        auto t = componentManager.get_component<Transform2DComponent>(*e);
        auto s = componentManager.get_component<SpriteComponent>(*e);
        if (t->translation.y < 0.0f - s->sprite->get_height())
        {
            e->destroy();
        }
    }
}
