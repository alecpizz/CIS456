#include "EnemyController.h"
#include "Galaga.h"

//Add Defined Values here
#define	VELOCITY_ENEMY 500.0f

EnemyController::EnemyController(Game* game) : _game(game)
{
}

void EnemyController::initialize()
{

}

void EnemyController::update(Mage::ComponentManager& componentManager, float deltaTime)
{

}

void EnemyController::collision_detected(Mage::Entity* other_entity, const glm::vec2& overlap)
{
}

void EnemyController::create_enemy_entity()
{
}

void EnemyController::update_enemy_velocity(RigidBody2DComponent* r, float dt)
{
}

void EnemyController::update_enemy_sprite(const RigidBody2DComponent* r, SpriteComponent* sprite, Transform2DComponent* t, BoundingBoxComponent* b)
{
}
