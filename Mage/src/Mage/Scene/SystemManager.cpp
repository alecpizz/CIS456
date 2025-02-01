#include "SystemList.h"
#include "SystemManager.h"

namespace Mage {

    struct SystemManager::Impl
    {
        std::mutex sync_object;
        ComponentManager* component_manager;
        std::map<std::type_index, uint32_t> system_type_index_to_id_map;
        std::map<uint32_t, std::vector<uint32_t>> system_id_to_components_map;
        std::map<uint32_t, System*> system_id_to_system_map;
        std::vector<System*> systems;
        uint32_t last_system_id;
    };

    SystemManager::SystemManager()
    {
        LOG_E_INFO("SystemManager Starting");
        _impl = new Impl();
    }

    SystemManager::~SystemManager()
    {
        LOG_E_INFO("SystemManager Stopping");
        delete _impl;
    }

    ComponentManager &SystemManager::get_component_manager() const
    {
        return *_impl->component_manager;
    }

    void SystemManager::set_component_manager(Mage::ComponentManager &componentManager)
    {
        _impl->component_manager = &componentManager;
    }

    SystemList SystemManager::get_all_systems() const
    {
        return {_impl->systems.data(), _impl->systems.size()};
    }

    uint32_t SystemManager::get_system_id(std::type_index type_idx) const
    {
        return _impl->system_type_index_to_id_map[type_idx];
    }

    bool SystemManager::is_system_registered(const Mage::System &system)
    {
        return std::find(_impl->systems.begin(), _impl->systems.end(), &system) != _impl->systems.end();
    }

    void SystemManager::register_system_internal(Mage::System &system)
    {
        std::lock_guard lock(_impl->sync_object);
        auto new_id = ++_impl->last_system_id;
        _impl->systems.push_back(&system);
        auto map_pair = std::make_pair(std::type_index(typeid(system)), new_id);
        _impl->system_type_index_to_id_map.insert(map_pair);
        auto map_pair2 = std::make_pair(new_id, &system);
        _impl->system_id_to_system_map.insert(map_pair2);
    }

}
