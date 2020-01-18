#include <GL/glew.h>
#include <GL/glut.h>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "loadpng.h"

#include "estelle.h"

#include "ground.h"
#include "square.h"

#include "music.h"

Estelle* estelle = NULL;

int eface = 1;

bool keyStates[256] = { false };
bool specialkeyStates[256] = { false };

float cx = 2.0f;
float cy = 0.0f;
float cz = 2.0f;
int cra = 0;
int era = 0;
#define PI 3.1415926

Ground* ground = NULL;
Square* square = NULL;

void key_operations();

void display()
{
    key_operations();

    glClearStencil(0);
    glClearDepth( 1.0f );
    glClearColor( 1.0, 1.0, 1.0, 1.0 );
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT );
    glLoadIdentity();

    //BEGIN scene
    glPushMatrix();
    glTranslatef( 0.0f, 0.0f, -4.0f );
    glRotatef( 40.0f, 1.0f, 0.0f, 0.0f );
    glRotatef( cra, 0.0f, 1.0f, 0.0f );
    glTranslatef( -cx, 0.0f, -cz );

    /// bench for shadow
    glColorMask( GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE );
    glDepthMask( GL_FALSE );
    glEnable( GL_STENCIL_TEST );

    glStencilFunc( GL_ALWAYS, 1, 0xffffffff );
    glStencilOp( GL_REPLACE, GL_REPLACE, GL_REPLACE );

    ground->display();

    glColorMask( GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE );
    glDepthMask( GL_TRUE );

    glStencilFunc( GL_EQUAL, 1, 0xffffffff );
    glStencilOp( GL_KEEP, GL_KEEP, GL_KEEP );

    glDisable( GL_TEXTURE_2D );
    glDisable( GL_DEPTH_TEST );
    glPushMatrix();
    glScalef( 1.0f, -1.0f, 1.0f );
// 
    glTranslatef( 0, 0.0f, 0 );
    glRotatef( 0, 0, 1, 0 );
// 
    glColor4f( 0, 0, 0, 1 );
// 
    square->display();
// 
    glPopMatrix();
// 
    glEnable( GL_DEPTH_TEST );
    glEnable( GL_TEXTURE_2D );

    glDisable( GL_STENCIL_TEST );


    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    /// draw ground
    ground->display();
    glDisable(GL_BLEND);

    /// draw cube
    square->display();

    glPopMatrix();
    //END scene

    //BEGIN camera
    glTranslatef( 0.0f, 0.0f, -4.0f );
    glRotatef( 40.0f, 1.0f, 0.0f, 0.0f );

    /// draw estelle
    glEnable( GL_BLEND );
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
    estelle->display();
    glDisable( GL_BLEND );

    glRotatef( cra, 0.0f, 1.0f, 0.0f );
    glTranslatef( -cx, 0.0f, -cz );
    //END camera

    static int frame = 0;
    static int time = 0;
    static int timebase = 0;
    frame++;
    time = glutGet( GLUT_ELAPSED_TIME );
    if ( time - timebase > 1000 ) {
        fprintf( stderr, "FPS: %4.2f    \r", frame * 1000.0 / ( time - timebase ) );
        timebase = time;
        frame = 0;
    }

    glutSwapBuffers();
}

void reshape( int w, int h )
{
    glViewport( 0, 0, w, h );

    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    gluPerspective( 60, (GLfloat)w/(GLfloat)h, 0.1, 100.0 );

    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();

    /// post redisplay
    glutPostRedisplay();
}

void keyboard( unsigned char key, int x, int y )
{
    keyStates[ key ] = true;

    /// post redisplay
    glutPostRedisplay();
}

void keyboard_up( unsigned char key, int x, int y )
{
    keyStates[ key ] = false;
}

void special( int key, int x, int y )
{
    specialkeyStates[ key ] = true;

    /// post redisplay
    glutPostRedisplay();
}

void special_up( int key, int x, int y )
{
    specialkeyStates[ key ] = false;
}

void update_cface()
{
    int cface;
    if ( cra > era ) {
        cface = eface + (int)( cra - era + 22.5 ) / 45;
        if ( cface >= 8 )
            cface -= 8;
    }
    else {
        cface = eface - (int)( era - cra + 22.5 ) / 45;
        if ( cface < 0 )
            cface += 8;
    }
    estelle->setcface( cface );
}

void estelle_move( float delta_x, float delta_z )
{
    float cx_old = cx;
    float cz_old = cz;

    /// left right move, right positive
    cx += delta_x * cos(cra*PI/180);
    cz += delta_x * sin(cra*PI/180);
    /// forward backward move, backward positive
    cz += delta_z * cos(cra*PI/180);
    cx -= delta_z * sin(cra*PI/180);

//     fprintf( stderr, "move to:  %f %f\n", cx, cz );

    /// check collision
    if ( cx >= -1.1 && cx <= 1.1 && cz >= -1.1 && cz <= 1.1 ) {
        /// move back
        cx = cx_old;
        cz = cz_old;
    }
}

void key_operations()
{
    if ( keyStates[ 'z' ] ) {
        cra -= 2;
        if ( cra < 0 )
            cra += 360;
        update_cface();
        /// post redisplay
        glutPostRedisplay();
    }
    else if ( keyStates[ 'x' ] ) {
        cra += 2;
        if ( cra >= 360 )
            cra -= 360;
        update_cface();
        /// post redisplay
        glutPostRedisplay();
    }

    if ( specialkeyStates[ GLUT_KEY_LEFT ] || specialkeyStates[ GLUT_KEY_RIGHT ] || specialkeyStates[ GLUT_KEY_DOWN ] || specialkeyStates[ GLUT_KEY_UP ] ) {
        era = cra;

        estelle->nextframe();
        /// post redisplay
        glutPostRedisplay();
    }

    if ( specialkeyStates[ GLUT_KEY_LEFT ] ) {
        if ( specialkeyStates[ GLUT_KEY_DOWN ] ) {
            estelle_move( -0.07071f, 0.07071f );
            eface = 0;
            estelle->setcface( 0 );
        }
        else if ( specialkeyStates[ GLUT_KEY_UP ] ) {
            estelle_move( -0.07071f, -0.07071f );
            eface = 6;
            estelle->setcface( 6 );
        }
        else {
            estelle_move( -0.1f, 0.0f );
            eface = 7;
            estelle->setcface( 7 );
        }
    }
    else if ( specialkeyStates[ GLUT_KEY_RIGHT ] ) {
        if ( specialkeyStates[ GLUT_KEY_DOWN ] ) {
            estelle_move( 0.07071f, 0.07071f );
            eface = 2;
            estelle->setcface( 2 );
        }
        else if ( specialkeyStates[ GLUT_KEY_UP ] ) {
            estelle_move( 0.07071f, -0.07071f );
            eface = 4;
            estelle->setcface( 4 );
        }
        else {
            estelle_move( 0.1f, 0.0f );
            eface = 3;
            estelle->setcface( 3 );
        }
    }
    else if ( specialkeyStates[ GLUT_KEY_DOWN ] ) {
        estelle_move( 0.0f, 0.1f );
        eface = 1;
        estelle->setcface( 1 );
    }
    else if ( specialkeyStates[ GLUT_KEY_UP ] ) {
        estelle_move( 0.0f, -0.1f );
        eface = 5;
        estelle->setcface( 5 );
    }
}


void mouse( int button, int state, int x, int y )
{
    if ( button == 3 ) {
        if ( state ) {
            /// wheel up
            cra -= 5;
            if ( cra < 0 )
                cra += 360;
            update_cface();
            /// post redisplay
            glutPostRedisplay();
        }
    }
    else if ( button == 4 ) {
        if ( state ) {
            /// wheel down
            cra += 5;
            if ( cra >= 360 )
                cra -= 360;
            update_cface();
            /// post redisplay
            glutPostRedisplay();
        }
    }
}

int main( int argc, char* argv[] )
{
    glutInit( &argc, argv );
    glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGBA | GLUT_STENCIL );
    glutInitWindowSize( 640, 480 );
    glutInitWindowPosition( 100, 100 );
    glutCreateWindow( "Estelle Test" );

    /// init
    glEnable( GL_DEPTH_TEST );
    glShadeModel( GL_SMOOTH );
    glClearColor( 0, 0, 0, 0 );
    glClearDepth( 1.0f );
    glDepthFunc( GL_LEQUAL );
    glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );

    glEnable( GL_TEXTURE_2D );

    estelle = new Estelle;

    ground = new Ground;
    square = new Square;

    Music* bgm = new Music;
    bgm->play();


    glutDisplayFunc( display );
//     glutIdleFunc( display );
    glutReshapeFunc( reshape );

    glutKeyboardFunc( keyboard );
    glutKeyboardUpFunc( keyboard_up );
    glutSpecialFunc( special );
    glutSpecialUpFunc( special_up );
    glutMouseFunc( mouse );

    glutMainLoop();


    delete bgm;

    delete square;
    delete ground;

    delete estelle;

    return 0;
}
