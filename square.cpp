#include "square.h"

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include "loadpng.h"

class SquarePrivate
{
    public:
        GLuint texture;
};

Square::Square()
: d(new SquarePrivate)
{
    glGenTextures( 1, &d->texture );

    glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );

    int w, h;
    GLubyte* pixels = (GLubyte *)load_png( "square.png", &w, &h );
//     fprintf( stderr, "%d %d\n", w, h );

    glBindTexture( GL_TEXTURE_2D, d->texture );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels );

    free( pixels );
}

Square::~Square()
{
    glDeleteTextures( 1, &d->texture );
    delete d;
}

void Square::display()
{
    glPushMatrix();

    glBindTexture( GL_TEXTURE_2D, d->texture );
//     glTranslatef( 0.0f, 0.5f, 0.0f );

    glBegin( GL_QUADS );
        // front
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f,  0.0f,  1.0f);
        glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f,  0.0f,  1.0f);
        glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f);
        // back
        glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f,  0.0f, -1.0f);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);
        glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);
        glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f,  0.0f, -1.0f);
        // top
        glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f,  1.0f,  1.0f);
        glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f,  1.0f,  1.0f);
        glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);
        // bottom
        glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,  0.0f, -1.0f);
        glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,  0.0f, -1.0f);
        glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f,  0.0f,  1.0f);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f,  0.0f,  1.0f);
        // right
        glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f,  0.0f, -1.0f);
        glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);
        glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f);
        glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f,  0.0f,  1.0f);
        // left
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f,  0.0f, -1.0f);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f,  0.0f,  1.0f);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);
    glEnd();

    glPopMatrix();
}
