#include "Exception.h"

namespace Mage
{
    struct Exception::Impl
    {
        std::string msg;
    };

    Exception::Exception(const char *msg)
    {
        _impl = new Impl(); // NOLINT(*-throw-keyword-missing)
        _impl->msg = std::string(msg);
    }

    Exception::~Exception()
    {
        delete _impl;
    }

    const char *Exception::what() const
    {
        return _impl->msg.c_str();
    }

    Exception::Exception(const Exception &src)
    {
        _impl = new Impl(); // NOLINT(*-throw-keyword-missing)
        _impl->msg = src._impl->msg;
    }

    Exception::Exception(Mage::Exception &&src) noexcept
    {
        _impl = src._impl;
        src._impl = nullptr;
    }

    Exception &Exception::operator=(Exception &&src) noexcept
    {
        _impl = new Impl(); // NOLINT(*-throw-keyword-missing)
        if(this != &src)
        {
            _impl = src._impl;
            src._impl = nullptr;
        }
        return *this;
    }

    Exception &Exception::operator=(const Exception &src)
    {
        if (this != &src)
        {
            _impl = new Impl();// NOLINT(*-throw-keyword-missing)
            _impl->msg = src._impl->msg;
        }
        return *this;
    }

}
