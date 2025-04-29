//
// Created by alecp on 4/24/2025.
//

#include "SnowRenderer.h"

namespace Mage
{
    struct SnowRenderer::Impl
    {
        const char *vs =
                R"( #version 330 core
                layout (location = 0) in vec3 position;
                layout (location = 1) in vec2 uv;

                out vec2 TexCoords;

                void main()
                {
                    gl_Position = vec4(position, 1.0f);
                    TexCoords = position.xy;
                } )";

        const char *fs =
                R"( #version 330 core
                uniform vec2 resolution;
                uniform float time;
                out vec4 color;
                in vec2 TexCoords;

		void main( )
		{
            const mat3 p = mat3(13.323122,23.5112,21.71123,21.1212,28.7312,11.9312,21.8112,14.7212,61.3934);
                 int LAYERS = 100;
                float DEPTH = 0.2;
                float WIDTH = 0.9 ;
            float SPEED = 0.9  ;   
				vec2 uv = TexCoords.xy;
                vec3 acc = vec3(0.0,0.0,0.0);
                float dof = 5.*sin(time*.1);
                for (int i=0;i<LAYERS;i++)
                {
                        float f = float(i);
                        vec2 q = uv*(1.+f*DEPTH);
                        q += vec2(q.y*(WIDTH*mod(f*7.238917,1.)-WIDTH*.5),SPEED*time/(1.+f*DEPTH*.03));
                        vec3 n = vec3(floor(q),31.189+f);
                        vec3 m = floor(n)*.00001 + fract(n);
                        vec3 mp = (31415.9+m)/fract(mul(m,p));
                        vec3 r = fract(mp);
                        vec2 s = abs(mod(q,1.)-0.5+0.9*r.xy-0.45);
                        s += 0.01*abs(2.*fract(10.*q.yx)-1.0);
                        float d = .6*max(s.x-s.y,s.x+s.y)+max(s.x,s.y)-.01;
                        float edge = .005+.05*min(.5*abs(f-5.-dof),1.);
                        float t = smoothstep(edge,-edge,d)*(r.x/(1.+.02*f*DEPTH));
                        acc += vec3(t,t,t);
                    }
                    color = vec4(vec3(acc),0.25);
		} )";
        Shader snow_shader = Shader("SnowShader", vs, fs);
        GLuint vao = 0;
        GLuint resolution_loc = 0;
        GLuint time_loc = 0;
    };


    SnowRenderer::SnowRenderer()
    {
        _impl = new Impl();
        _impl->resolution_loc = glGetUniformLocation(_impl->snow_shader.get_shader_id(), "resolution");
        _impl->time_loc = glGetUniformLocation(_impl->snow_shader.get_shader_id(), "time");
    }

    SnowRenderer::~SnowRenderer()
    {
        delete _impl;
    }

    void SnowRenderer::render_snow(float time, glm::vec2 window_size)
    {
        if (_impl->vao == 0)
        {
            float quadVertices[] = {
                // positions        // texture Coords
                -1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
                -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
                1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
                1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
            };
            glGenVertexArrays(1, &_impl->vao);
            glGenBuffers(1, &_impl->vao);
            glBindVertexArray(_impl->vao);
            glBindBuffer(GL_ARRAY_BUFFER, _impl->vao);
            glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *) 0);
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *) (3 * sizeof(float)));
        }
        _impl->snow_shader.use();
        glBindVertexArray(_impl->vao);
        glUniform2f(_impl->resolution_loc, window_size.x, window_size.y);
        glUniform1f(_impl->time_loc, time);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        glBindVertexArray(0);
    }
}
