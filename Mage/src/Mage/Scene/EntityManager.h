#pragma once

#include "../Core/Core.h"
#include "Entity.h"
#include "EntityList.h"

namespace Mage
{

    class SystemManager;

    class ComponentManager;

    class MAGE_API EntityManager
    {
        friend class Application;

    public:
        ~EntityManager();

        EntityManager(EntityManager &&) = delete;

        EntityManager(const EntityManager &) = delete;

        EntityManager &operator=(EntityManager &&) = delete;

        EntityManager &operator=(const EntityManager &) = delete;

        void update();

        Entity* add_entity(uint32_t type);
        EntityList get_all_entities() const;
        EntityList get_all_entities_by_type(uint32_t type) const;
    private:
        struct Impl;
        Impl* _impl;
        EntityManager();
        void set_component_manager(ComponentManager& componentManager);
        void set_system_manager(SystemManager& systemManager);

    };
}
