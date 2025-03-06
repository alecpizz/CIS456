#pragma once

#include "../Core/Core.h"
#include "Camera.h"
#include "Shader.h"
#include "Sprite.h"

namespace Mage
{
    class MAGE_API SpriteRenderer
    {
        friend class Application ;

    public:
        SpriteRenderer( SpriteRenderer&& ) = delete ;
        SpriteRenderer( SpriteRenderer const& ) = delete ;
        SpriteRenderer& operator=( SpriteRenderer&& ) = delete ;
        SpriteRenderer& operator=( SpriteRenderer const& ) = delete ;

        ~SpriteRenderer() ;

        void render_sprite( const Sprite& sprite, const glm::vec2& translation,
            const glm::vec2& scale, float rotation, float elapsed_time ) const ;

    private:
        explicit SpriteRenderer( Camera& camera ) ;

        struct Impl ;
        Impl* _impl ;
    };
}
