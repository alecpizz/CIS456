#include "ShapeRenderer.h"

namespace Mage {

    struct ShapeRenderer::Impl
    {
        const char* vs =
            R"( #version 330 core
                
                layout( location = 0 ) in vec2 pos ;

                uniform vec4  camera      ;
                uniform vec2  translation ;
                uniform vec2  scale       ;
                uniform float rotation    ;

                void main()
                {
					mat4 ortho_projection = mat4( 1.0f ) ;
					ortho_projection[ 0 ][ 0 ] = 2.0f / ( camera.y - camera.x ) ;
					ortho_projection[ 1 ][ 1 ] = 2.0f / ( camera.w - camera.z ) ;
					ortho_projection[ 2 ][ 2 ] = -1.0f ;
					ortho_projection[ 3 ][ 0 ] = -1.0f * ( camera.y + camera.x ) / ( camera.y - camera.x ) ;
					ortho_projection[ 3 ][ 1 ] = -1.0f * ( camera.w + camera.z ) / ( camera.w - camera.z ) ;
		
                    mat4 translate_and_translate_half_scale = mat4( 1.0f ) ;
                    translate_and_translate_half_scale[ 3 ][ 0 ] = translation.x + 0.5f * scale.x ;
                    translate_and_translate_half_scale[ 3 ][ 1 ] = translation.y + 0.5f * scale.y ;

					float rotation_radians = radians( rotation ) ;
					
                    mat4 transform_rotation = mat4( 1.0f ) ;
					transform_rotation[ 0 ][ 0 ] = cos( rotation_radians ) ;
					transform_rotation[ 0 ][ 1 ] = -sin( rotation_radians ) ;
					transform_rotation[ 1 ][ 0 ] = sin( rotation_radians ) ;
					transform_rotation[ 1 ][ 1 ] = cos( rotation_radians ) ;
					
                    mat4 translate_half_scale_back = mat4( 1.0f ) ;
		            translate_half_scale_back[ 3 ][ 0 ] = -0.5f * scale.x ;
		            translate_half_scale_back[ 3 ][ 1 ] = -0.5f * scale.y ;

                    mat4 transform_scale = mat4( 1.0f ) ;
                    transform_scale[ 0 ] *= scale.x ;
                    transform_scale[ 1 ] *= scale.y ;
					
                    mat4 transform = translate_and_translate_half_scale ;
					transform *= transform_rotation ;
					transform *= translate_half_scale_back ;
                    transform *= transform_scale ;

                    gl_Position = ortho_projection * transform * vec4( pos, 0.0, 1.0 ) ;
                } )" ;

        const char* fs =
            R"( #version 330 core

                out vec4 color ;

                uniform vec4 in_color ;

                void main()
                {
                    color = in_color ;
                } )" ;

        Shader  shape_shader = Shader( "ShapeShader", vs, fs ) ;
        Camera* camera                               = nullptr ;
        GLint   camera_uniform_loc                         = 0 ;
        GLint   translation_uniform_loc                    = 0 ;
        GLint   scale_uniform_loc                          = 0 ;
        GLint   rotation_uniform_loc                       = 0 ;
        GLint   color_uniform_loc                          = 0 ;
    } ;

    ShapeRenderer::ShapeRenderer( Camera& camera )
    {
        LOG_E_INFO( "ShapeRenderer starting." ) ;
        _impl = new Impl() ;
        _impl->camera = &camera ;
        _impl->camera_uniform_loc      = glGetUniformLocation( _impl->shape_shader.get_shader_id(), "camera" ) ;
        _impl->translation_uniform_loc = glGetUniformLocation( _impl->shape_shader.get_shader_id(), "translation" ) ;
        _impl->scale_uniform_loc       = glGetUniformLocation( _impl->shape_shader.get_shader_id(), "scale"       ) ;
        _impl->rotation_uniform_loc    = glGetUniformLocation( _impl->shape_shader.get_shader_id(), "rotation"    ) ;
        _impl->color_uniform_loc       = glGetUniformLocation( _impl->shape_shader.get_shader_id(), "in_color"    ) ;
    }

    ShapeRenderer::~ShapeRenderer()
    {
        LOG_E_INFO( "ShapeRenderer stopping." ) ;
        delete _impl ;
    }

    void ShapeRenderer::render_shape( const Shape& shape,
        const glm::vec2& translation, const glm::vec2& scale, float rotation, float elapsed_time ) const
    {
        _impl->shape_shader.use() ;

        glUniform4f( _impl->camera_uniform_loc, _impl->camera->left,
            _impl->camera->right, _impl->camera->bottom, _impl->camera->top ) ;
        glUniform2f( _impl->translation_uniform_loc, translation.x, translation.y ) ;
        glUniform2f( _impl->scale_uniform_loc, scale.x, scale.y ) ;
        glUniform1f( _impl->rotation_uniform_loc, rotation ) ;
        glUniform4f( _impl->color_uniform_loc,
            shape.get_color().r_float(), shape.get_color().g_float(),
            shape.get_color().b_float(), shape.get_color().a_float() ) ;

        shape.draw_shape() ;
    }
}
