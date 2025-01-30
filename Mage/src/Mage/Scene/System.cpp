#include "System.h"

namespace Mage
{
    struct System::Impl
    {
        std::vector<Entity *> entities;
    };

    System::System()
    {
        _impl = new Impl();
    }

    System::~System()
    {
        delete _impl;
    }

    void System::add_entity(Mage::Entity &entity)
    {
        if (std::find(_impl->entities.begin(), _impl->entities.end(), &entity) == _impl->entities.end())
        {
            _impl->entities.push_back(&entity);
        }
    }

    void System::remove_entity(Mage::Entity &entity)
    {
        _impl->entities.erase(std::remove(_impl->entities.begin(),
                                          _impl->entities.end(), &entity), _impl->entities.end());
    }

    EntityList System::get_entities() const
    {
        return {_impl->entities.data(), _impl->entities.size()};
    }
}
