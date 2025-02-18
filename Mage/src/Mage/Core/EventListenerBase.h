#pragma once

#include "Core.h"

namespace Mage
{
    class MAGE_API EventListenerBase
    {
    public:
        EventListenerBase() = default;

        EventListenerBase(EventListenerBase &&) = default;

        EventListenerBase(const EventListenerBase &) = default;

        EventListenerBase &operator=(EventListenerBase &&) = default;

        EventListenerBase &operator=(const EventListenerBase &) = default;

        virtual ~EventListenerBase() = default;
    };
}