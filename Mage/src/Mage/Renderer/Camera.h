#pragma once

#include "../Core/Core.h"

namespace Mage {

    class MAGE_API Camera
    {
        friend class Application ;

    public:
        ~Camera() = default ;

        Camera( Camera&& ) = delete ;
        Camera( Camera const& ) = delete ;
        Camera& operator=( Camera&& ) = delete ;
        Camera& operator=( Camera const& ) = delete ;

        float left   ;
        float right  ;
        float top    ;
        float bottom ;

    private:
        Camera() = default ;

    } ;
}
