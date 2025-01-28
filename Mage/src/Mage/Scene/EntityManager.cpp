#include "EntityManager.h"
#include "SystemManager.h"
#include "ComponentManager.h"

namespace Mage
{
    struct EntityManager::Impl
    {
        std::mutex sync_object;
        ComponentManager *component_manager;
        SystemManager *system_manager;
        std::vector<std::unique_ptr<Entity>> all_entities;
        std::vector<std::unique_ptr<Entity>> pending_entity_additions;
        std::vector<Entity *> all_entity_ptrs;
        std::map<uint32_t, std::vector<Entity *>> entity_type_map;
        uint64_t last_entity_id = 0;
    };

    EntityManager::EntityManager()
    {
        LOG_E_INFO("EntityManager starting.");
        _impl = new Impl();
    }

    EntityManager::~EntityManager()
    {
        LOG_E_INFO("EntityManager stopping.");
        delete _impl;
    }

    void EntityManager::set_component_manager(Mage::ComponentManager &componentManager)
    {
        _impl->component_manager = &componentManager;
    }

    void EntityManager::set_system_manager(Mage::SystemManager &systemManager)
    {
        _impl->system_manager = &systemManager;
    }

    Entity *EntityManager::add_entity(uint32_t type)
    {
        //prevent other things from accessing when adding
        std::lock_guard lock(_impl->sync_object);
        auto new_id = ++_impl->last_entity_id;
        auto e = std::unique_ptr<Entity>(new Entity(new_id, type));
        auto pe = e.get();

        _impl->pending_entity_additions.push_back(std::move(e));
        LOG_E_DEBUG("Entity added to entity manager. ID: %d, type %d", new_id, type);
        return pe;
    }

    EntityList EntityManager::get_all_entities() const
    {
        //TODO: implementation
        return EntityList();
    }

    EntityList EntityManager::get_all_entities_by_type(uint32_t ) const
    {
        //TODO: implementation
        return EntityList();
    }

    void EntityManager::update()
    {
        LOG_E_DEBUG("Entity Manager Update Called");
        std::lock_guard lock(_impl->sync_object);

        /*************************************/
        //move pending additions into use

        //   loop thur pending entity additions
        //      push entity into all entities
        //      push entity ptr into all entity ptrs
        //      push entity ptr into entity type map
        //   clear list of pending entity additions
        /***********************************************/


        /****************************************/
        //move pending deletions from use

        //  erase/remove idiom (using remove_if) for all entities in entity ptrs that are marked destroyed
        // loop thru types in entity map
        //  erase/remove idiom (using remove_if) for all entities in entity type map

        //loop thru entities in all entities
        //  if entity is destroyed, tell system manager about it, tell component manager about it,
        //  and finally erase/remove idiom (using remove_if) for all entities in entity list
        //  that are marked destroyed

        /****************************************/
    }
}
