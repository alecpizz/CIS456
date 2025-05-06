#pragma once

#include "../Core/Core.h"

namespace Mage
{
    class MAGE_API Entity
    {
        friend class EntityManager;
    public:
        ~Entity()
        {
            LOG_E_DEBUG("Destroyed an entity with id: %d. type %d", _id, _type);
        }

        //prevent copying of entities :)
        Entity(Entity &&) = delete;

        Entity(const Entity &) = delete;

        Entity &operator=(Entity &&) = delete;

        Entity &operator=(const Entity &) = delete;

        uint64_t get_id() const
        {
            return _id;
        }

        uint32_t get_type() const
        {
            return _type;
        }

        bool is_destroyed() const
        {
            return _destroyed;
        }

        void destroy()
        {
            _destroyed = true;
        }

    private:
        Entity(uint64_t id, uint32_t type) : _id(id), _type(type)
        {
            LOG_E_DEBUG("Created an entity with id: %d. type %d", _id, _type);
        }

        const uint64_t _id;
        const uint32_t _type;
        bool _destroyed = false;
    };
}
