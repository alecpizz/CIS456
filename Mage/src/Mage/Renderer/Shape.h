#pragma once

#include "../Core/Core.h"

namespace Mage {

    class MAGE_API Shape
    {
    public:
        Shape() ;

        Shape( Shape&& ) = delete ;
        Shape( Shape const& ) = delete ;
        Shape& operator=( Shape&& ) = delete ;
        Shape& operator=( Shape const& ) = delete ;

        ~Shape() ;

        Color& get_color() const ;

        void set_color( const Color& color ) ;

        void draw_shape() const ;

    private:
        struct Impl ;
        Impl* _impl ;

    } ;

}
