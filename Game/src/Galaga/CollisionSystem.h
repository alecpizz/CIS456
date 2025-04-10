#pragma once

#include <Mage/Mage.h>
#include "Components.h"

namespace Galaga
{

    class Galaga;

    class CollisionSystem : public Mage::System
    {
    public:
        explicit CollisionSystem(Galaga& game);

        static glm::vec2 calculate_overlap(const glm::vec2& translationA, const glm::vec2& scaleA,
            const glm::vec2& translationB, const glm::vec2& scaleB,
            const BoundingBoxComponent* boundingBoxA,
            const BoundingBoxComponent* boundingBoxB);

        void update(Mage::ComponentManager& componentManager, float deltaTime) override;

    private:
        Galaga* _game;
    };
}
