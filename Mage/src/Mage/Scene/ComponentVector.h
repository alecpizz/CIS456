#pragma once

#include "../Core/Core.h"
#include "EntityList.h"

namespace Mage
{
    class MAGE_API ComponentVectorBase
    {
    public:
        ComponentVectorBase() = default;

        virtual ~ComponentVectorBase() = default;

        ComponentVectorBase(ComponentVectorBase &&) = delete;

        ComponentVectorBase(const ComponentVectorBase &) = delete;

        ComponentVectorBase &operator=(ComponentVectorBase &&) = delete;

        ComponentVectorBase &operator=(const ComponentVectorBase &) = delete;

        virtual void entity_destroyed(Entity &entity) = 0;

        virtual EntityList get_entities() = 0;
    };

    template<typename T>
    class ComponentVector final : public ComponentVectorBase
    {
    public:
        ComponentVector() = default;

        ~ComponentVector() override = default;

        ComponentVector(ComponentVector &&) = delete;

        ComponentVector(const ComponentVector &) = delete;

        ComponentVector &operator=(ComponentVector &&) = delete;

        ComponentVector &operator=(const ComponentVector &) = delete;

        T *get(uint64_t entity_id)
        {
            if (!_entity_to_index_map.contains(entity_id))
            {
                return nullptr;
            }
            return &_components.at(_entity_to_index_map[entity_id]);
        }

        void add(Entity &entity, T component)
        {
            auto eid = entity.get_id();
            if (_entity_to_index_map.contains(eid))
            {
                throw Exception(
                        "Attempting to add a component that an entity that already has that component is not allowed!");
            }
            _entities.push_back(&entity);
            _components.push_back(component);

            //why do we use insert or assign here?
            _entity_to_index_map.insert_or_assign(eid, _components.size() - 1);
            _index_to_entity_map.insert_or_assign(_components.size() - 1, eid);
        }

        void remove(const Entity &entity)
        {
            auto eid = entity.get_id();

            if (!_entity_to_index_map.contains(eid))
            {
                return;
            }

            //move the last item in the vector to the place where the one being removed is
            auto idx_of_removed_component = _entity_to_index_map.at(eid);
            auto idx_of_last_component = _components.size() - 1;
            _components[idx_of_removed_component] = _components.at(idx_of_last_component);

            //now idx of removed component is really the index of what was the last component in the vector
            auto idx_of_arbitrary_other_component = idx_of_removed_component;

            //update the maps to reflect the old last component's new position
            auto entity_id_of_arbitrary_other_component = _index_to_entity_map.at(idx_of_last_component);
            _entity_to_index_map[entity_id_of_arbitrary_other_component] = idx_of_arbitrary_other_component;
            _index_to_entity_map[idx_of_arbitrary_other_component] = entity_id_of_arbitrary_other_component;

            // remove the now incorrect entries
            _entity_to_index_map.erase(eid);
            _index_to_entity_map.erase(idx_of_last_component);

            // remove the entity from the list of entities this vector has components for
            _entities.erase(std::remove(_entities.begin(), _entities.end(), &entity));

            // remove the last component of the vector
            _components.erase(_components.end() - 1, _components.end());
        }

        void entity_destroyed(Entity &entity) override
        {
            remove(entity);
        }

        EntityList get_entities() override
        {
            return {_entities.data(), _entities.size()};
        }

    private:
        //these are ok because this is a class template,
        //so it will be compiled with the game and it's version, thus
        // using whatever std cpp lib version they have
        std::vector<T> _components;
        std::vector<Entity *> _entities;
        std::unordered_map<uint64_t, uint64_t> _entity_to_index_map;
        std::unordered_map<uint64_t, uint64_t> _index_to_entity_map;

    };
}
