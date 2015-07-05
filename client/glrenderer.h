#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
//#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include <cmath>
#include "glm/glm.hpp"

namespace glrenderer {

//OpenGL context
extern SDL_GLContext gContext;

//world rendering info
struct worldinfo{
	int resX;
	int resY;
	double fovX;// total angle in radians
	double horizontal_angular_overlap; //in radians, =0 by default
	glm::mat4 projection;
	glm::mat4 view_central_cam;
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
