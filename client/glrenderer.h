#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
//#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include <cmath>

namespace glrenderer {

//OpenGL context
extern SDL_GLContext gContext;

bool initGL(int sx, int sy);

void renderGL();

}
