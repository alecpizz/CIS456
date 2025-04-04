#pragma once

#include <Mage/Mage.h>

namespace Galaga
{
    struct ColorComponent
    {
        Mage::Color color = Mage::Color::magenta;
    };

    struct SpriteComponent
    {
        Mage::Sprite* sprite;
    };

    struct TorqueComponent
    {
        float torque = 0.0f;
    };

    struct Transform2DComponent
    {
        glm::vec2 translation = glm::vec2(0.0f, 0.0f);
        glm::vec2 scale = glm::vec2(1.0f, 1.0f);
        float rotation = 0.0f;
        glm::vec2 prev_translation = glm::vec2(0.0f, 0.0f);
    };

    struct RigidBody2DComponent
    {
        glm::vec2 velocity = glm::vec2(0.0f, 0.0f);
        float angular_velocity = 0.0f;
    };

    struct PlayerComponent
    {

    };

    struct EnemyComponent
    {
    };

    struct BoundingBoxComponent
    {
        glm::vec2 center = glm::vec2(0.5f, 0.5f);
        glm::vec2 half_size = glm::vec2(0.5f, 0.5f);
        std::function<void(Mage::Entity*, Mage::Entity*, const glm::vec2&)> on_collided = nullptr;
    };

    struct LifetimeComponent
    {
        float remaining = 0.0f;
        std::function<void()> on_destroyed = nullptr;
    };

    struct DestructionNotificationComponent
    {
        std::function<void()> on_destroyed = nullptr;
    };
}
