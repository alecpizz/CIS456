#pragma once
#include "../Core/Core.h"
#include "Camera.h"
#include "Shader.h"

namespace Mage
{
    class SnowRenderer
    {
        friend class Application;
    public:
        SnowRenderer( SnowRenderer&& ) = delete ;
        SnowRenderer( SnowRenderer const& ) = delete ;
        SnowRenderer& operator=( SnowRenderer&& ) = delete ;
        SnowRenderer& operator=( SnowRenderer const& ) = delete ;
        ~SnowRenderer();

        void render_snow(float time, glm::vec2 window_size);
    private:
        SnowRenderer();
        struct Impl;
        Impl* _impl;
    };
}
