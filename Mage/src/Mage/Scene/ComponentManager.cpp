#include "ComponentManager.h"
#include "SystemManager.h"

namespace Mage
{
    struct ComponentManager::Impl
    {
        std::mutex sync_object;
        SystemManager *system_manager;
        std::unordered_map<size_t, uint32_t> component_type_to_id_map;
        std::unordered_map<uint32_t, ComponentVectorBase *> component_vectors;
        uint32_t last_component_id;
    };

    ComponentManager::ComponentManager()
    {
        LOG_E_INFO("Component manager starting");
        _impl = new Impl();
    }

    ComponentManager::~ComponentManager()
    {
        LOG_E_INFO("Component manager stopping");
        for (auto &cv: _impl->component_vectors)
        {
            delete cv.second;
        }
        delete _impl;
    }


    void ComponentManager::set_system_manager(SystemManager &system)
    {
        _impl->system_manager = &system;
    }

    void ComponentManager::register_component_internal(size_t type_idx, ComponentVectorBase *component_vector)
    {
        std::lock_guard lock(_impl->sync_object);
        auto new_id = ++_impl->last_component_id;
        _impl->component_type_to_id_map.insert(std::make_pair(type_idx, new_id));
        _impl->component_vectors.insert(std::make_pair(new_id, component_vector));
    }

    bool ComponentManager::get_component_id(size_t type_idx, uint32_t *component_id)
    {
        auto it = _impl->component_type_to_id_map.find(type_idx);
        if (it == _impl->component_type_to_id_map.end())
        {
            return false;
        }

        *component_id = it->second;
        return true;
    }

    ComponentVectorBase *ComponentManager::get_component_vector(uint32_t component_id)
    {
        return _impl->component_vectors[component_id];
    }

    void ComponentManager::notify_systems_of_entity_component_addition(Entity &entity, uint32_t component_id)
    {
        std::vector<uint32_t> entity_components;

        entity_components.push_back(component_id);

        for (auto &cv: _impl->component_vectors)
        {
            auto entities = cv.second->get_entities();
            if (std::find(entities.begin(), entities.end(), &entity) != entities.end())
            {
                entity_components.push_back(cv.first);
            }
        }

        _impl->system_manager->notify_systems_of_entity_component_additions(entity, entity_components.data(),
                                                                            entity_components.size());
    }

    void ComponentManager::notify_systems_of_entity_component_removal(Entity &entity, uint32_t component_id)
    {
        _impl->system_manager->notify_systems_of_entity_component_removal(entity, component_id);
    }

    void ComponentManager::entity_destroyed(Entity &entity)
    {
        for(auto& cv: _impl->component_vectors)
        {
            cv.second->entity_destroyed(entity);
        }
    }
}
