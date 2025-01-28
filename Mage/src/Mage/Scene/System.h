#pragma once

#include "EntityList.h"
#include "../Core/Core.h"

namespace Mage
{

    class ComponentManager;

    class MAGE_API System
    {
        friend class SystemManager;

    public:
        System();

        virtual ~System();

        //prevent copying of entities :)
        System(System &&) = delete;

        System(const System &) = delete;

        System &operator=(System &&) = delete;

        System &operator=(const System &) = delete;

        virtual void update(ComponentManager& componentManager, float deltaTime) = 0;
        EntityList get_entities() const;

    private:
        struct Impl;
        Impl* _impl;
        void add_entity(Entity& entity);
        void remove_entity(Entity& entity);
    };
}