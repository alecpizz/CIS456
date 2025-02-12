#include "Shape.h"

namespace Mage {

    struct Shape::Impl
    {
        Color  color = Color::white ;
        GLuint vao   =            0 ;
        GLuint vbo   =            0 ;
    } ;

    Shape::Shape()
    {
        LOG_E_INFO( "Loading Shape..." ) ;
        _impl = new Impl() ;

        auto first_pos_byte = reinterpret_cast<void*>( 0 ) ;

        glGenVertexArrays( 1, &_impl->vao ) ;
        glGenBuffers( 1, &_impl->vbo ) ;
        glBindVertexArray( _impl->vao ) ;
        glBindBuffer( GL_ARRAY_BUFFER, _impl->vbo ) ;
        glBufferData( GL_ARRAY_BUFFER, sizeof( float ) * 6 * 2, nullptr, GL_DYNAMIC_DRAW ) ;
        glVertexAttribPointer( 0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof( float ), first_pos_byte ) ;
        glEnableVertexAttribArray( 0 ) ;
        glBindBuffer( GL_ARRAY_BUFFER, 0 ) ;
        glBindVertexArray( 0 ) ;
    }

    Shape::~Shape()
    {
        LOG_E_INFO( "Unloading Shape..." ) ;
        glDeleteBuffers( 1, &_impl->vbo ) ;
        glDeleteBuffers( 1, &_impl->vao ) ;
        delete _impl ;
    }

    Color& Shape::get_color() const
    {
        return _impl->color ;
    }

    void Shape::set_color( const Color& color )
    {
        _impl->color = color ;
    }

    void Shape::draw_shape() const
    {
        float vertices[ 6 ][ 2 ] = {
            // pos    
          { 0.0f, 1.0f },
          { 1.0f, 0.0f },
          { 0.0f, 0.0f },
          { 0.0f, 1.0f },
          { 1.0f, 1.0f },
          { 1.0f, 0.0f }
        } ;

        glBindVertexArray( _impl->vao ) ;
        glBindBuffer( GL_ARRAY_BUFFER, _impl->vbo ) ;
        glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof( vertices ), vertices ) ;
        glBindBuffer( GL_ARRAY_BUFFER, 0 ) ;
        glDrawArrays( GL_TRIANGLES, 0, 6 ) ;
        glBindVertexArray( 0 ) ;
        glBindTexture( GL_TEXTURE_2D, 0 ) ;
    }
}
