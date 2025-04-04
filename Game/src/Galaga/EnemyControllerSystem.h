#pragma once

#include <Mage/Mage.h>
#include "Components.h"

namespace Galaga
{
    class Game;

    class EnemyControllerSystem final :
        public Mage::System
    {
    public:
        explicit EnemyControllerSystem(Game* game);

        ~EnemyControllerSystem() override = default;

        EnemyControllerSystem(EnemyControllerSystem&&) = delete;
        EnemyControllerSystem(const EnemyControllerSystem&) = delete;
        EnemyControllerSystem& operator=(EnemyControllerSystem&&) = delete;
        EnemyControllerSystem& operator=(const EnemyControllerSystem&) = delete;

        void update(Mage::ComponentManager& componentManager, float deltaTime) override;

    private:
        Game* _game;
        Mage::Entity* _enemy_entity;
        std::map<std::string, std::shared_ptr<Mage::Sprite> > _enemy_sprites;

        void update_enemy_velocity(RigidBody2DComponent* r, float dt);

        /*void update_enemy_sprite(const RigidBody2DComponent* r, SpriteComponent* sprite, Transform2DComponent* t,
            BoundingBoxComponent* b);*/
    };
}