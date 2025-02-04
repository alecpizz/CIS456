#pragma once

#include "Core.h"

namespace Mage
{
    class MAGE_API Exception
    {
    public:
        explicit Exception(const char *msg);

        ~Exception();

        Exception(Exception &&) = delete;

        Exception(const Exception &) = delete;

        Exception &operator=(Exception &&) = delete;

        Exception &operator=(const Exception &) = delete;

        const char *what() const;
    private:
        struct Impl;
        Impl* _impl;

    };
}
