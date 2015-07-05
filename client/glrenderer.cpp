#include "glrenderer.h"


SDL_GLContext glrenderer::gContext;

bool glrenderer::initGL(int sx, int sy)
{
	bool success = true;
	GLenum error = GL_NO_ERROR;

	glViewport(0, 0, sx, sy);

    //Initialize Projection Matrix
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();

    //Check for error
    error = glGetError();
    if( error != GL_NO_ERROR )
    {
        success = false;
    }

    //Initialize Modelview Matrix
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();

    //Check for error
    error = glGetError();
    if( error != GL_NO_ERROR )
    {
        success = false;
    }

    //Initialize clear color
    glClearColor( 0.f, 0.f, 0.4f, 1.f );

    //Check for error
    error = glGetError();
    if( error != GL_NO_ERROR )
    {
        success = false;
    }

    return success;
}








void glrenderer::renderGL()
{
	GLenum error = GL_NO_ERROR;
	//Initialize Projection Matrix
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();

    //Initialize Modelview Matrix
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();

    //Initialize clear color
    glClearColor( 0.f, 0.f, 0.4f, 1.f );

    //Check for error
    error = glGetError();
    if( error != GL_NO_ERROR )
    {
        printf("a1 : error rendering gl \n");
    }



	glPushMatrix();
    //Clear color buffer
    glClear( GL_COLOR_BUFFER_BIT );

    //Render quad
        glRotatef(0.4f,0.0f,1.0f,0.0f);    // Rotate The cube around the Y axis
        glRotatef(0.2f,1.0f,1.0f,1.0f);
        glColor3f(0.0f,1.0f,0.0f);

        glBegin( GL_QUADS );
            glVertex2f( -0.5f, -0.5f );
            glVertex2f( 0.5f, -0.5f );
            glVertex2f( 0.5f, 0.5f );
            glVertex2f( -0.5f, 0.5f );
        glEnd();
	glPopMatrix();
}


