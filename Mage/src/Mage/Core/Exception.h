#pragma once

#include "Core.h"

namespace Mage
{
    class MAGE_API Exception
    {
    public:
        explicit Exception(const char *msg);

        ~Exception();

        Exception(Exception &&) noexcept;

        Exception(const Exception &);

        Exception &operator=(Exception &&) noexcept ;

        Exception &operator=(const Exception &) ;

        const char *what() const;
    private:
        struct Impl;
        Impl* _impl;

    };
}
