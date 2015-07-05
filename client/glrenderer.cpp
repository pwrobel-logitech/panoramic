#include "glrenderer.h"


SDL_GLContext glrenderer::gContext;
glrenderer::worldinfo glrenderer::myworld;
glrenderer::texinfo glrenderer::texdata;



void glrenderer::setup_projection(){
	double pi = 2.0 * glm::asin(1);
	glViewport(0, 0, glrenderer::myworld.resX, glrenderer::myworld.resY);
	//Initialize Projection Matrix
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();


	double yfov = glrenderer::myworld.fovX * ((double)glrenderer::myworld.resY)/((double)glrenderer::myworld.resX) *
			(180.0/pi);

	//glm::dmat4 projection = glm::perspective(yfov,((double)glrenderer::myworld.resX)/((double)glrenderer::myworld.resY) , 0.1, 10.0);
	//glLoadMatrixd( &projection[0][0]);
	//gluPerspective(90.0,((double)glrenderer::myworld.resX)/((double)glrenderer::myworld.resY) , 0.1, 10.0);
	double zNear = 0.2;
	double zFar = 10.0;
	double fy = yfov;
	double fH = tan( (fy / 2.0) / 180.0 * pi ) * zNear;
	double fW = ((double)glrenderer::myworld.resX)/((double)glrenderer::myworld.resY) * fH;
	glFrustum( -fW, fW, -fH, fH, zNear, zFar );//perspective : left, right, bottom, top

    //Check for error
    bool error = glGetError();
    if( error != GL_NO_ERROR )
    {
        printf("Error projection \n");
    }
}

bool glrenderer::initGL(int sx, int sy)
{
	//fill the necessary data

	double pi = 2.0 * glm::asin(1);

	glrenderer::myworld.resX = sx;
	glrenderer::myworld.resY = sy;
	glrenderer::myworld.fovX = pi * (60.0 / 180.0);
	glrenderer::myworld.horizontal_angular_overlap = 0;

	bool success = true;
	GLenum error = GL_NO_ERROR;


	glrenderer::setup_projection();

	glEnable(GL_DEPTH_TEST);

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

	glrenderer::setup_projection();


    //Initialize Modelview Matrix
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();


	//eye, center, up
	//glm::dmat4 view = glm::lookAt(glm::dvec3(0.0, 0.0, 0.0), glm::dvec3(0.0, 0.0, -4.0), glm::dvec3(0.0, 1.0, 0.0));
	//glLoadMatrixd(&view[0][0]);

    //Initialize clear color
    glClearColor( 0.f, 0.f, 0.4f, 1.f );

    //Check for error
    error = glGetError();
    if( error != GL_NO_ERROR )
    {
        printf("a1 : error rendering gl \n");
    }



	//glPushMatrix();
    //Clear color buffer

	glLoadIdentity();


    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    //Render quad
    glColor3d(0.0,1.0,0.0);

	glBegin( GL_QUADS );
		glVertex3d( -0.5, -0.5, -2 );
		glVertex3d( 0.5, -0.5 , -2);
		glVertex3d( 0.5, 0.5, -2 );
		glVertex3d( -0.5, 0.5 , -2);
    glEnd();
	//glPopMatrix();
}


