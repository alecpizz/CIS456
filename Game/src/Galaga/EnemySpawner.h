#pragma once
#include <Mage/Mage.h>
#include "RandomWrapper.h"

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
        void update(Mage::ComponentManager &componentManager, float delta_time) override;
        void collision_detected(Mage::Entity* enemy, Mage::Entity* other_entity, const glm::vec2& overlap);

    private:
        std::vector<Mage::Entity*> _enemies;
        Galaga* _game;
        Mage::Entity* _enemy_entity;
        uint32_t _rows, _cols;
        std::map<std::string, std::shared_ptr<Mage::Sprite> > _enemy_sprites;
        std::map<uint64_t, std::unique_ptr<Mage::Sprite>> _enemy_instances;
        

        RandomWrapper _rands;

        void shoot(float delta_time);
        void update_enemy_sprites(Mage::Entity* e);
        void update_enemy_sprites();
        void create_enemy_entity(glm::vec2 pos);
        void spawn();
        void kill_player(Mage::Entity* bullet, Mage::Entity* other);
    };
}
