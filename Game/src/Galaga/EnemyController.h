#pragma once

#include <Mage/Mage.h>
#include "Components.h"

class Game;

class EnemyController final :
    public Mage::System
{
public:
    explicit EnemyController(Game* game);

    ~EnemyController() override = default;

    void initialize();

    EnemyController(EnemyController&&) = delete;
    EnemyController(const EnemyController&) = delete;
    EnemyController& operator=(EnemyController&&) = delete;
    EnemyController& operator=(const EnemyController&) = delete;

    void update(Mage::ComponentManager& componentManager, float deltaTime) override;

    void collision_detected(Mage::Entity* other_entity, const glm::vec2& overlap);

private:
    Game* _game;
    Mage::Entity* _enemy_entity;
    std::map<std::string, std::shared_ptr<Mage::Sprite> > _enemy_sprites;
    
    void create_enemy_entity();

    void update_enemy_velocity(RigidBody2DComponent* r, float dt);

    void update_enemy_sprite(const RigidBody2DComponent* r, SpriteComponent* sprite, Transform2DComponent* t,
        BoundingBoxComponent* b);
};
