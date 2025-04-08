#pragma once

#include <Mage/Mage.h>
#include "Components.h"

namespace Galaga
{
    class Galaga;

    class EnemyControllerSystem final :
        public Mage::System
    {
    public:
        explicit EnemyControllerSystem(Galaga* game);

        ~EnemyControllerSystem() override = default;

        EnemyControllerSystem(EnemyControllerSystem&&) = delete;
        EnemyControllerSystem(const EnemyControllerSystem&) = delete;
        EnemyControllerSystem& operator=(EnemyControllerSystem&&) = delete;
        EnemyControllerSystem& operator=(const EnemyControllerSystem&) = delete;

        void update(Mage::ComponentManager& componentManager, float deltaTime) override;

        void collision_detected(Mage::Entity* other_entity, const glm::vec2& overlap);

    private:
        Galaga* _game;
        Mage::Entity* _enemy_entity;
        std::map<std::string, std::shared_ptr<Mage::Sprite> > _enemy_sprites;

        bool _shooting = false;
        float _last_shot = 0.0f;

        void shoot();

        void update_enemy_velocity(RigidBody2DComponent* r, float dt);

        void create_enemy_entity();
        void place_enemy_entity();
        void spawn();
        /*void update_enemy_sprite(const RigidBody2DComponent* r, SpriteComponent* sprite, Transform2DComponent* t,
            BoundingBoxComponent* b);*/
    };
}