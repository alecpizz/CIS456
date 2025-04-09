#pragma once
#include <Mage/Mage.h>

namespace Galaga
{
    class Galaga;
    class EnemySpawner final : public Mage::System
    {
    public:
        explicit EnemySpawner(Galaga* galaga);

        ~EnemySpawner() override = default;

        EnemySpawner(EnemySpawner&&) = delete;
        EnemySpawner(const EnemySpawner&) = delete;
        EnemySpawner& operator=(EnemySpawner&&) = delete;
        EnemySpawner& operator=(const EnemySpawner&) = delete;

        void initialize();
        void update(Mage::ComponentManager &componentManager, float deltaTime) override;

    private:
        std::vector<Mage::Entity*> _enemies;
        Galaga* _game;
        Mage::Entity* _enemy_entity;
        std::map<std::string, std::shared_ptr<Mage::Sprite> > _enemy_sprites;

        void create_enemy_entity();
        void place_enemy_entity();
        void spawn();
    };
}
