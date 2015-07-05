#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
//#include <SDL2/SDL_image.h>
#include <GL/glu.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include <cmath>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace glrenderer {

//OpenGL context
extern SDL_GLContext gContext;

//world rendering info
struct worldinfo{
	int resX;
	int resY;
	double fovX;// total angle in radians
	double horizontal_angular_overlap; //in radians, =0 by default
	glm::dmat4 projection;
	glm::dmat4 view_central_cam;
};

//minimal texture info
struct texinfo{
	int ntexX;
	int ntexY;
};

extern worldinfo myworld;
extern texinfo texdata;

bool initGL(int sx, int sy);

void renderGL();

void setup_projection();

}
