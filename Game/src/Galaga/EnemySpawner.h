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
        void update(Mage::ComponentManager &componentManager, float deltaTime) override;
        void collision_detected(Mage::Entity* enemy, Mage::Entity* other_entity, const glm::vec2& overlap);

    private:
        std::vector<Mage::Entity*> _enemies;
        Galaga* _game;
        Mage::Entity* _enemy_entity;
        uint32_t _rows, _cols;
        std::map<std::string, std::shared_ptr<Mage::Sprite> > _enemy_sprites;

        RandomWrapper _rands;

        void create_enemy_entity(glm::vec2 pos);
        void spawn();


        //Working on shooting
        bool _shooting = false;
        float _last_shot = 0.0f;
        //float _enemy_shooting_rate = 0.0f;


        void shoot();

        void add_bullet();

        void kill_player(Mage::Entity* bullet, Mage::Entity* other);
    };
}
