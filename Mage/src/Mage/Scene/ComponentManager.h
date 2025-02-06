#pragma once

#include "../Core/Core.h"
#include "Entity.h"
#include "ComponentVector.h"

namespace Mage
{

    class SystemManager;

    // Unlike ComponentVector, ComponentManager IS marked MAGE_API.
    // This is because this is a regular class with member function templates.
    // This means this class exists in the DLL, but additional member function template
    //  instantiations can be created for it in the client executable.
    // Doing it this way allows the Application object to "own" the ComponentManager
    //  as it does the other *Manager classes.
    class MAGE_API ComponentManager
    {
        friend class Application;

        friend class EntityManager;

        friend class SystemManager;

    public:
        ~ComponentManager();

        ComponentManager(ComponentManager &&) = delete;

        ComponentManager(const ComponentManager &) = delete;

        ComponentManager &operator=(ComponentManager &&) = delete;

        ComponentManager &operator=(const ComponentManager &) = delete;

        template<typename T>
        void register_component()
        {
            LOG_E_INFO("Component '%s' registered.", typeid(T).name());
            auto component_type_idx = std::type_index(typeid(T)).hash_code();
            uint32_t component_id;
            auto registered = get_component_id(component_type_idx, &component_id);
            if (registered)
            {
                throw Exception("Attempting to register a component that has"
                                " already been registered is not allowed!");
            }
            register_component_internal(component_type_idx, new ComponentVector<T>());
        }

        template<typename T>
        T *get_component(Entity &entity)
        {
            auto component_type_idx = std::type_index(typeid(T)).hash_code();
            uint32_t component_id;
            auto registered = get_component_id(component_type_idx, &component_id);
            if (!registered)
            {
                throw Exception(
                        "Attempting to get a component for an entity when that component has not"
                        " been registered with the component manager is not allowed!");
            }
            return static_cast<ComponentVector<T>*>(get_component_vector(component_id))->get(entity.get_id());
        }

        template<typename T>
        void add_component(Entity &entity, T component)
        {
            LOG_E_DEBUG("Component '%s' added to entity %d.", typeid(T).name(), entity.get_id());
            auto component_type_idx = std::type_index(typeid(T)).hash_code();
            uint32_t component_id;
            auto registered = get_component_id(component_type_idx, &component_id);
            if(!registered)
            {
                throw Exception("Attempting to add a component to an entity when that"
                                "component has not been registered with the componentManager is not allowed!");
            }
            static_cast<ComponentVector<T>*>(get_component_vector(component_id))->add(entity, component);
            notify_systems_of_entity_component_addition(entity, component_id);
        }

        template<typename T>
        void remove_component(Entity &entity)
        {
            LOG_E_DEBUG("Component '%s' removed from entity %d.", typeid(T).name(), entity.get_id());
            auto component_type_idx = std::type_index(typeid(T)).hash_code();
            uint32_t component_id;
            auto registered = get_component_id(component_type_idx, &component_id);
            if(!registered)
            {
                throw Exception("Attempting to remove a component from an entity when that"
                                "component has not been registered with the componentManager is not allowed!");
            }
            static_cast<ComponentVector<T>*>(get_component_vector(component_id))->remove(entity);
            notify_systems_of_entity_component_removal(entity, component_id);
        }

        template<typename T>
        uint32_t get_component_id()
        {
            auto component_type_idx = std::type_index(typeid(T)).hash_code();
            uint32_t component_id;
            auto registered = get_component_id(component_type_idx, &component_id);
            if(!registered)
            {
                throw Exception("Attempting to get a component id of a component that is not registered with"
                                "the componentManager is now allowed!");
            }
            return component_id;
        }

    private:
        ComponentManager();

        struct Impl;
        Impl *_impl;

        void set_system_manager(SystemManager &system);

        void register_component_internal(size_t type_idx, ComponentVectorBase *component_vector);

        bool get_component_id(size_t type_idx, uint32_t *component_id);

        ComponentVectorBase *get_component_vector(uint32_t component_id);

        void notify_systems_of_entity_component_addition(Entity &entity, uint32_t component_id);

        void notify_systems_of_entity_component_removal(Entity &entity, uint32_t component_id);

        void entity_destroyed(Entity &entity);
    };
}
