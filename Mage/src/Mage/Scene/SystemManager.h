#pragma once

#include "../Core/Core.h"
#include "System.h"
#include "ComponentManager.h"

namespace Mage
{

    class SystemList;

    class MAGE_API SystemManager
    {
        friend class Application;

        friend class ComponentManager;

        friend class EntityManager;

    public:
        ~SystemManager();

        SystemManager(SystemManager &&) = delete;

        SystemManager(const SystemManager &) = delete;

        SystemManager &operator=(SystemManager &&) = delete;

        SystemManager &operator=(const SystemManager &) = delete;

        template<typename ...Ts>
        void register_system(System &system)
        {
            LOG_E_INFO("System '%s' registered.", typeid(system).name());
            if (is_system_registered(system))
            {
                throw Exception("Attempting to register a system that has already been registered is not allowed!");
            }
            register_system_internal(system);
            auto system_id = get_system_id(std::type_index(typeid(system)).hash_code());

            //how the fuck
            (void) std::initializer_list<int>{
                    (add_system_component(system_id, get_component_manager().get_component_id<Ts>()), 0)...};
        }

    private:
        struct Impl;
        Impl *_impl;

        SystemManager();

        //these next 4 are only used by sts manager, but not friend classes. but still need to be in header
        //but not in impl due to register_system above referencing them
        ComponentManager &get_component_manager() const;

        void set_component_manager(ComponentManager &componentManager);

        bool is_system_registered(const System &system);

        uint32_t get_system_id(size_t type_idx) const;

        void register_system_internal(System &system);

        SystemList get_all_systems() const;

        void add_system_component(uint32_t system_id, uint32_t component_id);

        void notify_systems_of_entity_component_additions(Entity &entity, const uint32_t *componentIds,
                                                          size_t componentIdsSize);

        void notify_systems_of_entity_component_removal(Entity &entity, uint32_t component_id);

        void entity_destroyed(Entity &entity);
    };
}
