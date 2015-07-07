#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
//#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include <cmath>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <vector>

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
		int ntexX;//total number of textures : ntexX * ntexY;
		int ntexY;
		std::vector<char*> teximgs; //pointers to texture data
		std::vector<int>   texids; //texture id's
		int tex_resX; //each tex res xy
		int tex_resY;
	};

	extern worldinfo myworld;
	extern texinfo texdata;

	struct sphereinfo{
		double R;
		int nfi;//number of division units in polar teta angle
		int nteta;
		double normal_dir; //+1 or -1
		int sphere_display_list_id;
		int sphere_grid_display_list_id;
	};

	extern sphereinfo mysphereinfo;

	//Call only from the render thread only !
	bool initGL(int sx, int sy);//init displaylists and pass window size
	void renderGL(bool is_grid, int sx, int sy);//render with grid or not to the given viewport
	void set_viewport_size(int x, int y);
	void setup_projection();
	void setup_sphere();

}
