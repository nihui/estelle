#include "estelle.h"

#include <stdio.h>
#include <stdlib.h>
#include <GL/gl.h>

#include "loadpng.h"

class EstellePrivate
{
    public:
        GLuint texture[8][8];
        GLuint shadowt;
        int step;
        int cface;
};

Estelle::Estelle()
: d(new EstellePrivate)
{
    d->step = 7;
    d->cface = 1;

    {
        /// init body textures
        glGenTextures( 64, (GLuint*)d->texture );
        glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );

        int w, h;
        GLubyte* pixels = (GLubyte *)load_png( "a.png", &w, &h );
    //     fprintf( stderr, "%d %d\n", w, h );

        glPixelStorei( GL_UNPACK_ROW_LENGTH, w );
        for ( int i = 0; i < 8; ++i ) {
            for ( int j = 0; j < 8; ++j ) {
                glBindTexture( GL_TEXTURE_2D, d->texture[i][j] );

                glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP );
                glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP );
                glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
                glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );

                glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, w, w, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels + (i*8+j)*w*w*4 );
            }
        }
        glPixelStorei( GL_UNPACK_ROW_LENGTH, 0 );

        free( pixels );
    }

    {
        /// init shadow texture
        glGenTextures( 1, &d->shadowt );
        glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );

        int w, h;
        GLubyte* pixels = (GLubyte *)load_png( "sh.png", &w, &h );
//         fprintf( stderr, "%d %d\n", w, h );

        glBindTexture( GL_TEXTURE_2D, d->shadowt );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
        glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels );

        free( pixels );
    }
}

Estelle::~Estelle()
{
    glDeleteTextures( 1, &d->shadowt );
    glDeleteTextures( 64, (GLuint*)d->texture );
    delete d;
}

void Estelle::display()
{
    /// estelle drop shadow
    glBindTexture( GL_TEXTURE_2D, d->shadowt );
    glBegin( GL_QUADS );
    glTexCoord2d( 0.0, 0.0 );glVertex3d( -1.0, 0.0001, -1.0 );
    glTexCoord2d( 1.0, 0.0 );glVertex3d(  1.0, 0.0001, -1.0 );
    glTexCoord2d( 1.0, 1.0 );glVertex3d(  1.0, 0.0001,  1.0 );
    glTexCoord2d( 0.0, 1.0 );glVertex3d( -1.0, 0.0001,  1.0 );
    glEnd();
    /// estelle body
    glBindTexture( GL_TEXTURE_2D, d->texture[d->step][d->cface] );
    glBegin( GL_QUADS );
    glTexCoord2d( 0.0, 0.0 );glVertex2d( -1.0, -0.64 );
    glTexCoord2d( 1.0, 0.0 );glVertex2d(  1.0, -0.64 );
    glTexCoord2d( 1.0, 1.0 );glVertex2d(  1.0,  1.36 );
    glTexCoord2d( 0.0, 1.0 );glVertex2d( -1.0,  1.36 );
    glEnd();
}

void Estelle::nextframe()
{
    --d->step;
    if ( d->step < 0 )
        d->step = 7;
}

void Estelle::setcface( int cface )
{
//     fprintf( stderr, "cface: %d\n", cface );
    d->cface = cface;
}
