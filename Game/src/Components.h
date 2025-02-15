#pragma once

#include <Mage/Mage.h>

struct ColorComponent
{
    Mage::Color color = Mage::Color::magenta;
};

struct GravityComponent
{
    glm::vec2 force = glm::vec2(0.0f, 0.0f);
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