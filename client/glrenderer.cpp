#include "glrenderer.h"

SDL_GLContext glrenderer::gContext;
glrenderer::worldinfo glrenderer::myworld;
glrenderer::texinfo glrenderer::texdata;

double default_fovX = 90;//degrees

void glrenderer::setup_projection(){
	double pi = 2.0 * glm::asin(1);
	glViewport(0, 0, glrenderer::myworld.resX, glrenderer::myworld.resY);
	//Initialize Projection Matrix
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	glrenderer::myworld.fovX = pi * (default_fovX / 180.0);
	double yfov = (180.0/pi)*2.0*atan((((double)glrenderer::myworld.resY)/((double)glrenderer::myworld.resX))*tan(glrenderer::myworld.fovX/2.0));
	//glm::dmat4 projection = glm::perspective(yfov,((double)glrenderer::myworld.resX)/((double)glrenderer::myworld.resY) , 0.1, 10.0);
	//glLoadMatrixd( &projection[0][0]);
	//gluPerspective(90.0,((double)glrenderer::myworld.resX)/((double)glrenderer::myworld.resY) , 0.1, 10.0);
	double zNear = 0.2;
	double zFar = 1000.0;
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

void glrenderer::set_viewport_size(int x, int y){
	glrenderer::myworld.resX = x;
	glrenderer::myworld.resY = y;
}

bool glrenderer::initGL(int sx, int sy)
{
	glEnable(GL_DEPTH_TEST);
	//Edit Modelview Matrix
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();
	glrenderer::setup_sphere();
	return false;
}

void glrenderer::renderGL(bool is_grid, int sx, int sy)
{
	GLenum error = GL_NO_ERROR;
	double pi = 2.0 * glm::asin(1);
	glrenderer::myworld.resX = sx;
	glrenderer::myworld.resY = sy;
	glrenderer::myworld.fovX = pi * (default_fovX / 180.0);
	glrenderer::myworld.horizontal_angular_overlap = 0;
	glrenderer::setup_projection();
	glViewport(0, 0, glrenderer::myworld.resX, glrenderer::myworld.resY);
	//Initialize Modelview Matrix
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();

	//eye, center, up
	//glm::dmat4 view = glm::lookAt(glm::dvec3(0.0, 0.0, 0.0), glm::dvec3(0.0, 0.0, -4.0), glm::dvec3(1.0, 1.0, 0.0));
	//glLoadMatrixd(&view[0][0]);
	//clear color
	glClearColor( 0.f, 0.f, 0.4f, 1.f );

	//Check for error
	error = glGetError();
	if( error != GL_NO_ERROR )
	{
		printf("a1 : error rendering gl \n");
	}
	//Clear color buffer and depth buffer
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	glPushMatrix();
	glRotated(glrenderer::myworld.center_teta, 1.0, 0.0, 0.0);
	glRotated(glrenderer::myworld.center_fi, 0.0, 0.0, 1.0);
	//Render sphere
	glCallList(glrenderer::mysphereinfo.sphere_display_list_id);
	//Render spherical grid
	if(is_grid)
		glCallList(glrenderer::mysphereinfo.sphere_grid_display_list_id);
	glPopMatrix();
	//Render quad
	glColor3d(0.0,1.0,0.0);

    glEnable( GL_TEXTURE_2D );
    //glGenTextures(1, &mtex0id);
    //glBindTexture(GL_TEXTURE_2D, mtex0id);

    int Mode = GL_RGB;
    printf(" QQ surf : %d\n", glrenderer::surf);
    if(glrenderer::surf->format->BytesPerPixel == 4) {
        Mode = GL_RGBA;
    }

    glTexImage2D(GL_TEXTURE_2D, 0, Mode, glrenderer::surf->w, glrenderer::surf->h, 0, Mode, GL_UNSIGNED_BYTE, glrenderer::surf->pixels);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glLoadIdentity();
    //glBindTexture(GL_TEXTURE_2D, mtex0id);
	glBegin( GL_QUADS );
		glTexCoord2f(0, 0);glVertex3d( -0.5, -0.5, -2 );
		glTexCoord2f(1, 0);glVertex3d( 0.5, -0.5 , -2);
		glTexCoord2f(1, 1);glVertex3d( 0.5, 0.5, -2 );
		glTexCoord2f(0, 1);glVertex3d( -0.5, 0.5 , -2);
	glEnd();
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
	glColor3d(1.0, 0.0, 0.0);
	glPushMatrix();
	//glRotated(-90.0, 0.0, 1.0, 0.0);
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

	R *= 0.99;//little smaller sphere for the grid
	int Ngrid_fi = 40;
	int Ngrid_teta = 40;
	glrenderer::mysphereinfo.sphere_grid_display_list_id = glGenLists (1); //cache into display list for faster drawing later
	glNewList(glrenderer::mysphereinfo.sphere_grid_display_list_id, GL_COMPILE);
	d_fi = (2.0*pi)/Ngrid_fi;
	d_teta = (pi)/Ngrid_teta;
	glBegin(GL_LINES);
	glColor3d(0.0, 1.0, 0.0);
	//generate longintudal lines - from poles
	for(int j=0;j<glrenderer::mysphereinfo.nteta;j++){
		for(int i=0;i<=glrenderer::mysphereinfo.nfi;i++){
			glVertex3d(R*sin(j*d_teta)*cos(i*d_fi), R*sin(j*d_teta)*sin(i*d_fi), R*cos(j*d_teta));
			glVertex3d(R*sin((j+1)*d_teta)*cos(i*d_fi), R*sin((j+1)*d_teta)*sin(i*d_fi), R*cos((j+1)*d_teta));
		}
	}
	//generate lattitudal lines - equatorial
	for(int j=0;j<glrenderer::mysphereinfo.nteta;j++){
		for(int i=0;i<=glrenderer::mysphereinfo.nfi;i++){
			glVertex3d(R*sin(j*d_teta)*cos(i*d_fi), R*sin(j*d_teta)*sin(i*d_fi), R*cos(j*d_teta));
			glVertex3d(R*sin(j*d_teta)*cos((i+1)*d_fi), R*sin(j*d_teta)*sin((i+1)*d_fi), R*cos(j*d_teta));
		}
	}
	glEnd();
	glPopMatrix();
	glEndList();
}

GLuint glrenderer::mtex0id = 0;
SDL_Surface *glrenderer::surf;

void glrenderer::setup_textures(){

    //glEnable( GL_TEXTURE_2D );
    //glGenTextures(1, &mtex0id);
    //glBindTexture(GL_TEXTURE_2D, mtex0id);

    //int Mode = GL_RGB;

    //if(glrenderer::surf->format->BytesPerPixel == 4) {
    //    Mode = GL_RGBA;
   // }

    //glTexImage2D(GL_TEXTURE_2D, 0, Mode, glrenderer::surf->w, glrenderer::surf->h, 0, Mode, GL_UNSIGNED_BYTE, glrenderer::surf->pixels);

    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}
