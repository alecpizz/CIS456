#include "CollisionSystem.h"
#include "Galaga.h"

namespace Galaga
{
    CollisionSystem::CollisionSystem(Galaga& game) : _game(&game)
    {
    }

    glm::vec2 CollisionSystem::calculate_overlap(const glm::vec2& translationA, const glm::vec2& scaleA,
        const glm::vec2& translationB, const glm::vec2& scaleB,
        const BoundingBoxComponent* boundingBoxA,
        const BoundingBoxComponent* boundingBoxB)
    {
        auto x1 = (boundingBoxA->center.x * scaleA.x) + translationA.x;
        auto x2 = (boundingBoxB->center.x * scaleB.x) + translationB.x;
        auto y1 = (boundingBoxA->center.y * scaleA.y) + translationA.y;
        auto y2 = (boundingBoxB->center.y * scaleB.y) + translationB.y;

        auto delta_x = std::abs(x1 - x2);
        auto delta_y = std::abs(y1 - y2);

        auto ol_x = (boundingBoxA->half_size.x * scaleA.x) + (boundingBoxB->half_size.x * scaleB.x) - delta_x;
        auto ol_y = (boundingBoxA->half_size.y * scaleA.y) + (boundingBoxB->half_size.y * scaleB.y) - delta_y;
        return { ol_x, ol_y };
    }

    void CollisionSystem::update(Mage::ComponentManager& componentManager, float deltaTime)
    {
        for (auto e1 : get_entities())
        {
            if (e1->is_destroyed())
            {
                continue;
            }

            for (auto e2 : get_entities())
            {
                if (e2->is_destroyed() || e1->get_id() == e2->get_id())
                {
                    continue;
                }

                auto bb1 = componentManager.get_component<BoundingBoxComponent>(*e1);
                auto t1 = componentManager.get_component<Transform2DComponent>(*e1);
                auto bb2 = componentManager.get_component<BoundingBoxComponent>(*e2);
                auto t2 = componentManager.get_component<Transform2DComponent>(*e2);
                auto ol = calculate_overlap(t1->translation, t1->scale,
                    t2->translation, t2->scale, bb1, bb2);
                if (ol.x > 0.0f && ol.y > 0.0f)
                {
                    if (bb1->on_collided != nullptr)
                    {
                        bb1->on_collided(e1, e2, ol);
                    }
                    if (bb2->on_collided != nullptr)
                    {
                        bb2->on_collided(e2, e1, ol);
                    }
                }
            }
        }
    }
}
