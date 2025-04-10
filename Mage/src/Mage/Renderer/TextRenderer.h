#pragma once

#include "../Core/Core.h"
#include "Font.h"
#include "Shader.h"

namespace Mage {

    class MAGE_API TextRenderer
    {
        friend class Application;
    public:
        TextRenderer(TextRenderer &&) = delete;

        TextRenderer(const TextRenderer &) = delete;

        TextRenderer &operator=(TextRenderer &&) = delete;

        TextRenderer &operator=(const TextRenderer &) = delete;
        ~TextRenderer();
        void render_text(Font& font, const char* text, float x, float y, float scale, const Color& color);
    private:
        explicit TextRenderer(Window& window);
        struct Impl;
        Impl* _impl;
    };
}
