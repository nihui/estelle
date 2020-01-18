#include "ground.h"

#include <stdlib.h>
#include <GL/gl.h>

#include "loadpng.h"

class GroundPrivate
{
    public:
        GLuint gt;
};

Ground::Ground()
: d(new GroundPrivate)
{
    glGenTextures( 1, &d->gt );

    glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );

    int w, h;
    GLubyte* pixels = (GLubyte *)load_png( "g.png", &w, &h );
//     fprintf( stderr, "%d %d\n", w, h );

    glBindTexture( GL_TEXTURE_2D, d->gt );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels );

    free( pixels );
}

Ground::~Ground()
{
    glDeleteTextures( 1, &d->gt );
    delete d;
}

void Ground::display()
{
    glColor4f( 1, 1, 1, 0.8 );
    glBindTexture( GL_TEXTURE_2D, d->gt );
    glBegin( GL_QUADS );
    glTexCoord2d(  0.0,  0.0 );glVertex3d( -5.0, 0.0, -5.0 );
    glTexCoord2d( 10.0,  0.0 );glVertex3d(  5.0, 0.0, -5.0 );
    glTexCoord2d( 10.0, 10.0 );glVertex3d(  5.0, 0.0,  5.0 );
    glTexCoord2d(  0.0, 10.0 );glVertex3d( -5.0, 0.0,  5.0 );
    glEnd();
}
