#pragma once

#include "../Core/Core.h"
#include "Camera.h"
#include "Shader.h"
#include "Shape.h"

namespace Mage {

    class MAGE_API ShapeRenderer
    {
        friend class Application ;

    public:
        ShapeRenderer( ShapeRenderer&& ) = delete ;
        ShapeRenderer( ShapeRenderer const& ) = delete ;
        ShapeRenderer& operator=( ShapeRenderer&& ) = delete ;
        ShapeRenderer& operator=( ShapeRenderer const& ) = delete ;

        ~ShapeRenderer() ;

        void render_shape( const Shape& shape, const glm::vec2& translation,
            const glm::vec2& scale, float rotation, float elapsed_time ) const ;

    private:
        explicit ShapeRenderer( Camera& camera ) ;

        struct Impl ;
        Impl* _impl ;

    } ;

}
