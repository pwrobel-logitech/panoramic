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


glrenderer::sphereinfo glrenderer::mysphereinfo;

void glrenderer::setup_sphere(){

	double pi = 2.0 * glm::asin(1);
	glrenderer::mysphereinfo.R = 100;

	glrenderer::mysphereinfo.nfi = 50;
	glrenderer::mysphereinfo.nteta = 50;
	glrenderer::mysphereinfo.normal_dir = -1.0;

	double d_fi = (2.0*pi)/glrenderer::mysphereinfo.nfi;
	double d_teta = (pi)/glrenderer::mysphereinfo.nteta;
	//double uzyteczna_czesc_textury_x = (double)w/w0;
	double R = glrenderer::mysphereinfo.R ;

	glrenderer::mysphereinfo.sphere_display_list_id = glGenLists (1);
	glNewList(glrenderer::mysphereinfo.sphere_display_list_id, GL_COMPILE);

	glBegin(GL_QUAD_STRIP);
	for(int j=0;j<glrenderer::mysphereinfo.nteta;j++){
		for(int i=0;i<=glrenderer::mysphereinfo.nfi;i++){
			glNormal3d(glrenderer::mysphereinfo.normal_dir*sin(j*d_teta)*cos(i*d_fi),glrenderer::mysphereinfo.normal_dir*sin(j*d_teta)*sin(i*d_fi),glrenderer::mysphereinfo.normal_dir*cos(j*d_teta));
			//glTexCoord2d(((i*d_fi)/(2.0*Math.PI))*uzyteczna_czesc_textury_x,(1-cos(j*d_teta))/2.0);
			glVertex3d(R*sin(j*d_teta)*cos(i*d_fi),R*sin(j*d_teta)*sin(i*d_fi),R*cos(j*d_teta));
			glNormal3d(glrenderer::mysphereinfo.normal_dir*sin((j+1)*d_teta)*cos(i*d_fi),glrenderer::mysphereinfo.normal_dir*sin((j+1)*d_teta)*sin(i*d_fi),glrenderer::mysphereinfo.normal_dir*cos((j+1)*d_teta));
			//glTexCoord2d(((i*d_fi)/(2.0*Math.PI))*uzyteczna_czesc_textury_x,(1-cos((j+1)*d_teta))/2.0);
			glVertex3d(R*sin((j+1)*d_teta)*cos(i*d_fi), R*sin((j+1)*d_teta)*sin(i*d_fi), R*cos((j+1)*d_teta));
        }
	}
	glEnd();
	glEndList();
}
