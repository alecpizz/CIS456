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

        //TODO: void register_system(System& system)
        template<typename ...Ts>
        void register_system(System &system)
        {
            LOG_E_INFO("System '%s' registered.", typeid(system).name());
            if (is_system_registered(system))
            {
                //TODO: throw exceptions
                LOG_E_ERROR("System is already registered!");
            }
            register_system_internal(system);
            auto system_id = get_system_id(std::type_index(typeid(system)));

            //how the fuck
            //TODO: uncomment this when done with component manager
//            (void) std::initializer_list<int>{
//                    (add_system_component(system_id, get_component_manager().get_component_id<Ts>()), void(), 0)...};
        }

    private:
        struct Impl;
        Impl *_impl;

        SystemManager();

        ComponentManager &get_component_manager() const;

        void set_component_manager(ComponentManager &componentManager);

        uint32_t get_system_id(std::type_index type_idx) const;

        SystemList get_all_systems() const;

        bool is_system_registered(const System &system);

        void register_system_internal(System &system);

        void add_system_component(uint32_t system_id, uint32_t component_id);

        void notify_systems_of_entity_component_additions(Entity &entity, const uint32_t *componentIds,
                                                          size_t componentIdsSize);

        void notify_systems_of_entity_component_removal(Entity &entity, uint32_t component_id);

        void entity_destroyed(Entity &entity);
    };
}
