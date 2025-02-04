#include "Exception.h"

namespace Mage
{
    struct Exception::Impl
    {
        std::string msg;
    };

    Exception::Exception(const char* msg)
    {
        _impl = new Impl(); // NOLINT(*-throw-keyword-missing)
        _impl->msg = std::string(msg);
    }

    Exception::~Exception()
    {
        delete _impl;
    }

    const char* Exception::what() const
    {
        return _impl->msg.c_str();
    }
}
